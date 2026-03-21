#include "dolphin/types.h"

/*
 * printf.c - MetroWerks CRT printf implementation.
 *
 * Provides vprintf and its support functions for formatted output.
 * The core formatter (__pformatter) is a large function that handles
 * all printf format specifiers. Helper functions convert numeric
 * values to strings.
 *
 * On GameCube, printf output goes through __FileWrite which calls
 * fwrite on stdout (__files + 0x50).
 */

/* va_list for PPC */
typedef struct __va_list_struct {
    u8  gpr;            /* 0x00: next GPR index (3-10) */
    u8  fpr;            /* 0x01: next FPR index (1-8) */
    u16 padding;        /* 0x02 */
    u32* overflow_arg_area; /* 0x04: pointer to stack args */
    u32* reg_save_area;    /* 0x08: pointer to saved regs */
} __va_list_struct;

typedef __va_list_struct va_list[1];

/* File structure forward decl */
typedef struct __FILE __FILE;

/* __files array - the standard streams (stdin, stdout, stderr) */
extern u8 __files[];

/* Function types for the formatter */
typedef s32 (*WriteFunc)(void* data, s32 count, __FILE* file);

extern void __begin_critical_region(s32 region);
extern void __end_critical_region(s32 region);
extern s32  fwide(__FILE* file, s32 mode);
extern u32  fwrite(const void* ptr, u32 size, u32 count, __FILE* file);

/* Forward declarations for internal format helpers */
static s32 __pformatter(WriteFunc writefunc, __FILE* file,
                        const char* fmt, va_list args);

/* Forward declaration for __FileWrite (defined below vprintf) */
s32 __FileWrite(void* data, s32 count, __FILE* file);

/*
 * vprintf - Formatted print to stdout using a va_list.
 *
 * Gets the stdout stream from __files (offset 0x50), checks the
 * wide orientation, then calls __pformatter with __FileWrite.
 * Returns the number of characters written, or -1 on error.
 */
s32 vprintf(const char* fmt, va_list args) {
    __FILE* stdout_file = (__FILE*)(__files + 0x50);
    s32 result;

    /* Check that stream is byte-oriented */
    if (fwide(stdout_file, -1) >= 0) {
        return -1;
    }

    __begin_critical_region(2);
    result = __pformatter((WriteFunc)__FileWrite, stdout_file, fmt, args);
    __end_critical_region(2);

    return result;
}

/*
 * __FileWrite - Write callback for printf.
 *
 * Calls fwrite to write the formatted output data.
 * Returns the original data pointer on success, or 0 on failure.
 */
s32 __FileWrite(void* data, s32 count, __FILE* file) {
    s32 written;
    s32 requested = count;

    written = (s32)fwrite(data, 1, (u32)requested, file);

    if ((u32)written == (u32)count) {
        return requested;
    }

    return 0;
}

/*
 * __pformatter - Core printf format string processor.
 *
 * Parses the format string and calls the write function for each
 * formatted output segment. Handles all standard printf specifiers:
 * %d, %i, %u, %x, %X, %o, %s, %c, %p, %f, %e, %g, %n, %%, etc.
 *
 * This is a very large function (0x774 bytes in the original binary).
 * A full matching implementation requires careful register allocation
 * to match the MetroWerks compiler output.
 *
 * NOTE: This is a simplified stub. The full implementation at
 * 0x800C88BC (size 0x774) needs asm-level matching.
 */
static s32 __pformatter(WriteFunc writefunc, __FILE* file,
                        const char* fmt, va_list args) {
    /* Stub - the full implementation is 0x774 bytes of complex
     * format parsing and number conversion logic. It calls:
     *   - long2str for integer formatting
     *   - longlong2str for 64-bit integer formatting
     *   - float2str for floating point
     *   - double2hex for %a/%A
     *   - parse_format for format specifier parsing
     */
    return 0;
}

