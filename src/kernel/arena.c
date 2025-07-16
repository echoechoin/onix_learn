#include <os/arena.h>
#include <os/memory.h>
#include <os/string.h>
#include <os/stdlib.h>
#include <os/assert.h>
#include <os/os.h>


extern uint32_t free_pages;
static arena_descriptor_t descriptors[DESC_COUNT];

// arena 初始化
void arena_init()
{
    uint32_t block_size = 16;
    for (size_t i = 0; i < DESC_COUNT; i++)
    {
        arena_descriptor_t *desc = &descriptors[i];
        /* 每个 arena 的块大小分别为 16、32、64、128、256、512、1024 */
        desc->block_size = block_size;
        
        /* 每个 arena 有多少块 */
        desc->total_block = (PAGE_SIZE - sizeof(arena_t)) / block_size;
        list_init(&desc->free_list);
        block_size <<= 1; // block *= 2;
    }
}

// 获得 arena 第 idx 块内存指针
static void *get_arena_block(arena_t *arena, uint32_t idx)
{
    assert(arena->desc->total_block > idx);
    void *addr = (void *)(arena + 1);
    uint32_t gap = idx * arena->desc->block_size;
    return addr + gap;
}

static arena_t *get_block_arena(block_t *block)
{
    return (arena_t *)((uint32_t)block & 0xfffff000);
}

void *kmalloc(size_t size)
{
    arena_descriptor_t *desc = NULL;
    arena_t *arena;
    block_t *block;
    char *addr;

    /* 超出了 1024 字节时，直接申请 */
    if (size > 1024)
    {
        uint32_t asize = size + sizeof(arena_t);
        uint32_t count = div_round_up(asize, PAGE_SIZE);

        /* 申请内存 */
        arena = (arena_t *)alloc_kpage(count);
        memset(arena, 0, count * PAGE_SIZE);


        /* 表示超出了 1024 字节*/
        arena->large = true;

        /* 申请的内存的页数 */
        arena->count = count;
        arena->desc = NULL;
        arena->magic = OS_MAGIC;

        addr = (char *)((uint32_t)arena + sizeof(arena_t));
        return addr;
    }

    /* 小于 1024 字节时，从 descriptors 中申请 */
    for (size_t i = 0; i < DESC_COUNT; i++)
    {
        desc = &descriptors[i];
        if (desc->block_size >= size)
            break;
    }

    assert(desc != NULL);

    /* 如果空闲列表为空，申请一页内存，按照block_size拆分为多个块存储到空闲列表中 */
    if (list_empty(&desc->free_list))
    {
        arena = (arena_t *)alloc_kpage(1);
        memset(arena, 0, PAGE_SIZE);

        arena->desc = desc;
        arena->large = false;
        arena->count = desc->total_block;
        arena->magic = OS_MAGIC;

        for (size_t i = 0; i < desc->total_block; i++)
        {
            // 从 arena 中取出第 i 块
            block = get_arena_block(arena, i);

            // 将第 i 块放入空闲列表
            list_push(&desc->free_list, block);
        }
    }

    /* 从空闲列表中取出一块内存 */
    block = list_pop(&desc->free_list);

    arena = get_block_arena(block);
    assert(arena->magic == OS_MAGIC && !arena->large);

    // memset(block, 0, desc->block_size);

    // 块数量减一
    arena->count--;

    return block;
}

void kfree(void *ptr)
{
    assert(ptr);

    block_t *block = (block_t *)ptr;
    arena_t *arena = get_block_arena(block);

    assert(arena->large == 1 || arena->large == 0);
    assert(arena->magic == OS_MAGIC);

    // 超过 1024 字节时，直接释放
    if (arena->large)
    {
        free_kpage((uint32_t)arena, arena->count);
        return;
    }

    // 将块放入空闲列表
    list_push(&arena->desc->free_list, block);
    arena->count++;

    // 如果所有块都在空闲列表中，释放 arena
    if (arena->count == arena->desc->total_block)
    {
        for (size_t i = 0; i < arena->desc->total_block; i++)
        {
            block = get_arena_block(arena, i);
            assert(list_search(&arena->desc->free_list, block));
            list_remove(block);
            assert(!list_search(&arena->desc->free_list, block));
        }
        free_kpage((uint32_t)arena, 1);
    }
}
