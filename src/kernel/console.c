#include "onix/console.h"
#include "onix/types.h"
#include "onix/debug.h"
#include "onix/clock.h"

#define CRT_ADDR_REG 0x3D4              // CRT 索引寄存器
#define CRT_DATA_REG 0x3D5              // CRT 数据寄存器

#define CRT_START_ADDR_H 0xC            // CRT 显存起始地址高字节
#define CRT_START_ADDR_L 0xD            // CRT 显存起始地址低字节
#define CRT_CURSOR_H 0xE                // CRT 光标位置高字节
#define CRT_CURSOR_L 0xF                // CRT 光标位置低字节

#define MEM_BASE 0xB8000                // 显卡内存基地址
#define MEM_SIZE 0x4000                 // 显卡内存大小
#define MEM_END (MEM_BASE + MEM_SIZE)   // 显卡内存结束地址
#define WIDTH 80                        // 屏幕文本宽度
#define HEIGHT 25                       // 屏幕文本高度
#define ROW_SIZE (WIDTH * 2)            // 每行字节数
#define SCREEN_SIZE (ROW_SIZE * HEIGHT) // 屏幕字节数

#define ASCII_NULL 0x0
#define ASCII_ENQ  0x5
#define ASCII_BEL  0x7                    // \a
#define ASCII_BS   0x8                    // \b
#define ASCII_HT   0x9                    // \t
#define ASCII_LF   0xA                    // \n
#define ASCII_VT   0xB                    // \v
#define ASCII_FF   0xC                    // \f
#define ASCII_CR   0xD                    // \r
#define ASCII_DEL  0x7F

// 当前显示器开始的内存位置
// 因为显存总共有0x4000B，但是显示器只能显示4000B的内容，因此可以选择显示器显示的内容在显存的哪个位置
static uint32 screen;

// 记录当前光标的内存位置
static uint32 pos;

// 记录当前光标的屏幕位置
static uint32 x, y;

// 字符样式
static uint8 attr = 0x07; // 黑底白字

// 空格
static uint16 erase = 0x0720; // 黑底白字的空格

// 读取显示器开始的内存位置
static void get_screen()
{
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);
    screen = inb(CRT_DATA_REG) << 8;
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);
    screen |= inb(CRT_DATA_REG);

    screen <<= 1; // 乘以2，因为显存中的每个单元是16位的
    screen += MEM_BASE; // 加上显存基地址
}

// 设置当前显示器开始的内存位置
static void set_screen()
{
    uint32 addr = (screen - MEM_BASE) >> 1;
    outb(CRT_ADDR_REG, CRT_START_ADDR_H);
    outb(CRT_DATA_REG, addr >> 8);
    outb(CRT_ADDR_REG, CRT_START_ADDR_L);
    outb(CRT_DATA_REG, addr & 0xFF);
}

// 获得当前光标位置
static void get_cursor()
{
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    pos = inb(CRT_DATA_REG) << 8;
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    pos |= inb(CRT_DATA_REG);

    pos <<= 1; // 乘以2，因为显存中的每个单元是16位的
    pos += MEM_BASE; // 加上显存基地址

    // 计算光标的屏幕位置
    x = (pos - screen) / 2 % WIDTH;
    y = (pos - screen) / 2 / WIDTH;
}

static void set_cursor()
{
    uint32 addr = (pos - MEM_BASE) >> 1;
    outb(CRT_ADDR_REG, CRT_CURSOR_H);
    outb(CRT_DATA_REG, addr >> 8);
    outb(CRT_ADDR_REG, CRT_CURSOR_L);
    outb(CRT_DATA_REG, addr & 0xFF);
}

void console_init(void)
{
    console_clear();
}

void console_clear(void)
{
    screen = MEM_BASE;
    pos = MEM_BASE;
    x = y = 0;
    set_cursor();
    set_screen();

    uint16 *ptr = (uint16 *)MEM_BASE;
    for (int i = 0; i < MEM_SIZE / 2; i++)
    {
        *ptr++ = erase;
    }
}

static void command_bs()
{
    if(x)
    {
        x--;
        pos -= 2;
        *(uint16 *)pos = erase;
    }
}

static void command_del()
{
    *(uint16 *)pos = erase;
}

static void command_cr()
{
    pos -= x * 2;
    x = 0;
}

static void scroll_up()
{
    if (screen + SCREEN_SIZE + ROW_SIZE >= MEM_END){
        memcpy((void *)MEM_BASE, (void *)screen, SCREEN_SIZE);
        pos -= (screen - MEM_BASE);
        screen = MEM_BASE;
        BMB;
    }

    uint32 *ptr = (uint32 *)(screen + SCREEN_SIZE);
    for (int i = 0; i < WIDTH; i++)
    {
        *ptr++ = erase;
    }
    screen += ROW_SIZE;
    pos += ROW_SIZE;
    set_screen();
}

static void command_lf()
{
    if (y + 1 < HEIGHT)
    {
        y++;
        pos += ROW_SIZE;
        return;
    }
    scroll_up();
}

void console_write(char *buf, uint32 count)
{
    char ch;
    while (count--)
    {
        ch = *buf++;
        switch (ch)
        {
        case ASCII_NULL:
            break;
        case ASCII_ENQ:
            break;
        case ASCII_BEL:
            start_beep();
            break;
        case ASCII_BS:
            command_bs();
            break;
        case ASCII_HT:
            break;
        case ASCII_LF:
            command_lf();
            command_cr();
            break;
        case ASCII_VT:
            break;
        case ASCII_FF:
            command_lf();
            break;
        case ASCII_CR:
            command_cr();
            break;
        case ASCII_DEL:
            command_del();
            break;
        
        default:
            if (x >= WIDTH) {
                x -= WIDTH;
                pos -= ROW_SIZE;
                command_lf();
            }
            
            *((char *)pos) = ch;
            pos++;
            *((char *)pos) = attr;
            pos++;
            x++;
            break;
        }
        set_cursor();
    }
}
