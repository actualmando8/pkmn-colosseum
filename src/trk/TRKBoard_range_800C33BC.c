#include "dolphin/types.h"

extern void OSReport(const char* fmt, ...);

/* Comm table */
extern u32 gDBCommTable[];

/* "%s\n" */
extern char lbl_8026FB94[];

/* TRKUARTInterruptHandler - 0x800C349C | size 0x4 | scope global (empty, returns void) */
void TRKUARTInterruptHandler(void) {
}

/* TRK_board_display - 0x800C34F8 | size 0x30 | scope global */
void TRK_board_display(const char* msg) {
    OSReport(lbl_8026FB94, msg);
}

/* UnreserveEXI2Port - 0x800C3528 | size 0x30 | scope global */
void UnreserveEXI2Port(void) {
    typedef void (*CommFunc)(void);
    CommFunc func = (CommFunc)((u32*)gDBCommTable)[8]; /* offset 0x20 */
    func();
}

/* ReserveEXI2Port - 0x800C3558 | size 0x30 | scope global */
void ReserveEXI2Port(void) {
    typedef void (*CommFunc)(void);
    CommFunc func = (CommFunc)((u32*)gDBCommTable)[9]; /* offset 0x24 */
    func();
}