/*
 * parse_format - Parse a printf format specifier.
 *
 * Reads flags, width, precision, and length modifiers from the
 * format string starting after '%'.
 *
 * Size: 0x504 bytes at 0x800CA11C.
 * NOTE: Stub - needs full asm match.
 */
static void parse_format(void) {
    /* Stub */
}

/*
 * long2str - Convert a 32-bit integer to string.
 *
 * Handles decimal, octal, and hexadecimal conversion with
 * proper sign handling and zero-padding.
 *
 * Size: 0x258 bytes at 0x800C9EC4.
 * NOTE: Stub - needs full asm match.
 */
static void long2str(void) {
    /* Stub */
}

/*
 * longlong2str - Convert a 64-bit integer to string.
 *
 * Similar to long2str but handles 64-bit values using
 * the PPC's 32-bit arithmetic operations.
 *
 * Size: 0x314 bytes at 0x800C9BB0.
 * NOTE: Stub - needs full asm match.
 */
static void longlong2str(void) {
    /* Stub */
}

/*
 * float2str - Convert a floating-point value to string.
 *
 * Handles %f, %e, %g formatting with proper rounding,
 * precision, and special values (inf, nan).
 *
 * Size: 0x71C bytes at 0x800C9030.
 * NOTE: Stub - needs full asm match.
 */
static void float2str(void) {
    /* Stub */
}

/*
 * double2hex - Convert a double to hexadecimal string (%a/%A).
 *
 * Size: 0x338 bytes at 0x800C9878.
 * NOTE: Stub - needs full asm match.
 */
static void double2hex(void) {
    /* Stub */
}

/* ========================================================== */
/* Stub functions for coverage - TODO: decompile              */
/* ========================================================== */

/* fn_800C8520 - 0x800C8520 | size: 0xE0 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C8520(void) {
    extern void fn_800C87F8();
    u8 sp[0xA0];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r12 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;

    /* stmw r25, 0x84(r1) */;
    r25 = r3;
    r26 = r4;
    if ((s32)r0 != (s32)0) goto L_800C855C;
    *(f64*)(sp + 0x28) = f1;
    *(f64*)(sp + 0x30) = f2;
    *(f64*)(sp + 0x38) = f3;
    *(f64*)(sp + 0x40) = f4;
    *(f64*)(sp + 0x48) = f5;
    *(f64*)(sp + 0x50) = f6;
    *(f64*)(sp + 0x58) = f7;
    *(f64*)(sp + 0x60) = f8;
L_800C855C: ;
    r29 = r1 + 0xa8;
    r30 = r1 + 0x8;
    r28 = (0x200 << 16);
    r31 = -0x1;
    r12 = 0x0;
    r11 = (u32)fn_800C87F8;
    r27 = r1 + 0x74;
    r0 = (u32)fn_800C87F8;
    r4 = r1 + 0x68;
    r3 = r0;
    r5 = r26;
    r6 = r27;
    ((void(*)(void))__pformatter)();
    if ((u32)r25 == (u32)0x0) goto L_800C85EC;
    r0 = -0x1;
    r4 = -0x2;
    if ((u32)r3 >= (u32)r0) goto L_800C85E4;
    r4 = r3;
L_800C85E4: ;
    r0 = 0x0;
    *(u8*)(r25 + r4) = r0;
L_800C85EC: ;
    /* lmw r25, 0x84(r1) */;
    return;
}
#pragma pop

