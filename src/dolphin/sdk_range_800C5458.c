/**
 * @file sdk_range_800C5458.c
 * @brief dolphin-sdk code, 0x800C5458 - 0x800C754C (7 fns).
 *
 * Range unit assigned from the propagated subsystem map
 * (tools/subsystem_propagation.py, >=80% single-label dominance;
 * campaign 2026-07-01). All functions asm-only until matched; the
 * range name stays honest until internal TU structure is proven.
 */
#include "dolphin/types.h"

typedef struct _file_modes {
    u32 open_mode : 2;
    u32 io_mode : 3;
    u32 buffer_mode : 2;
    u32 file_kind : 3;
    u32 file_orientation : 2;
    u32 binary_io : 1;
} file_modes;

typedef void (*IdleProc)(void);
typedef s32 (*WriteProc)(u32 handle, u8* buffer, u32* length, IdleProc idle_fn);

typedef struct _MSL_FILE {
    /* 0x00 */ u32 handle;
    /* 0x04 */ file_modes file_mode;
    /* 0x08 */ u32 file_state;
    /* 0x0C */ u8 is_dynamically_allocated;
    /* 0x0D */ char pad0d;
    /* 0x0E */ char pad0e;
    /* 0x0F */ char pad0f;
    /* 0x10 */ char ungetc_buffer[2];
    /* 0x12 */ u16 ungetc_wide_buffer[2];
    /* 0x18 */ u32 position;
    /* 0x1C */ u8* buffer;
    /* 0x20 */ u32 buffer_size;
    /* 0x24 */ u8* buffer_ptr;
    /* 0x28 */ u32 buffer_length;
    /* 0x2C */ u32 buffer_alignment;
    /* 0x30 */ u32 save_buffer_length;
    /* 0x34 */ u32 buffer_position;
    /* 0x38 */ void* position_fn;
    /* 0x3C */ void* read_fn;
    /* 0x40 */ WriteProc write_fn;
    /* 0x44 */ void* close_fn;
    /* 0x48 */ IdleProc idle_fn;
    /* 0x4C */ struct _MSL_FILE* next_file;
} MSL_FILE;

void __close_all(void) {
    extern MSL_FILE __files;
    extern void __begin_critical_region(s32 region);
    extern void __end_critical_region(s32 region);
    extern s32 fclose(MSL_FILE* stream);
    extern void free(void* ptr);

    MSL_FILE* stream = &__files;

    __begin_critical_region(2);
    while (stream != NULL) {
        MSL_FILE* current;

        if (stream->file_mode.file_kind != 0) {
            fclose(stream);
        }

        current = stream;
        stream = stream->next_file;
        if (current->is_dynamically_allocated) {
            free(current);
        } else {
            current->file_mode.file_kind = 3;
            if (stream != NULL && stream->is_dynamically_allocated) {
                current->next_file = NULL;
            }
        }
    }
    __end_critical_region(2);
}

#pragma peephole off
s32 __flush_buffer(MSL_FILE* stream, u32* bytes_flushed) {
    u32 buffer_size;
    s32 result;

    buffer_size = stream->buffer_ptr - stream->buffer;
    if (buffer_size != 0) {
        stream->buffer_length = buffer_size;
        result = (*stream->write_fn)(stream->handle, stream->buffer, &stream->buffer_length, stream->idle_fn);
        if (bytes_flushed != NULL) {
            *bytes_flushed = stream->buffer_length;
        }
        if (result != 0) {
            return result;
        }
        stream->position += stream->buffer_length;
    }

    stream->buffer_ptr = stream->buffer;
    stream->buffer_length = stream->buffer_size;
    stream->buffer_length -= stream->position & stream->buffer_alignment;
    stream->buffer_position = stream->position;
    return 0;
}
#pragma peephole reset

void __prep_buffer(MSL_FILE* stream) {
    stream->buffer_ptr = stream->buffer;
    stream->buffer_length = stream->buffer_size;
    stream->buffer_length -= stream->position & stream->buffer_alignment;
    stream->buffer_position = stream->position;
}
