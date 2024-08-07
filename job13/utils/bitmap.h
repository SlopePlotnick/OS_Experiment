#ifndef _UTILS_BITMAP_H
#define _UTILS_BITMAP_H

extern int bitmap_alloc_bit(void *bitmap, int byte_count);
extern void bitmap_free_bit(void *bitmap, int index);
extern void bitmap_set_bit(void *bitmap, int index);
extern void bitmap_clear_bit(void *bitmap, int index);
extern int bitmap_test_bit(void *bitmap, int index);
extern void bitmap_dump(void *bitmap, int byte_count);

#endif
