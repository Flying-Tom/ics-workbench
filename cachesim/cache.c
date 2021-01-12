#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

#define CACHE_TAG(addr) ((addr >> (CACHE_WIDTH - IDX_WIDTH - BLOCK_WIDTH)) & mask_with_len(TAG_WIDTH))
#define CACHE_IDX(addr) ((addr >> BLOCK_WIDTH) & mask_with_len(IDX_WIDTH))
#define CACHE_INBLOCK(addr) (addr & mask_with_len(BLOCK_WIDTH))
#define BLOCK_IDX(addr) ((addr >> BLOCK_WIDTH) & mask_with_len(CACHE_WIDTH - BLOCK_WIDTH))

uint32_t group_size, CACHE_WIDTH, TAG_WIDTH, IDX_WIDTH;
uint32_t cache_size;

typedef struct
{
    bool valid_bit;
    bool dirty_bit;
    uint32_t tag;
    uint8_t data[BLOCK_WIDTH];
} cacheline;

cacheline *Cache;

uint32_t cache_replace(uintptr_t addr)
{
    uint32_t group_idx = CACHE_IDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];
    uint32_t idx_ingroup = rand() * rand() / group_size;

    if (group_base[idx_ingroup].dirty_bit)
        mem_write(BLOCK_IDX(addr), group_base[idx_ingroup].data);

    group_base[idx_ingroup].dirty_bit = false;
    group_base[idx_ingroup].valid_bit = true;
    group_base[idx_ingroup].tag = CACHE_TAG(addr);
    return idx_ingroup;
}

uint32_t cache_load(uintptr_t addr)
{
    uint32_t group_idx = CACHE_IDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];
    for (int i = 0; i < group_size; i++)
    {
        if (!group_base[i].valid_bit)
        {
            mem_read(BLOCK_IDX(addr), group_base[i].data);
            group_base[i].valid_bit = true;
            group_base[i].tag = CACHE_TAG(addr);
            return i;
        }
    }
    return cache_replace(addr);
}

uint32_t cache_read(uintptr_t addr)
{
    uint32_t group_idx = CACHE_IDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];

    for (int i = 0; i < group_size; i++)
    {
        if (group_base[i].tag == CACHE_TAG(addr) && group_base[i].valid_bit)
            return *(uint32_t *)&group_base[i].data[CACHE_INBLOCK(addr)];
    }
    return *(uint32_t *)&group_base[cache_load(addr)].data[CACHE_INBLOCK(addr)];
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask)
{
    uint32_t group_idx = CACHE_IDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];
    uint32_t *addr_temp = malloc(sizeof(uint32_t));
    for (int i = 0; i < group_size; i++)
    {
        if (group_base[i].tag == CACHE_TAG(addr) && group_base[i].valid_bit)
        {
            assert(*(uint32_t *)&group_base[i].data[CACHE_INBLOCK(addr)] == *(uint32_t *)(group_base[i].data + CACHE_INBLOCK(addr)));
            *addr_temp = *(uint32_t *)&group_base[i].data[CACHE_INBLOCK(addr)];
            *addr_temp = (*addr_temp & ~wmask) | (data & wmask);
            group_base[i].dirty_bit = true;
            return;
        }
    }
    uint32_t idx_ingroup = cache_load(addr);
    *addr_temp = *(uint32_t *)&group_base[idx_ingroup].data[CACHE_INBLOCK(addr)];
    *addr_temp = (*addr_temp & ~wmask) | (data & wmask);
    group_base[idx_ingroup].dirty_bit = true;
}

void init_cache(int total_size_width, int associativity_width)
{
    CACHE_WIDTH = total_size_width;
    cache_size = exp2(total_size_width - BLOCK_WIDTH);
    group_size = exp2(associativity_width);
    Cache = malloc(sizeof(cacheline) * cache_size);
    IDX_WIDTH = associativity_width;
    printf("IDX_WIDTH:%u\n", IDX_WIDTH);
    TAG_WIDTH = total_size_width - BLOCK_WIDTH - IDX_WIDTH;
    printf("TAG_WIDTH:%u\n", TAG_WIDTH);
    for (int i = 0; i < cache_size; i++)
    {
        Cache[i].valid_bit = false;
        Cache[i].dirty_bit = false;
    }
}

void display_statistic(void)
{
}
