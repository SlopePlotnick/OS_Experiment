#include "std.h"
#include "bitmap.h"

/* return -1 if failed. */
int bitmap_alloc_bit(void *bitmap, int byte_count)
{
    unsigned int *uint = bitmap;
    assert(byte_count % 4 == 0);

    int uint_count = byte_count / 4;
    for (int i = 0; i < uint_count; i++, uint++) {
        if (*uint == 0xFFFFFFFF)
            continue;
        for (int j = 0; j < 32; j++) {
            int mask = 1 << j;
            if ((*uint & mask) == 0) {
                *uint |= mask;
                return i * 32 + j;
            }
        }
    }
    return -1;
}

void bitmap_free_bit(void *bitmap, int index)
{
    assert(bitmap_test_bit(bitmap, index));
    bitmap_clear_bit(bitmap, index);
}

void bitmap_set_bit(void *bitmap, int index)
{
    char *byte = bitmap + (index >> 3);
    int mask = 1 << (index & 7);
    *byte |= mask;
}

void bitmap_clear_bit(void *bitmap, int index)
{
    char *byte = bitmap + (index >> 3);
    int mask = 1 << (index & 7);
    *byte &= ~mask;
}

int bitmap_test_bit(void *bitmap, int index)
{
    char *byte = bitmap + (index >> 3);
    int mask = 1 << (index & 7);
    return *byte & mask;
}

// 0123 4567 - 0123 4567 - 0123 4567 - 0123 4567
static void dump_byte(char byte)
{
    for (int i = 0; i < 8; i++) {
        if (i == 4)
            putchar(' ');
        if ((byte >> i) & 1)
            putchar('1');
        else
            putchar('0');
    }
}

void bitmap_dump(void *bitmap, int byte_count)
{
    char *byte_array = (char *) bitmap;
    for (int i = 0; i < byte_count; i++) {
        dump_byte(byte_array[i]);
        if (i != byte_count - 1) {
            if ((i + 1) % 4)
                printf(" - ");
            else
                printf("\n");
        }
    }
    printf("\n");
}