/* fn_800C8600 - 0x800C8600 | size: 0x78 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C8600(void) {
    extern void fn_800C87F8();
    u8 sp[0x20];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r31 = 0;

    r6 = r5;
    r5 = r4;
    r7 = -0x1;
    r0 = 0x0;
    r4 = r1 + 0x8;
    r31 = r3;
    r3 = (u32)fn_800C87F8;
    r3 = (u32)fn_800C87F8;
    *(u32*)(sp + 0x10) = r0;
    ((void(*)(void))__pformatter)();
    if ((u32)r31 == (u32)0x0) goto L_800C8664;
    r0 = -0x1;
    r4 = -0x2;
    if ((u32)r3 >= (u32)r0) goto L_800C865C;
    r4 = r3;
L_800C865C: ;
    r0 = 0x0;
    *(u8*)(r31 + r4) = r0;
L_800C8664: ;
    r31 = *(u32*)(sp + 0x1C);
    return;
}
#pragma pop

/* fn_800C8710 - 0x800C8710 | size: 0xE8 */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C8710(void) {
    u8 sp[0x80];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;
    u32 r9 = 0;
    u32 r10 = 0;
    u32 r11 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    f32 f1 = 0.0f;
    f32 f2 = 0.0f;
    f32 f3 = 0.0f;
    f32 f4 = 0.0f;
    f32 f5 = 0.0f;
    f32 f6 = 0.0f;
    f32 f7 = 0.0f;
    f32 f8 = 0.0f;

    r30 = r3;
    if ((s32)r0 != (s32)0) goto L_800C874C;
    *(f64*)(sp + 0x28) = f1;
    *(f64*)(sp + 0x30) = f2;
    *(f64*)(sp + 0x38) = f3;
    *(f64*)(sp + 0x40) = f4;
    *(f64*)(sp + 0x48) = f5;
    *(f64*)(sp + 0x50) = f6;
    *(f64*)(sp + 0x58) = f7;
    *(f64*)(sp + 0x60) = f8;
L_800C874C: ;
    r11 = (u32)__files;
    r11 = (u32)__files;
    r4 = -0x1;
    r31 = r11 + 0x50;
    r3 = r31;
    ((void(*)(void))fwide)();
    if ((s32)r3 < (s32)0x0) goto L_800C8794;
    r3 = -0x1;
    goto L_800C87E0;
L_800C8794: ;
    r3 = 0x2;
    ((void(*)(void))__begin_critical_region)();
    r5 = r1 + 0x88;
    r0 = r1 + 0x8;
    r4 = (0x100 << 16);
    r3 = (u32)__FileWrite;
    r6 = r1 + 0x68;
    r3 = (u32)__FileWrite;
    r4 = r31;
    r5 = r30;
    *(u32*)(sp + 0x70) = r0;
    ((void(*)(void))__pformatter)();
    r0 = r3;
    r3 = 0x2;
    r31 = r0;
    ((void(*)(void))__end_critical_region)();
    r3 = r31;
L_800C87E0: ;
    r31 = *(u32*)(sp + 0x7C);
    r30 = *(u32*)(sp + 0x78);
    return;
}
#pragma pop

/* fn_800C87F8 - 0x800C87F8 | size: 0x6C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C87F8(void) {
    u8 sp[0x10];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r30 = 0;
    u32 r31 = 0;

    r30 = r3;
    r3 = *(u32*)((u8*)r3 + 0x8);
    r6 = *(u32*)((u8*)r30 + 0x4);
    r0 = r3 + r5;
    r31 = r6 - r3;
    if ((u32)r0 > (u32)r6) goto L_800C882C;
    r31 = r5;
L_800C882C: ;
    r0 = *(u32*)((u8*)r30 + 0x0);
    r5 = r31;
    r3 = r0 + r3;
    memcpy((void*)r3, (const void*)r4, (u32)r5);
    r0 = *(u32*)((u8*)r30 + 0x8);
    r3 = 0x1;
    r0 = r0 + r31;
    *(u32*)((u8*)r30 + 0x8) = r0;
    r31 = *(u32*)(sp + 0xC);
    r30 = *(u32*)(sp + 0x8);
    return;
}
#pragma pop

/* fn_800C974C - 0x800C974C | size: 0x12C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800C974C(void) {
    u32 r0 = 0;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r7 = 0;
    u32 r8 = 0;

    if ((s32)r4 >= (s32)0x0) goto L_800C9770;
L_800C9754: ;
    r0 = 0x0;
    r4 = 0x1;
    *(u16*)((u8*)r3 + 0x2) = r0;
    r0 = 0x30;
    *(u8*)((u8*)r3 + 0x4) = r4;
    *(u8*)((u8*)r3 + 0x5) = r0;
    return;
L_800C9770: ;
    r7 = *(u8*)((u8*)r3 + 0x4);
    if ((s32)r4 >= (s32)r7) return;
    r6 = r3 + r4;
    r5 = *(u8*)((u8*)r6 + 0x5);
    r8 = r6 + 0x5;
    /* subi r0, r5, 0x30 */;
    r6 = (s8)r0;
    if ((s32)r6 != (s32)0x5) goto L_800C97D4;
    r5 = r3 + r7;
    r5 = r5 + 0x5;
