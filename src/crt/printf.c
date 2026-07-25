#include "dolphin/types.h"

typedef struct __FILE __FILE;

/* va_list for PPC */
typedef struct __va_list_struct {
    u8  gpr;                /* 0x00: next GPR index (3-10) */
    u8  fpr;                /* 0x01: next FPR index (1-8) */
    u16 padding;            /* 0x02 */
    u32* overflow_arg_area; /* 0x04: pointer to stack args */
    u32* reg_save_area;     /* 0x08: pointer to saved regs */
} __va_list_struct;

typedef __va_list_struct va_list[1];

typedef s32 (*WriteFunc)(void* data, s32 count, __FILE* file);

extern u8 __files[];
extern void __begin_critical_region(s32 region);
extern void __end_critical_region(s32 region);
extern s32 fwide(__FILE* file, s32 mode);
typedef void* (*WriteProcT)(void*, const char*, u32);
int __pformatter_800C88BC(WriteProcT WriteProc, void* WriteProcArg,
                          const char* format_str, va_list arg);
void* __FileWrite(void* pFile, const char* pBuffer, u32 char_num);

/* vsprintf - 0x800C8600 | size: 0x78 */
s32 vsprintf(char* buf, const char* fmt, va_list args) {
    extern s32 __StringWrite(void* data, s32 count, __FILE* file);
    struct { char* p; s32 a; s32 b; } sf;
    s32 n;

    sf.p = buf;
    sf.a = -1;
    sf.b = 0;
    n = __pformatter_800C88BC((WriteProcT)__StringWrite, &sf, fmt, args);
    if (buf != NULL) {
        s32 idx = -2;
        if ((u32)n < 0xFFFFFFFFU)
            idx = n;
        buf[idx] = 0;
    }
    return n;
}

/* vprintf - 0x800C8678 | size: 0x98 */
s32 vprintf(const char* fmt, va_list args) {
    __FILE* stdout_file = (__FILE*)(__files + 0x50);
    s32 result;

    if (fwide(stdout_file, -1) >= 0) {
        return -1;
    }

    __begin_critical_region(2);
    result = __pformatter_800C88BC((WriteProcT)__FileWrite, stdout_file, fmt, args);
    __end_critical_region(2);

    return result;
}

/* printf - 0x800C8710 | size: 0xE8 */
s32 printf(const char* fmt, ...) {
    __FILE* stdout_file = (__FILE*)(__files + 0x50);
    va_list args;
    s32 result;

    if (fwide(stdout_file, -1) >= 0) {
        return -1;
    }

    __begin_critical_region(2);
    __builtin_va_info(&args);
    result = __pformatter_800C88BC((WriteProcT)__FileWrite, stdout_file, fmt, args);
    __end_critical_region(2);

    return result;
}

/* ------------------------------------------------------------------ */
/* MSL printf.c body (0x800C8864 onward, printf_candidate_800C8864).   */
/* ------------------------------------------------------------------ */

enum justification_options { left_justification, right_justification, zero_fill };

enum sign_options { only_minus, sign_always, space_holder };

enum argument_options {
    normal_argument,
    char_argument,
    short_argument,
    long_argument,
    long_long_argument,
    long_double_argument,
    wchar_argument
};

typedef struct {
    /* 0x00 */ unsigned char justification_options;
    /* 0x01 */ unsigned char sign_options;
    /* 0x02 */ unsigned char precision_specified;
    /* 0x03 */ unsigned char alternate_form;
    /* 0x04 */ unsigned char argument_options;
    /* 0x05 */ unsigned char conversion_char;
    /* 0x08 */ int field_width;
    /* 0x0C */ int precision;
} print_format;

typedef struct decimal {
    /* 0x00 */ char sign;
    /* 0x01 */ char unk1;
    /* 0x02 */ short exp;
    struct {
        /* 0x04 */ unsigned char length;
        /* 0x05 */ unsigned char text[36];
        /* 0x29 */ unsigned char unk41;
    } sig;
} decimal;

/* __FileWrite - 0x800C8864 | size: 0x58 */
void* __FileWrite(void* pFile, const char* pBuffer, u32 char_num) {
    extern u32 fwrite(const void* ptr, u32 size, u32 count, void* file);

    return (fwrite(pBuffer, 1, char_num, pFile) == char_num ? pFile : 0);
}

extern void* __va_arg(void* ap, u32 kind);
extern char* strchr(const char* s, int c);
extern u32 strlen(const char* s);
extern void* memchr(const void* s, int c, u32 n);
extern s32 wcstombs(char* dst, const u16* src, u32 n);

