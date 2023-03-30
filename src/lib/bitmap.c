#include <onix/bitmap.h>
#include <onix/string.h>
#include <onix/onix.h>
#include <onix/assert.h>

// 构造位图
void bitmap_make(bitmap_t *map, char *bits, uint32 length, uint32 offset)
{
    map->bits = bits;
    map->length = length;
    map->offset = offset;
}

// 位图初始化，全部置为 0
// map: 位图
// bits: 存储位图的数组
// length: bits 数组的长度
// start: 位图的起始位置，如果想在数组的中间开始存储位图，就需要设置这个参数，如果不为0.则之后设置位图的时候，index需要大于等于start
void bitmap_init(bitmap_t *map, char *bits, uint32 length, uint32 start)
{
    memset(bits, 0, length);
    bitmap_make(map, bits, length, start);
}

// 测试某一位是否为 1
bool bitmap_test(bitmap_t *map, idx_t index)
{
    assert(index >= map->offset);

    // 得到位图的索引
    idx_t idx = index - map->offset;

    // 位图数组中的字节
    uint32 bytes = idx / 8;

    // 该字节中的那一位
    uint8 bits = idx % 8;

    assert(bytes < map->length);

    // 返回那一位是否等于 1
    return (map->bits[bytes] & (1 << bits));
}

// 设置位图某位的值
void bitmap_set(bitmap_t *map, idx_t index, bool value)
{
    // value 必须是二值的
    assert(value == 0 || value == 1);

    assert(index >= map->offset);

    // 得到位图的索引
    idx_t idx = index - map->offset;

    // 位图数组中的字节
    uint32 bytes = idx >> 3;

    // 该字节中的那一位
    uint8 bits = idx & 0x07;
    if (value)
    {
        // 置为 1
        map->bits[bytes] |= (1 << bits);
    }
    else
    {
        // 置为 0
        map->bits[bytes] &= ~(1 << bits);
    }
}

// 从位图中得到连续的 count 位 返回第一个位的索引
int bitmap_scan(bitmap_t *map, uint32 count)
{
    int start = EOF;                 // 标记目标开始的位置
    uint32 bits_left = map->length * 8; // 剩余的位数
    uint32 next_bit = 0;                // 下一个位
    uint32 counter = 0;                 // 计数器

    // 从头开始找
    while (bits_left-- > 0)
    {
        if (!bitmap_test(map, map->offset + next_bit))
        {
            // 如果下一个位没有占用，则计数器加一
            counter++;
        }
        else
        {
            // 否则计数器置为 0，继续寻找
            counter = 0;
        }

        // 下一位，位置加一
        next_bit++;

        // 找到数量一致，则设置开始的位置，结束
        if (counter == count)
        {
            start = next_bit - count;
            break;
        }
    }

    // 如果没找到，则返回 EOF(END OF FILE)
    if (start == EOF)
        return EOF;

    // 否则将找到的位，全部置为 1
    bits_left = count;
    next_bit = start;
    while (bits_left--)
    {
        bitmap_set(map, map->offset + next_bit, true);
        next_bit++;
    }

    // 然后返回索引
    return start + map->offset;
}

#include <onix/debug.h>

#define LOGK(fmt, args...) DEBUGK(fmt, ##args)

#define LEN 2
uint8 buf[LEN];
bitmap_t map;

void bitmap_tests()
{
    bitmap_init(&map, buf, LEN, 0);
    for (size_t i = 0; i < 33; i++)
    {
        idx_t idx = bitmap_scan(&map, 1);
        if (idx == EOF)
        {
            LOGK("TEST FINISH\n");
            break;
        }
        LOGK("%d\n", idx);
    }
}