L_800C97A0: ;
    /* subi r5, r5, 0x1 */;
    if ((u32)r5 <= (u32)r8) goto L_800C97B8;
    r0 = *(u8*)((u8*)r5 + 0x0);
    if ((s32)r0 == (s32)0x30) goto L_800C97A0;
L_800C97B8: ;
    if ((u32)r5 != (u32)r8) goto L_800C97CC;
    r0 = *(u8*)((u8*)r8 + (-1));
    r5 = r0 & 0x1;
    goto L_800C9834;
L_800C97CC: ;
    r5 = 0x1;
    goto L_800C9834;
L_800C97D4: ;
    r0 = 0x5;
    r0 = r6 ^ r0;
    r5 = (s32)r0 >> 1;
    r0 = r0 & r6;
    r0 = r5 - r0;
    r5 = (u32)r0 >> 31;
    goto L_800C9834;
L_800C97F0: ;
    r0 = *(u8*)((u8*)r8 + (-1));
    r5 = r0 + r5;
    /* subi r0, r5, 0x30 */;
    r7 = (s8)r0;
    r0 = r7 ^ r6;
    r5 = (s32)r0 >> 1;
    r0 = r0 & r7;
    r0 = r5 - r0;
    /* srwi. r5, r0, 31 */;
    if ((u32)r5 != (u32)r8) goto L_800C9820;
    r0 = (s8)r7;
    if ((u32)r5 != (u32)r8) goto L_800C9828;
L_800C9820: ;
    /* subi r4, r4, 0x1 */;
    goto L_800C9838;
L_800C9828: ;
    r0 = r7 + 0x30;
    *(u8*)((u8*)r8 + 0x0) = r0;
    goto L_800C9840;
L_800C9834: ;
    r6 = 0x9;
L_800C9838: ;
    if ((s32)r4 != (s32)0x0) goto L_800C97F0;
L_800C9840: ;
    if ((s32)r5 == (s32)0x0) goto L_800C9868;
    r5 = *(s16*)((u8*)r3 + 0x2);
    r4 = 0x1;
    r0 = 0x31;
    r5 = r5 + 0x1;
    *(u16*)((u8*)r3 + 0x2) = r5;
    *(u8*)((u8*)r3 + 0x4) = r4;
    *(u8*)((u8*)r3 + 0x5) = r0;
    return;
L_800C9868: ;
    if ((s32)r4 == (s32)0x0) goto L_800C9754;
    *(u8*)((u8*)r3 + 0x4) = r4;
    return;
}
#pragma pop

