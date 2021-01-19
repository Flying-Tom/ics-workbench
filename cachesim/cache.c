#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

#define ADDR_TAG(addr) ((addr >> (MEM_WIDTH - GROUP_WIDTH - BLOCK_WIDTH)) & mask_with_len(TAG_WIDTH))
#define ADDR_GRPIDX(addr) ((addr >> BLOCK_WIDTH) & mask_with_len(GROUP_WIDTH))
#define ADDR_INBLOCK(addr) (addr & mask_with_len(BLOCK_WIDTH))
#define BLOCK_IDX(addr) ((addr >> BLOCK_WIDTH) & mask_with_len(MEM_WIDTH - BLOCK_WIDTH))

uint32_t group_size, ADDR_WIDTH, TAG_WIDTH, GROUP_WIDTH;
uint32_t cache_size;

typedef struct
{
    bool valid_bit;
    bool dirty_bit;
    uint32_t tag;
    uint8_t data[BLOCK_SIZE];
} cacheline;

cacheline *Cache;

uint32_t cache_replace(uintptr_t addr)
{
    uint32_t group_idx = ADDR_GRPIDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];

    uint32_t rand_idx_ingroup = rand() % group_size;
    printf("rand_idx_ingroup:%d\n", rand_idx_ingroup);

    uint32_t rand_idx_block = group_idx + (group_base[rand_idx_ingroup].tag << GROUP_WIDTH);
    printf("group_base[rand_idx_ingroup].tag:%d\n", group_base[rand_idx_ingroup].tag);
    printf("rand_idx_block:%d\n", rand_idx_block);

    if (group_base[rand_idx_ingroup].dirty_bit)
        mem_write(rand_idx_block, group_base[rand_idx_ingroup].data);

    group_base[rand_idx_block].dirty_bit = false;
    group_base[rand_idx_block].valid_bit = true;
    group_base[rand_idx_block].tag = ADDR_TAG(addr);
    mem_read(BLOCK_IDX(addr), group_base[rand_idx_ingroup].data);
    puts("replace completed!");
    return rand_idx_block;
}

uint32_t cache_load(uintptr_t addr)
{
    puts("cache_load");
    uint32_t group_idx = ADDR_GRPIDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];
    for (int i = 0; i < group_size; i++)
    {
        if (!group_base[i].valid_bit)
        {
            mem_read(BLOCK_IDX(addr), group_base[i].data);
            group_base[i].valid_bit = true;
            group_base[i].tag = ADDR_TAG(addr);
            return i;
        }
    }
    return cache_replace(addr);
}

uint32_t cache_read(uintptr_t addr)
{
    puts("cache_read");
    uint32_t group_idx = ADDR_GRPIDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];

    for (int i = 0; i < group_size; i++)
    {
        //assert(*(uint32_t *)&group_base[i].data[ADDR_INBLOCK(addr)] == *(uint32_t *)(group_base[i].data + ADDR_INBLOCK(addr)));
        if (group_base[i].tag == ADDR_TAG(addr) && group_base[i].valid_bit)
            return *(uint32_t *)&group_base[i].data[ADDR_INBLOCK(addr)];
    }
    uint32_t idx_ingroup = cache_load(addr);
    assert(group_base[idx_ingroup].data + ADDR_INBLOCK(addr) == &group_base[idx_ingroup].data[ADDR_INBLOCK(addr)] + 1);
    return *(uint32_t *)(group_base[idx_ingroup].data + ADDR_INBLOCK(addr));
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask)
{
    puts("cache_write");
    uint32_t group_idx = ADDR_GRPIDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];
    for (int i = 0; i < group_size; i++)
    {
        //assert(*(uint32_t *)&group_base[i].data[ADDR_INBLOCK(addr)] == *(uint32_t *)(group_base[i].data + ADDR_INBLOCK(addr)));
        if (group_base[i].tag == ADDR_TAG(addr) && group_base[i].valid_bit)
        {
            uint32_t *addr_temp = (uint32_t *)&group_base[i].data[ADDR_INBLOCK(addr)];
            *addr_temp = (*addr_temp & ~wmask) | (data & wmask);
            group_base[i].dirty_bit = true;
            return;
        }
    }
    uint32_t idx_ingroup = cache_load(addr);
    uint32_t *addr_temp = (uint32_t *)&group_base[idx_ingroup].data[ADDR_INBLOCK(addr)];
    *addr_temp = (*addr_temp & ~wmask) | (data & wmask);
    group_base[idx_ingroup].dirty_bit = true;
}

void init_cache(int total_size_width, int associativity_width)
{
    ADDR_WIDTH = total_size_width;
    cache_size = exp2(total_size_width - BLOCK_WIDTH);
    group_size = exp2(associativity_width);
    GROUP_WIDTH = associativity_width;
    printf("GROUP_WIDTH:%u\n", GROUP_WIDTH);
    TAG_WIDTH = total_size_width - BLOCK_WIDTH - GROUP_WIDTH;
    printf("TAG_WIDTH:%u\n", TAG_WIDTH);
    Cache = malloc(sizeof(cacheline) * cache_size);
    for (int i = 0; i < cache_size; i++)
    {
        Cache[i].valid_bit = false;
        Cache[i].dirty_bit = false;
    }
    printf("Total number of blocks:%d\n", MEM_SIZE / 64);
}

void display_statistic(void)
{
}
