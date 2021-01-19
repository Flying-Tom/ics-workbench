#include "common.h"
#include <inttypes.h>
#define Print(A) printf(#A ":%d\n", A)

void mem_read(uintptr_t block_num, uint8_t *buf);
void mem_write(uintptr_t block_num, const uint8_t *buf);

static uint64_t cycle_cnt = 0;

void cycle_increase(int n) { cycle_cnt += n; }

// TODO: implement the following functions
#define MEM_WIDTH 25

#define BLOCK_IDX(addr) ((addr >> BLOCK_WIDTH) & mask_with_len(MEM_WIDTH - BLOCK_WIDTH))
#define ADDR_TAG(addr) ((BLOCK_IDX(addr) >> GROUP_WIDTH) & mask_with_len(TAG_WIDTH))
#define ADDR_GRPIDX(addr) (BLOCK_IDX(addr) & mask_with_len(GROUP_WIDTH))
#define ADDR_INBLOCK(addr) (addr & mask_with_len(BLOCK_WIDTH))

uint32_t group_size, cache_size, TAG_WIDTH, GROUP_WIDTH;

typedef struct
{
    bool valid_bit;
    bool dirty_bit;
    uint32_t tag;
    uint8_t data[BLOCK_SIZE];
} cacheline;

cacheline *Cache;

uint32_t cache_load(uintptr_t addr)
{
    //puts("cache_load");
    uint32_t group_idx = ADDR_GRPIDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];
    for (int i = 0; i < group_size; i++)
    {
        if (!group_base[i].valid_bit)
        {
            mem_read(BLOCK_IDX(addr), group_base[i].data);
            group_base[i].valid_bit = true;
            group_base[i].dirty_bit = false;
            group_base[i].tag = ADDR_TAG(addr);
            return i;
        }
    }
    //若找不到空余块则随机替换
    uint32_t rand_idx_ingroup = rand() % group_size;

    if (group_base[rand_idx_ingroup].dirty_bit)
        mem_write((group_base[rand_idx_ingroup].tag << GROUP_WIDTH) | group_idx, group_base[rand_idx_ingroup].data);

    mem_read(BLOCK_IDX(addr), group_base[rand_idx_ingroup].data);
    group_base[rand_idx_ingroup].dirty_bit = false;
    group_base[rand_idx_ingroup].valid_bit = true;
    group_base[rand_idx_ingroup].tag = ADDR_TAG(addr);
    puts("replace completed!");
    return rand_idx_ingroup;
}

uint32_t cache_read(uintptr_t addr)
{
    puts("cache_read");
    uint32_t group_idx = ADDR_GRPIDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];

    for (int i = 0; i < group_size; i++)
    {
        if (group_base[i].tag == ADDR_TAG(addr) && group_base[i].valid_bit)
            return *(uint32_t *)(group_base[i].data + ADDR_INBLOCK(addr));
    }
    //找不到则加载块
    return *(uint32_t *)(group_base[cache_load(addr)].data + ADDR_INBLOCK(addr));
}

void cache_write(uintptr_t addr, uint32_t data, uint32_t wmask)
{
    puts("cache_write");
    uint32_t group_idx = ADDR_GRPIDX(addr);
    cacheline *group_base = &Cache[group_size * group_idx];
    for (int i = 0; i < group_size; i++)
    {
        if (group_base[i].tag == ADDR_TAG(addr) && group_base[i].valid_bit)
        {
            uint32_t *addr_temp = (uint32_t *)group_base[i].data[ADDR_INBLOCK(addr)];
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
    cache_size = exp2(total_size_width - BLOCK_WIDTH);                  //cache的行数
    group_size = exp2(associativity_width);                             //每组的行数
    GROUP_WIDTH = total_size_width - BLOCK_WIDTH - associativity_width; //每组的位数
    TAG_WIDTH = MEM_WIDTH - GROUP_WIDTH - BLOCK_WIDTH;                  //标记的位数
    Print(cache_size);
    Print(group_size);
    Print(GROUP_WIDTH);
    Print(TAG_WIDTH);
    Cache = (cacheline *)malloc(sizeof(cacheline) * cache_size);
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
