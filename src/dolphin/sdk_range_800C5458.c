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

typedef struct _MSL_FILE {
    /* 0x00 */ u32 handle;
    /* 0x04 */ u8 file_mode_open : 2;
    /* 0x04 */ u8 file_mode_io : 3;
    /* 0x04 */ u8 file_mode_buffer : 2;
    /* 0x04 */ u8 file_kind : 3;
    /* 0x04 */ u8 file_orientation : 2;
    /* 0x04 */ u8 binary_io : 1;
    /* 0x04 */ u8 file_mode_unused;
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
    /* 0x40 */ void* write_fn;
    /* 0x44 */ void* close_fn;
    /* 0x48 */ void* idle_fn;
    /* 0x4C */ struct _MSL_FILE* next_file;
} MSL_FILE;

void __prep_buffer(MSL_FILE* stream) {
    stream->buffer_ptr = stream->buffer;
    stream->buffer_length = stream->buffer_size;
    stream->buffer_length -= stream->position & stream->buffer_alignment;
    stream->buffer_position = stream->position;
}