/* fn_800CA620 - 0x800CA620 | size: 0x16C */
#pragma push
#pragma optimization_level 0
#pragma optimizewithasm off
void fn_800CA620(void) {
    u8 sp[0x40];
    u32 r0 = 0;
    u32 r1 = (u32)sp;
    u32 r3 = 0;
    u32 r4 = 0;
    u32 r5 = 0;
    u32 r6 = 0;
    u32 r12 = 0;
    u32 r21 = 0;
    u32 r22 = 0;
    u32 r23 = 0;
    u32 r24 = 0;
    u32 r25 = 0;
    u32 r26 = 0;
    u32 r27 = 0;
    u32 r28 = 0;
    u32 r29 = 0;
    u32 r30 = 0;
    u32 r31 = 0;
    void (*ctr_fn)(void) = 0;

    /* stmw r21, 0x14(r1) */;
    r29 = r3;
    r30 = r5;
    r31 = r6;
    if ((u32)r4 < (u32)0x2) goto L_800CA778;
    r3 = (u32)r4 >> 1;
    /* subi r0, r4, 0x1 */;
    r28 = r3 + 0x1;
    r27 = r4;
    /* subi r3, r28, 0x1 */;
    r3 = r30 * r3;
    r0 = r30 * r0;
    r25 = r29 + r3;
    r24 = r29 + r0;
L_800CA668: ;
    if ((u32)r28 <= (u32)0x1) goto L_800CA67C;
    r25 = r25 - r30;
    /* subi r28, r28, 0x1 */;
    goto L_800CA6C0;
L_800CA67C: ;
    /* subi r3, r24, 0x1 */;
    /* subi r4, r25, 0x1 */;
    r5 = r30 + 0x1;
    goto L_800CA6A8;
L_800CA68C: ;
    r6 = *(u8*)((u8*)r4 + 0x1);
    r0 = *(u8*)((u8*)r3 + 0x1);
    r6 = (s8)r6;
    *(u8*)((u8*)r4 + 0x1) = r0;
    r4 = r4 + 0x1;
    *(u8*)((u8*)r3 + 0x1) = r6;
    r3 = r3 + 0x1;
L_800CA6A8: ;
    /* subic. r5, r5, 0x1 */;
    if ((u32)r28 != (u32)0x1) goto L_800CA68C;
    /* subi r27, r27, 0x1 */;
    if ((u32)r27 == (u32)0x1) goto L_800CA778;
    r24 = r24 - r30;
L_800CA6C0: ;
    /* subi r0, r28, 0x1 */;
    r26 = r28;
    r0 = r30 * r0;
    r22 = r29 + r0;
    goto L_800CA768;
L_800CA6D4: ;
    r26 = r26 << 1;
    r23 = r22;
    /* subi r0, r26, 0x1 */;
    r0 = r30 * r0;
    r22 = r29 + r0;
    if ((u32)r26 >= (u32)r27) goto L_800CA718;
    r21 = r22 + r30;
    r12 = r31;
    r3 = r22;
    r4 = r21;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    if ((s32)r3 >= (s32)0x0) goto L_800CA718;
    r22 = r21;
    r26 = r26 + 0x1;
L_800CA718: ;
    r12 = r31;
    r3 = r23;
    r4 = r22;
    ctr_fn = (void(*)(void))r12;
    ctr_fn();
    if ((s32)r3 >= (s32)0x0) goto L_800CA668;
    /* subi r3, r22, 0x1 */;
    /* subi r4, r23, 0x1 */;
    r5 = r30 + 0x1;
    goto L_800CA760;
L_800CA744: ;
    r6 = *(u8*)((u8*)r4 + 0x1);
    r0 = *(u8*)((u8*)r3 + 0x1);
    r6 = (s8)r6;
    *(u8*)((u8*)r4 + 0x1) = r0;
    r4 = r4 + 0x1;
    *(u8*)((u8*)r3 + 0x1) = r6;
    r3 = r3 + 0x1;
L_800CA760: ;
    /* subic. r5, r5, 0x1 */;
    if ((s32)r3 != (s32)0x0) goto L_800CA744;
L_800CA768: ;
    r0 = r26 << 1;
    if ((u32)r0 <= (u32)r27) goto L_800CA6D4;
    goto L_800CA668;
L_800CA778: ;
    /* lmw r21, 0x14(r1) */;
    return;
}
#pragma pop