const char* parse_format_800CA11C(const char* format_string, va_list* arg,
                                  print_format* format);
char* longlong2str_800C9BB0(signed long long num, char* pBuf, print_format fmt);
char* long2str_800C9EC4(signed long num, char* buff, print_format format);
char* float2str(f64 num, char* buff, print_format format);
char* double2hex(f64 num, char* buff, print_format format);

/* __pformatter - 0x800C88BC */
int __pformatter_800C88BC(WriteProcT WriteProc, void* WriteProcArg,
                          const char* format_str, va_list arg) {
    int num_chars, chars_written, field_width;
    const char* format_ptr;
    const char* curr_format;
    print_format format;
    signed long long_num;
    signed long long long_long_num;
    f64 long_double_num;
    char buff[512];
    char* buff_ptr;
    char* string_end;
    char fill_char = ' ';

    format_ptr = format_str;
    chars_written = 0;

    while (*format_ptr) {
        if (!(curr_format = strchr(format_ptr, '%'))) {
            num_chars = strlen(format_ptr);
            chars_written += num_chars;

            if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars)) {
                return -1;
            }
            break;
        }

        num_chars = curr_format - format_ptr;
        chars_written += num_chars;

        if (num_chars && !(*WriteProc)(WriteProcArg, format_ptr, num_chars)) {
            return -1;
        }

        format_ptr = curr_format;
        format_ptr = parse_format_800CA11C(format_ptr, (va_list*)arg, &format);

        switch (format.conversion_char) {
        case 'd':
        case 'i':
            if (format.argument_options == long_argument) {
                long_num = *(signed long*)__va_arg(arg, 1);
            } else if (format.argument_options == long_long_argument) {
                long_long_num = *(signed long long*)__va_arg(arg, 2);
            } else {
                long_num = *(int*)__va_arg(arg, 1);
            }

            if (format.argument_options == short_argument) {
                long_num = (signed short)long_num;
            }
            if (format.argument_options == char_argument) {
                long_num = (signed char)long_num;
            }

            if ((format.argument_options == long_long_argument)) {
                if (!(buff_ptr = longlong2str_800C9BB0(long_long_num, buff + 512,
                                                       format))) {
                    goto conversion_error;
                }
            } else {
                if (!(buff_ptr = long2str_800C9EC4(long_num, buff + 512, format))) {
                    goto conversion_error;
                }
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'o':
        case 'u':
        case 'x':
        case 'X':
            if (format.argument_options == long_argument) {
                long_num = *(unsigned long*)__va_arg(arg, 1);
            } else if (format.argument_options == long_long_argument) {
                long_long_num = *(signed long long*)__va_arg(arg, 2);
            } else {
                long_num = *(unsigned int*)__va_arg(arg, 1);
            }

            if (format.argument_options == short_argument) {
                long_num = (unsigned short)long_num;
            }
            if (format.argument_options == char_argument) {
                long_num = (unsigned char)long_num;
            }

            if ((format.argument_options == long_long_argument)) {
                if (!(buff_ptr = longlong2str_800C9BB0(long_long_num, buff + 512,
                                                       format))) {
                    goto conversion_error;
                }
            } else {
                if (!(buff_ptr = long2str_800C9EC4(long_num, buff + 512, format))) {
                    goto conversion_error;
                }
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'f':
        case 'F':
        case 'e':
        case 'E':
        case 'g':
        case 'G':
            if (format.argument_options == long_double_argument) {
                long_double_num = *(f64*)__va_arg(arg, 3);
            } else {
                long_double_num = *(f64*)__va_arg(arg, 3);
            }

            if (!(buff_ptr = float2str(long_double_num, buff + 512, format))) {
                goto conversion_error;
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 'a':
        case 'A':
            if (format.argument_options == long_double_argument) {
                long_double_num = *(f64*)__va_arg(arg, 3);
            } else {
                long_double_num = *(f64*)__va_arg(arg, 3);
            }

            if (!(buff_ptr = double2hex(long_double_num, buff + 512, format))) {
                goto conversion_error;
            }

            num_chars = buff + 512 - 1 - buff_ptr;
            break;

        case 's':
            if (format.argument_options == wchar_argument) {
                u16* wcs_ptr = *(u16**)__va_arg(arg, 1);

                if (wcs_ptr == NULL) {
                    wcs_ptr = (u16*)L"";
                }

                if ((num_chars = wcstombs(buff, wcs_ptr, sizeof(buff))) < 0) {
                    goto conversion_error;
                }

                buff_ptr = &buff[0];
            } else {
                buff_ptr = *(char**)__va_arg(arg, 1);
            }

            if (buff_ptr == NULL) {
                buff_ptr = "";
            }

            if (format.alternate_form) {
                num_chars = (unsigned char)*buff_ptr++;

                if (format.precision_specified && num_chars > format.precision) {
                    num_chars = format.precision;
                }
            } else if (format.precision_specified) {
                num_chars = format.precision;

                if ((string_end = (char*)memchr((unsigned char*)buff_ptr, 0,
                                                num_chars)) != 0) {
                    num_chars = string_end - buff_ptr;
                }
            } else {
                num_chars = strlen(buff_ptr);
            }
            break;

        case 'n':
            buff_ptr = *(char**)__va_arg(arg, 1);

            switch (format.argument_options) {
            case normal_argument:
                *(int*)buff_ptr = chars_written;
                break;
            case short_argument:
                *(signed short*)buff_ptr = chars_written;
                break;
            case long_argument:
                *(signed long*)buff_ptr = chars_written;
                break;
            case long_long_argument:
                *(signed long long*)buff_ptr = chars_written;
                break;
            }
            continue;

        case 'c':
            buff_ptr = buff;
            *buff_ptr = *(int*)__va_arg(arg, 1);
            num_chars = 1;
            break;

        case '%':
            buff_ptr = buff;
            *buff_ptr = '%';
            num_chars = 1;
            break;

        case 0xFF:
        default:
        conversion_error:
            num_chars = strlen(curr_format);
            chars_written += num_chars;

            if (num_chars && !(*WriteProc)(WriteProcArg, curr_format, num_chars)) {
                return -1;
            }
            return chars_written;
            break;
        }

        field_width = num_chars;

        if (format.justification_options != left_justification) {
            fill_char = (format.justification_options == zero_fill) ? '0' : ' ';

            if (((*buff_ptr == '+') || (*buff_ptr == '-') || (*buff_ptr == ' ')) &&
                (fill_char == '0')) {
                if ((*WriteProc)(WriteProcArg, buff_ptr, 1) == 0) {
                    return -1;
                }
                ++buff_ptr;
                num_chars--;
            }

            while (field_width < format.field_width) {
                if ((*WriteProc)(WriteProcArg, &fill_char, 1) == 0) {
                    return -1;
                }
                ++field_width;
            }
        }

        if (num_chars && !(*WriteProc)(WriteProcArg, buff_ptr, num_chars)) {
            return -1;
        }

        if (format.justification_options == left_justification) {
            while (field_width < format.field_width) {
                char blank = ' ';

                if ((*WriteProc)(WriteProcArg, &blank, 1) == 0) {
                    return -1;
                }
                ++field_width;
            }
        }

        chars_written += field_width;
    }

    return chars_written;
}

/* round_decimal - 0x800C974C */
void round_decimal(decimal* dec, int new_length) {
    char c;
    char* p;
    int carry;

    if (new_length < 0) {
    return_zero:
        dec->exp = 0;
        dec->sig.length = 1;
        *dec->sig.text = '0';
        return;
    }

    if (new_length >= dec->sig.length) {
        return;
    }

    p = (char*)dec->sig.text + new_length + 1;
    c = *--p - '0';

    if (c == 5) {
        char* q = &((char*)dec->sig.text)[dec->sig.length];

        while (--q > p && *q == '0') {
            ;
        }
        carry = (q == p) ? p[-1] & 1 : 1;
    } else {
        carry = (c > 5);
    }

    while (new_length != 0) {
        c = *--p - '0' + carry;

        if ((carry = (c > 9)) != 0 || c == 0) {
            --new_length;
        } else {
            *p = c + '0';
            break;
        }
    }

    if (carry != 0) {
        dec->exp += 1;
        dec->sig.length = 1;
        *dec->sig.text = '1';
        return;
    } else if (new_length == 0) {
        goto return_zero;
    }

    dec->sig.length = new_length;
}

/* longlong2str - 0x800C9BB0 */
char* longlong2str_800C9BB0(signed long long num, char* pBuf, print_format fmt) {
    unsigned long long unsigned_num, base;
    char* p;
    int n, digits;
    int minus = 0;

    unsigned_num = num;
    minus = 0;
    p = pBuf;
    *--p = 0;
    digits = 0;

    if (!num && !fmt.precision &&
        !(fmt.alternate_form && fmt.conversion_char == 'o')) {
        return p;
    }

    switch (fmt.conversion_char) {
    case 'd':
    case 'i':
        base = 10;
        if (num < 0) {
            unsigned_num = -unsigned_num;
            minus = 1;
        }
        break;
    case 'o':
        base = 8;
        fmt.sign_options = only_minus;
        break;
    case 'u':
        base = 10;
        fmt.sign_options = only_minus;
        break;
    case 'x':
    case 'X':
        base = 16;
        fmt.sign_options = only_minus;
        break;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;

        if (n < 10) {
            n += '0';
        } else {
            n -= 10;
            if (fmt.conversion_char == 'x') {
                n += 'a';
            } else {
                n += 'A';
            }
        }

        *--p = n;
        ++digits;
    } while (unsigned_num != 0);

    if (base == 8 && fmt.alternate_form && *p != '0') {
        *--p = '0';
        ++digits;
    }

    if (fmt.justification_options == zero_fill) {
        fmt.precision = fmt.field_width;

        if (minus || fmt.sign_options != only_minus) {
            --fmt.precision;
        }
        if (base == 16 && fmt.alternate_form) {
            fmt.precision -= 2;
        }
    }

    if (pBuf - p + fmt.precision > 509) {
        return 0;
    }

    while (digits < fmt.precision) {
        *--p = '0';
        ++digits;
    }

    if (base == 16 && fmt.alternate_form) {
        *--p = fmt.conversion_char;
        *--p = '0';
    }

    if (minus) {
        *--p = '-';
    } else if (fmt.sign_options == sign_always) {
        *--p = '+';
    } else if (fmt.sign_options == space_holder) {
        *--p = ' ';
    }

    return p;
}

/* long2str - 0x800C9EC4 */
char* long2str_800C9EC4(signed long num, char* buff, print_format format) {
    unsigned long unsigned_num, base;
    char* p;
    int n, digits;
    int minus = 0;

    unsigned_num = num;
    p = buff;
    *--p = 0;
    digits = 0;

    if (!num && !format.precision &&
        !(format.alternate_form && format.conversion_char == 'o')) {
        return p;
    }

    switch (format.conversion_char) {
    case 'd':
    case 'i':
        base = 10;
        if (num < 0) {
            unsigned_num = -unsigned_num;
            minus = 1;
        }
        break;
    case 'o':
        base = 8;
        format.sign_options = only_minus;
        break;
    case 'u':
        base = 10;
        format.sign_options = only_minus;
        break;
    case 'x':
    case 'X':
        base = 16;
        format.sign_options = only_minus;
        break;
    }

    do {
        n = unsigned_num % base;
        unsigned_num /= base;

        if (n < 10) {
            n += '0';
        } else if (format.conversion_char == 'x') {
            n += 'W';
        } else {
            n += '7';
        }

        *--p = n;
        ++digits;
    } while (unsigned_num != 0);

    if (base == 8 && format.alternate_form && *p != '0') {
        *--p = '0';
        ++digits;
    }

    if (format.justification_options == zero_fill) {
        format.precision = format.field_width;

        if (minus || format.sign_options != only_minus) {
            --format.precision;
        }
        if (base == 16 && format.alternate_form) {
            format.precision -= 2;
        }
    }

    if (buff - p + format.precision > 509) {
        return 0;
    }

    while (digits < format.precision) {
        *--p = '0';
        ++digits;
    }

    if (base == 16 && format.alternate_form) {
        *--p = format.conversion_char;
        *--p = '0';
    }

    if (minus) {
        *--p = '-';
    } else if (format.sign_options == sign_always) {
        *--p = '+';
    } else if (format.sign_options == space_holder) {
        *--p = ' ';
    }

    return p;
}

/* MSL ctype table; bit 0x10 marks a decimal digit. */
extern u8 lbl_80313B18[];
#define isdigit(c) (lbl_80313B18[(u8)(c)] & 0x10)

extern void* __va_arg(void* ap, u32 kind);

/* parse_format - 0x800CA11C */
const char* parse_format_800CA11C(const char* format_string, va_list* arg,
                                  print_format* format) {
    print_format f;
    const char* s = format_string;
    int c;
    int flag_found;

    f.justification_options = right_justification;
    f.sign_options = only_minus;
    f.precision_specified = 0;
    f.alternate_form = 0;
    f.argument_options = normal_argument;
    f.field_width = 0;
    f.precision = 0;

    if ((c = *++s) == '%') {
        f.conversion_char = c;
        *format = f;
        return ((const char*)s + 1);
    }

    for (;;) {
        flag_found = 1;

        switch (c) {
        case '-':
            f.justification_options = left_justification;
            break;
        case '+':
            f.sign_options = sign_always;
            break;
        case ' ':
            if (f.sign_options != sign_always) {
                f.sign_options = space_holder;
            }
            break;
        case '#':
            f.alternate_form = 1;
            break;
        case '0':
            if (f.justification_options != left_justification) {
                f.justification_options = zero_fill;
            }
            break;
        default:
            flag_found = 0;
            break;
        }

        if (flag_found) {
            c = *++s;
        } else {
            break;
        }
    }

    if (c == '*') {
        if ((f.field_width = *(int*)__va_arg(*arg, 1)) < 0) {
            f.justification_options = left_justification;
            f.field_width = -f.field_width;
        }

        c = *++s;
    } else {
        while (isdigit(c)) {
            f.field_width = (f.field_width * 10) + (c - '0');
            c = *++s;
        }
    }

    if (f.field_width > 509) {
        f.conversion_char = 0xFF;
        *format = f;
        return ((const char*)s + 1);
    }

    if (c == '.') {
        f.precision_specified = 1;

        if ((c = *++s) == '*') {
            if ((f.precision = *(int*)__va_arg(*arg, 1)) < 0) {
                f.precision_specified = 0;
            }

            c = *++s;
        } else {
            while (isdigit(c)) {
                f.precision = (f.precision * 10) + (c - '0');
                c = *++s;
            }
        }
    }

    flag_found = 1;

    switch (c) {
    case 'h':
        f.argument_options = short_argument;
        if (s[1] == 'h') {
            f.argument_options = char_argument;
            c = *++s;
        }
        break;

    case 'l':
        f.argument_options = long_argument;
        if (s[1] == 'l') {
            f.argument_options = long_long_argument;
            c = *++s;
        }
        break;

    case 'L':
        f.argument_options = long_double_argument;
        break;

    default:
        flag_found = 0;
        break;
    }

    if (flag_found) {
        c = *++s;
    }

    f.conversion_char = c;

    switch (c) {
    case 'd':
    case 'i':
    case 'u':
    case 'o':
    case 'x':
    case 'X':
        if (f.argument_options == long_double_argument) {
            f.conversion_char = 0xFF;
            break;
        }
        if (!f.precision_specified) {
            f.precision = 1;
        } else if (f.justification_options == zero_fill) {
            f.justification_options = right_justification;
        }
        break;

    case 'f':
    case 'F':
        if (f.argument_options == short_argument ||
            f.argument_options == long_long_argument) {
            f.conversion_char = 0xFF;
            break;
        }
        if (!f.precision_specified) {
            f.precision = 6;
        }
        break;

    case 'a':
    case 'A':
        if (!f.precision_specified) {
            f.precision = 0xD;
        }
        if (f.argument_options == short_argument ||
            f.argument_options == long_long_argument ||
            f.argument_options == char_argument) {
            f.conversion_char = 0xFF;
        }
        break;

    case 'g':
    case 'G':
        if (!f.precision) {
            f.precision = 1;
        }

    case 'e':
    case 'E':
        if (f.argument_options == short_argument ||
            f.argument_options == long_long_argument ||
            f.argument_options == char_argument) {
            f.conversion_char = 0xFF;
            break;
        }
        if (!f.precision_specified) {
            f.precision = 6;
        }
        break;

    case 'p':
        f.conversion_char = 'x';
        f.alternate_form = 1;
        f.argument_options = long_argument;
        f.precision = 8;
        break;

    case 'c':
        if (f.argument_options == long_argument) {
            f.argument_options = wchar_argument;
        } else {
            if (f.precision_specified || f.argument_options != normal_argument) {
                f.conversion_char = 0xFF;
            }
        }
        break;

    case 's':
        if (f.argument_options == long_argument) {
            f.argument_options = wchar_argument;
        } else {
            if (f.argument_options != normal_argument) {
                f.conversion_char = 0xFF;
            }
        }
        break;

    case 'n':
        if (f.argument_options == long_double_argument) {
            f.conversion_char = 0xFF;
        }
        break;

    default:
        f.conversion_char = 0xFF;
        break;
    }

    *format = f;
    return ((const char*)s + 1);
}

/* __StringWrite - 0x800C87F8 | size: 0x6C */
s32 __StringWrite(u8* ctx, const void* src, u32 count) {
    extern void* memcpy(void* dst, const void* src, u32 n);
    u32 pos;
    u32 end;
    u32 len;

    pos = *(u32*)(ctx + 0x8);
    end = *(u32*)(ctx + 0x4);
    len = end - pos;
    if (pos + count <= end) {
        len = count;
    }
    memcpy((void*)(*(u32*)ctx + pos), src, len);
    *(u32*)(ctx + 0x8) += len;
    return 1;
}
