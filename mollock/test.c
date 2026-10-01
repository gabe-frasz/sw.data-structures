#include "mollock.h"
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(cond, ...) do { \
    if (!(cond)) { \
        fprintf(stderr, "FAIL: " __VA_ARGS__); \
        fputc('\n', stderr); \
        exit(EXIT_FAILURE); \
    } \
} while (0)

static void fill_and_check(void *p, size_t n, unsigned char value) {
    memset(p, value, n);
    for (size_t i = 0; i < n; i++)
        CHECK(((unsigned char *)p)[i] == value, "pattern write/read failed at byte %zu", i);
}

static void check_pattern(void *p, size_t n, unsigned char value) {
    for (size_t i = 0; i < n; i++)
        CHECK(((unsigned char *)p)[i] == value, "memory corrupted at byte %zu (expected %u, got %u)", i, value, ((unsigned char *)p)[i]);
}

static void test_basic_limits(void) {
    const size_t max = 16378;
    CHECK(mollock(0) == NULL, "mollock(0) must return NULL");
    CHECK(mollock(max + 1) == NULL, "allocation above capacity must return NULL");

    void *p = mollock(max);
    CHECK(p != NULL, "maximum valid allocation must succeed");
    fill_and_check(p, max, 0xA5);
    check_pattern(p, max, 0xA5);
    mfree(p);

    void *q = mollock(max);
    CHECK(q != NULL, "maximum allocation must work again after free");
    fill_and_check(q, max, 0x5A);
    mfree(q);
}

static void test_null_and_single_block(void) {
    void *p = mollock(64);
    CHECK(p != NULL, "single allocation failed");
    fill_and_check(p, 64, 0x11);

    mfree(NULL);
    check_pattern(p, 64, 0x11);

    uintptr_t old_addr = (uintptr_t)p;
    mfree(p);

    void *q = mollock(64);
    CHECK(q != NULL, "allocation after freeing the only block failed");
    CHECK((uintptr_t)q == old_addr, "first free space was not reused");
    mfree(q);
}

static void test_multiple_blocks_and_last_gap(void) {
    void *a = mollock(32);
    void *b = mollock(48);
    void *c = mollock(64);
    CHECK(a && b && c, "multiple sequential allocations failed");

    fill_and_check(a, 32, 0xA1);
    fill_and_check(b, 48, 0xB2);
    fill_and_check(c, 64, 0xC3);

    check_pattern(a, 32, 0xA1);
    check_pattern(b, 48, 0xB2);
    check_pattern(c, 64, 0xC3);

    uintptr_t c_addr = (uintptr_t)c;
    mfree(c);

    void *d = mollock(16);
    CHECK(d != NULL, "allocation in the last freed gap failed");
    CHECK((uintptr_t)d == c_addr, "last freed gap was not reused");

    check_pattern(a, 32, 0xA1);
    check_pattern(b, 48, 0xB2);
    fill_and_check(d, 16, 0xD4);

    mfree(a);
    mfree(b);
    mfree(d);
}

static void test_free_first_and_middle(void) {
    void *a = mollock(64);
    void *b = mollock(128);
    void *c = mollock(64);
    CHECK(a && b && c, "setup allocation failed");

    fill_and_check(a, 64, 0x10);
    fill_and_check(b, 128, 0x20);
    fill_and_check(c, 64, 0x30);

    uintptr_t a_addr = (uintptr_t)a;
    mfree(a);
    check_pattern(b, 128, 0x20);
    check_pattern(c, 64, 0x30);

    void *d = mollock(32);
    CHECK(d != NULL, "allocation before first block failed");
    CHECK((uintptr_t)d == a_addr, "head gap was not reused");
    fill_and_check(d, 32, 0x40);

    check_pattern(b, 128, 0x20);
    check_pattern(c, 64, 0x30);

    mfree(d);
    mfree(b);
    check_pattern(c, 64, 0x30);

    void *e = mollock(64);
    CHECK(e != NULL, "allocation in the middle gap failed");
    fill_and_check(e, 64, 0x50);
    check_pattern(c, 64, 0x30);

    mfree(e);
    mfree(c);
}

static void test_gap_splitting(void) {
    void *a = mollock(100);
    void *b = mollock(200);
    void *c = mollock(100);
    CHECK(a && b && c, "setup allocation failed");

    fill_and_check(a, 100, 0xA0);
    fill_and_check(c, 100, 0xC0);

    uintptr_t b_addr = (uintptr_t)b;
    mfree(b);

    void *d = mollock(50);
    CHECK(d != NULL, "allocation in split gap failed");
    CHECK((uintptr_t)d == b_addr, "split gap did not start at freed block");
    fill_and_check(d, 50, 0xD0);

    void *e = mollock(100);
    CHECK(e != NULL, "remaining part of split gap was not usable");
    fill_and_check(e, 100, 0xE0);

    check_pattern(a, 100, 0xA0);
    check_pattern(c, 100, 0xC0);
    check_pattern(d, 50, 0xD0);
    check_pattern(e, 100, 0xE0);

    mfree(a);
    mfree(d);
    mfree(e);
    mfree(c);
}

