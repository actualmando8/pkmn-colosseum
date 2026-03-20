#include "dolphin/types.h"

/*
 * stdio.c - MetroWerks CRT stdio support.
 *
 * Provides the minimal stdio functions needed by the TRK debugger
 * and CRT. Most of these are stubs or simplified implementations
 * since GameCube has no filesystem or console I/O in the
 * traditional sense.
 */

/* File structure - simplified for GCN */
typedef struct __FILE {
    s32 handle;         /* 0x00 */
    s32 mode;           /* 0x04 */
    s32 state;          /* 0x08 */
    u8  isEOF;          /* 0x0C */
    u8  isErr;          /* 0x0D */
    u8  padding[2];     /* 0x0E */
    u32 position;       /* 0x10 */
    u8* buffer;         /* 0x14 */
    u32 bufferSize;     /* 0x18 */
    u32 bufferPos;      /* 0x1C */
    u32 bufferLen;      /* 0x20 */
    s32 ungetChar;      /* 0x24 */
    s32 wideOrient;     /* 0x28 */
    struct __FILE* next; /* 0x2C */
} __FILE;

typedef void (*FuncPtr)(void);

/* SDA-relative globals */
extern FuncPtr __stdio_exit;
extern void __close_all(void);

/* File list head for open files */
extern __FILE* __file_list;  /* not actually used on GCN but needed for linking */

/*
 * __begin_critical_region - Enter a critical section.
 * No-op on single-threaded GameCube TRK context.
 */
void __begin_critical_region(s32 region) {
    /* empty */
}

/*
 * __end_critical_region - Leave a critical section.
 * No-op on single-threaded GameCube TRK context.
 */
void __end_critical_region(s32 region) {
    /* empty */
}

/*
 * __kill_critical_regions - Destroy all critical sections.
 * No-op on single-threaded GameCube TRK context.
 */
void __kill_critical_regions(void) {
    /* empty */
}

/*
 * __stdio_atexit - Register the stdio cleanup handler.
 * Sets __stdio_exit to point to __close_all so it gets called
 * during program shutdown.
 */
void __stdio_atexit(void) {
    __stdio_exit = (FuncPtr)__close_all;
}

/*
 * __close_all - Close all open file streams.
 *
 * Iterates the linked list of open __FILE structures and
 * flushes/closes each one. On GameCube this primarily handles
 * the stdout/stderr buffers if they were used.
 *
 * NOTE: Full implementation requires matching the complex loop
 * structure from the original binary. This is a simplified version.
 */
void __close_all(void) {
    /* The original iterates __file_list and calls fclose on each.
     * On GCN with TRK, this is effectively empty since no real
     * files are opened. The asm version at 0x800C5458 is 0xA8 bytes. */
}

/*
 * __flush_buffer - Flush a file's output buffer to its destination.
 *
 * Writes buffered data to the underlying file handle via the
 * file's write function. Used by fwrite and fclose.
 *
 * NOTE: Full implementation at 0x800C7454, size 0xC4.
 */
s32 __flush_buffer(__FILE* file, s32* written) {
    /* Stub - needs full asm match for 0xC4 bytes */
    return 0;
}

/*
 * __prep_buffer - Prepare a file's buffer for I/O.
 *
 * Sets up buffer pointers and initializes the buffer state
 * for subsequent read/write operations.
 *
 * Size: 0x34 bytes at 0x800C7518.
 */
void __prep_buffer(__FILE* file) {
    /* Stub - needs full asm match for 0x34 bytes */
}

/*
 * __fwrite - Internal implementation of fwrite.
 *
 * Handles the buffered write logic: fills the current buffer,
 * flushes when full, and handles direct writes for large requests.
 *
 * Size: 0x30C bytes at 0x800C757C.
 */
u32 __fwrite(const void* ptr, u32 size, u32 count, __FILE* file) {
    /* Stub - needs full asm match for 0x30C bytes */
    return 0;
}

/*
 * fwrite - Standard C library fwrite.
 *
 * Writes count elements of size bytes each from ptr to file.
 * Returns the number of elements successfully written.
 *
 * Size: 0x7C bytes at 0x800C7888.
 */
u32 fwrite(const void* ptr, u32 size, u32 count, __FILE* file) {
    u32 total;
    u32 written;

    if (size == 0 || count == 0) {
        return 0;
    }

    total = size * count;
    written = __fwrite(ptr, 1, total, file);

    if (written != 0) {
        return written / size;
    }

    return 0;
}

/*
 * fseek - Seek to a position in a file stream.
 *
 * Size: 0x6C bytes at 0x800C7BF8.
 * On GameCube, this is primarily used by the CRT for
 * stream positioning.
 */
s32 fseek(__FILE* file, s32 offset, s32 whence) {
    /* Stub - needs full asm match for 0x6C bytes */
    return 0;
}

/*
 * fwide - Set or query the orientation of a file stream.
 *
 * If mode > 0, try to set wide orientation.
 * If mode < 0, try to set byte orientation.
 * If mode == 0, just query current orientation.
 *
 * Returns current orientation (positive=wide, negative=byte, 0=unset).
 *
 * Size: 0x88 bytes at 0x800CAA58.
 */
s32 fwide(__FILE* file, s32 mode) {
    s32 orient;

    if (file == NULL) {
        return 0;
    }

    orient = file->wideOrient;

    if (orient == 0) {
        if (mode > 0) {
            file->wideOrient = 1;
        } else if (mode < 0) {
            file->wideOrient = -1;
        }
    }

    return file->wideOrient;
}
