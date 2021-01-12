#include "common.h"
#include <inttypes.h>

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions

#define ADDR_TAG(addr) ((addr >> (GROUP_WIDTH + BLOCK_WIDTH)) & mask_with_len(TAG_WIDTH))
#define ADDR_IDX(addr) ((addr >> BLOCK_WIDTH) & mask_with_len(GROUP_WIDTH))
#define ADDR_INBLOCK(addr) (addr & mask_with_len(BLOCK_WIDTH))
uint32_t group_linenum, GROUP_WIDTH, TAG_WIDTH;
uint32_t total_line;

struct cacheline *Cache;

uint32_t cache_load(uintptr_t addr)
{
    return 0;
}

uint32_t cache_read(uintptr_t addr)
{
    uint32_t group_idx = ADDR_IDX(addr);
    cacheline *group_base = &Cache[group_linenum * group_idx];

    for (int i = 0; i < group_linenum; i++)
    {
        if (group_base[i].tag == ADDR_TAG(addr) && group_base[i].valid_bit)
            return *(uint32_t *)group_base[i].data;
    }
    return *(uint32_t *)group_base[cache_load(addr)].data;
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask)
{
}

void init_cache(int total_size_width, int associativity_width)
{
    total_line = exp2(total_size_width) / BLOCK_SIZE;
    GROUP_WIDTH = associativity_width;
    group_linenum = exp2(associativity_width);
    Cache = malloc(sizeof(cacheline) * total_line);
}

void display_statistic(void)
{
}
