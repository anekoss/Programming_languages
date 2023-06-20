#include "mem_debug.h"
#include "mem_internals.h"
#include "mem.h"
#include "util.h"

static void *block_after(struct block_header const *block) { return (void *) (block->contents + block->capacity.bytes); }
static bool blocks_continuous(struct block_header const *fst, struct block_header const *snd) { return (void *) snd == block_after(fst); }
static struct block_header *get_block_header(void *data) { return (struct block_header *) ((uint8_t *) data - offsetof(struct block_header, contents)); }

void test1() {
    debug("Test(1/6): ");
    debug_heap(stderr, HEAP_START);
    struct block_header *first_block = (struct block_header *) heap_init(8192);
    void *block = _malloc(200);
    debug_heap(stderr, HEAP_START);
    if (first_block->capacity.bytes != 200) err("Error! Block has incorrect size\n");
    _free(block);
    debug_heap(stderr, HEAP_START);
    debug("Success!\n");
}

void test2() {
    debug("Test(2/6): ");
    debug_heap(stderr, HEAP_START);
    void *block1 = _malloc(555);
    void *block2 = _malloc(555);
    debug_heap(stderr, HEAP_START);
    if (block1 == NULL || block2 == NULL) err("Error! Block is null\n");
    struct block_header *first_block_data = get_block_header(block1);
    _free(block1);
    struct block_header *second_block_data = get_block_header(block2);
    if (first_block_data->is_free == false || second_block_data->is_free == true) err("Error! Incorrect answer\n");
    _free(block2);
    debug_heap(stderr, HEAP_START);
    debug("Success!\n");
}

void test3() {
    debug("Test(3/6): ");
    debug_heap(stderr, HEAP_START);
    void *block1 = _malloc(682);
    void *block2 = _malloc(682);
    void *block3 = _malloc(682);
    debug_heap(stderr, HEAP_START);
    if (block1 == NULL || block2 == NULL || block3 == NULL) err("Error! Block is null\n");
    _free(block1);
    struct block_header *block1_header = get_block_header(block1);
    _free(block2);
    struct block_header *block2_header = get_block_header(block2);
    struct block_header *block3_header = get_block_header(block3);
    if (block1_header->is_free == false || block2_header->is_free == false || block3_header->is_free == true)
        err("Error! Incorrect answer\n");
    _free(block3);
    debug_heap(stderr, HEAP_START);
    debug("Success!\n");
}

void test4() {
    debug_heap(stderr, HEAP_START);
    debug("Test(4/6): ");
    void *block1 = _malloc(1200);
    void *block2 = _malloc(1200);
    void *block3 = _malloc(1200);
    debug_heap(stderr, HEAP_START);
    if (block1 == NULL || block2 == NULL || block3 == NULL) err("Error! Block is null\n");
    struct block_header *block1_header = get_block_header(block1);
    struct block_header *block2_header = get_block_header(block2);
    if (!blocks_continuous(block1_header, block2_header)) err("Error! Blocks do not located in a row");
    _free(block1);
    _free(block2);
    _free(block3);
    debug_heap(stderr, HEAP_START);
    debug("Success!\n");
}

void test5() {
    debug("Test(5/6): ");
    debug_heap(stderr, HEAP_START);
    void *block1 = _malloc(2003);
    debug_heap(stderr, HEAP_START);
    if (!block1) err("Error! Block is null\n");
    _free(block1);
    debug_heap(stderr, HEAP_START);
    debug("Success!\n");
}

void test6() {
    debug("Test(6/6): ");
    debug_heap(stderr, HEAP_START);
    for (int i = 0; i < 100; ++i) {
        void *block1 = _malloc(i);
        void *block2 = _malloc(i+1);
        void *block3 = _malloc(i+2);
        debug_heap(stderr, HEAP_START);
        if (block1 == NULL || block2 == NULL || block3 == NULL) err("Error! Block is null\n");
        struct block_header *block1_header = get_block_header(block1);
        struct block_header *block2_header = get_block_header(block2);
        if (!blocks_continuous(block1_header, block2_header)) err("Error! Blocks do not located in a row");
        _free(block3);
        _free(block2);
        _free(block1);
        debug_heap(stderr, HEAP_START);
    }
    debug("Success!\n");
}