static void test_fragmentation_and_coalescing(void) {
    void *a = mollock(4000);
    void *b = mollock(4000);
    void *c = mollock(4000);
    void *d = mollock(100);
    CHECK(a && b && c && d, "fragmentation setup failed");

    fill_and_check(b, 4000, 0xB1);
    fill_and_check(d, 100, 0xD1);

    mfree(a);
    mfree(c);

    CHECK(mollock(5000) == NULL,
          "fragmented memory incorrectly accepted an allocation larger than every gap");

    void *small = mollock(3000);
    CHECK(small != NULL, "allocation that fits a fragmented gap failed");
    fill_and_check(small, 3000, 0x51);

    check_pattern(b, 4000, 0xB1);
    check_pattern(d, 100, 0xD1);

    mfree(b);
    void *large = mollock(5000);
    CHECK(large != NULL,
          "large allocation should succeed after freeing the block that was joining the gap");
    fill_and_check(large, 5000, 0x61);

    check_pattern(small, 3000, 0x51);
    check_pattern(d, 100, 0xD1);

    mfree(small);
    mfree(large);
    mfree(d);
}

static void test_double_free_assignment_behavior(void) {
    void *a = mollock(32);
    void *b = mollock(32);
    CHECK(a && b, "double-free setup failed");
    fill_and_check(b, 32, 0xBB);

    mfree(a);
    mfree(a); // assignment-specific: should have no effect when block is absent

    check_pattern(b, 32, 0xBB);

    void *c = mollock(16);
    CHECK(c != NULL, "allocator broke after duplicate free");
    fill_and_check(c, 16, 0xCC);
    check_pattern(b, 32, 0xBB);

    mfree(b);
    mfree(c);
}

#define MAX_ACTIVE 64
#define STRESS_OPS 2000

typedef struct ActiveBlock {
    void *ptr;
    size_t size;
    unsigned char pattern;
} ActiveBlock;

static uint32_t rng_state = 0x12345678u;
static uint32_t next_rand(void) {
    rng_state = rng_state * 1664525u + 1013904223u;
    return rng_state;
}

static void test_stress(void) {
    ActiveBlock active[MAX_ACTIVE] = {0};
    size_t active_count = 0;

    for (size_t op = 0; op < STRESS_OPS; op++) {
        bool do_alloc = active_count == 0 ||
                        (active_count < MAX_ACTIVE && (next_rand() & 1u));

        if (do_alloc) {
            size_t slot = SIZE_MAX;
            for (size_t i = 0; i < MAX_ACTIVE; i++) {
                if (active[i].ptr == NULL) {
                    slot = i;
                    break;
                }
            }
            CHECK(slot != SIZE_MAX, "stress test lost an inactive slot");

            size_t size = 1 + (next_rand() % 256);
            void *p = mollock(size);
            if (p != NULL) {
                unsigned char pattern = (unsigned char)(slot + 1);
                fill_and_check(p, size, pattern);
                active[slot] = (ActiveBlock){p, size, pattern};
                active_count++;
            }
        } else {
            size_t candidates[MAX_ACTIVE];
            size_t n = 0;
            for (size_t i = 0; i < MAX_ACTIVE; i++) {
                if (active[i].ptr != NULL)
                    candidates[n++] = i;
            }
            size_t slot = candidates[next_rand() % n];
            mfree(active[slot].ptr);
            active[slot] = (ActiveBlock){0};
            active_count--;
        }

        for (size_t i = 0; i < MAX_ACTIVE; i++) {
            if (active[i].ptr != NULL)
                check_pattern(active[i].ptr, active[i].size, active[i].pattern);
        }
    }

    for (size_t i = 0; i < MAX_ACTIVE; i++) {
        if (active[i].ptr != NULL)
            mfree(active[i].ptr);
    }
}

#define RUN_TEST(fn) do { \
    printf("[TEST] %s... ", #fn); \
    fn(); \
    puts("PASS"); \
} while (0)

int main(void) {
    RUN_TEST(test_basic_limits);
    RUN_TEST(test_null_and_single_block);
    RUN_TEST(test_multiple_blocks_and_last_gap);
    RUN_TEST(test_free_first_and_middle);
    RUN_TEST(test_gap_splitting);
    RUN_TEST(test_fragmentation_and_coalescing);
    RUN_TEST(test_double_free_assignment_behavior);
    RUN_TEST(test_stress);
    puts("\nAll allocator tests passed.");
    return EXIT_SUCCESS;
}
