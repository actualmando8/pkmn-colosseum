#include "dolphin/types.h"

/* fn_800C7558 - 0x800C7558 | size: 0x24 */
s32 fn_800C7558(s32 ch) {
    extern u8 lbl_80313C18[];

    if (ch == -1) {
        return -1;
    }
    return lbl_80313C18[(u8)ch];
}

enum file_kind {
    FILE_KIND_CLOSED,
    FILE_KIND_DISK,
    FILE_KIND_CONSOLE,
    FILE_KIND_UNAVAILABLE
};

enum io_state {
    IO_STATE_NEUTRAL,
    IO_STATE_WRITING,
    IO_STATE_READING,
    IO_STATE_REREADING
};

typedef struct file_modes {
    u32 open_mode : 2;
    u32 io_mode : 3;
    u32 buffer_mode : 2;
    u32 file_kind : 3;
    u32 file_orientation : 2;
    u32 binary_io : 1;
} file_modes;

typedef struct file_states {
    u32 io_state : 3;
    u32 free_buffer : 1;
    u8 eof;
    u8 error;
} file_states;

typedef struct _MSL_FILE {
    /* 0x00 */ u32 handle;
    /* 0x04 */ file_modes file_mode;
    /* 0x08 */ file_states file_state;
    /* 0x0C */ u8 is_dynamically_allocated;
    /* 0x0D */ char char_buffer;
    /* 0x0E */ char char_buffer_overflow;
    /* 0x0F */ char ungetc_buffer[2];
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

u32 fwrite(const void* ptr, u32 size, u32 count, MSL_FILE* stream) {
    extern void __begin_critical_region(s32 region);
    extern void __end_critical_region(s32 region);
    extern u32 __fwrite(const void* ptr, u32 size, u32 count, MSL_FILE* stream);
    u32 result;

    __begin_critical_region(2);
    result = __fwrite(ptr, size, count, stream);
    __end_critical_region(2);
    return result;
}

s32 fseek(MSL_FILE* stream, s32 offset, s32 origin) {
    extern void __begin_critical_region(s32 region);
    extern void __end_critical_region(s32 region);
    extern s32 _fseek(MSL_FILE* stream, s32 offset, s32 origin);
    s32 result;

    __begin_critical_region(2);
    result = _fseek(stream, offset, origin);
    __end_critical_region(2);
    return result;
}

inline s32 _ftell(MSL_FILE* file) {
    extern s32 lbl_8047AA10;
    s32 chars_in_undo_buffer = 0;
    s32 position;
    u8 kind = file->file_mode.file_kind;

    if (!((kind == FILE_KIND_DISK) || (kind == FILE_KIND_CONSOLE)) || file->file_state.error) {
        lbl_8047AA10 = 0x28;
        return -1;
    }

    if (file->file_state.io_state == IO_STATE_NEUTRAL) {
        return file->position;
    }

    position = file->buffer_position + (file->buffer_ptr - file->buffer);

    if (file->file_state.io_state >= IO_STATE_REREADING) {
        chars_in_undo_buffer = file->file_state.io_state - IO_STATE_REREADING + 1;
        position -= chars_in_undo_buffer;
    }

    if (!file->file_mode.binary_io) {
        s32 count = file->buffer_ptr - file->buffer - chars_in_undo_buffer;
        u8* ptr = file->buffer;

        while (count--) {
            if (*ptr++ == '\n') {
                position++;
            }
        }
    }

    return position;
}

s32 ftell(MSL_FILE* stream) {
    extern void __begin_critical_region(s32 region);
    extern void __end_critical_region(s32 region);
    s32 result;

    __begin_critical_region(2);
    result = _ftell(stream);
    __end_critical_region(2);
    return result;
}
