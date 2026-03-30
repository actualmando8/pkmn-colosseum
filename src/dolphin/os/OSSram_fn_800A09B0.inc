    nofralloc
    mflr r0
    cmpwi r3, 0x0
    stw r0, 0x4(r1)
    lis r3, -32704
    stwu r1, -0x30(r1)
    stmw r27, 0x1c(r1)
    addi r31, r3, -18368
    beq @L_800A0C90
    cmplwi r4, 0x0
    bne @L_800A0B84
    lbz r3, 0x13(r31)
    rlwinm r0, r3, 0, 30, 31
    cmplwi r0, 0x2
    ble @L_800A09F0
    rlwinm r0, r3, 0, 0, 29
    stb r0, 0x13(r31)
@L_800A09F0:
    li r0, 0x0
    sth r0, 0x2(r31)
    addi r5, r31, 0x14
    addi r6, r31, 0xc
    addi r3, r5, 0x1
    sth r0, 0x0(r31)
    subf r3, r6, r3
    cmplw r6, r5
    rlwinm r3, r3, 31, 1, 31
    bge @L_800A0B84
    rlwinm. r0, r3, 29, 3, 31
    mtctr r0
    beq @L_800A0B54
@L_800A0A24:
    lhz r5, 0x0(r31)
    lhz r0, 0x0(r6)
    add r0, r5, r0
    sth r0, 0x0(r31)
    lhz r0, 0x0(r6)
    lhz r5, 0x2(r31)
    nor r0, r0, r0
    add r0, r5, r0
    sth r0, 0x2(r31)
    lhz r5, 0x0(r31)
    lhz r0, 0x2(r6)
    add r0, r5, r0
    sth r0, 0x0(r31)
    lhz r0, 0x2(r6)
    lhz r5, 0x2(r31)
    nor r0, r0, r0
    add r0, r5, r0
    sth r0, 0x2(r31)
    lhz r5, 0x0(r31)
    lhz r0, 0x4(r6)
    add r0, r5, r0
    sth r0, 0x0(r31)
    lhz r0, 0x4(r6)
    lhz r5, 0x2(r31)
    nor r0, r0, r0
    add r0, r5, r0
    sth r0, 0x2(r31)
    lhz r5, 0x0(r31)
    lhz r0, 0x6(r6)
    add r0, r5, r0
    sth r0, 0x0(r31)
    lhz r0, 0x6(r6)
    lhz r5, 0x2(r31)
    nor r0, r0, r0
    add r0, r5, r0
    sth r0, 0x2(r31)
    lhz r5, 0x0(r31)
    lhz r0, 0x8(r6)
    add r0, r5, r0
    sth r0, 0x0(r31)
    lhz r0, 0x8(r6)
    lhz r5, 0x2(r31)
    nor r0, r0, r0
    add r0, r5, r0
    sth r0, 0x2(r31)
    lhz r5, 0x0(r31)
    lhz r0, 0xa(r6)
    add r0, r5, r0
    sth r0, 0x0(r31)
    lhz r0, 0xa(r6)
    lhz r5, 0x2(r31)
    nor r0, r0, r0
    add r0, r5, r0
    sth r0, 0x2(r31)
    lhz r5, 0x0(r31)
    lhz r0, 0xc(r6)
    add r0, r5, r0
    sth r0, 0x0(r31)
    lhz r0, 0xc(r6)
    lhz r5, 0x2(r31)
    nor r0, r0, r0
    add r0, r5, r0
    sth r0, 0x2(r31)
    lhz r5, 0x0(r31)
    lhz r0, 0xe(r6)
    add r0, r5, r0
    sth r0, 0x0(r31)
    lhz r0, 0xe(r6)
    addi r6, r6, 0x10
    lhz r5, 0x2(r31)
    nor r0, r0, r0
    add r0, r5, r0
    sth r0, 0x2(r31)
    bdnz @L_800A0A24
    andi. r3, r3, 0x7
    beq @L_800A0B84
@L_800A0B54:
    mtctr r3
@L_800A0B58:
    lhz r5, 0x0(r31)
    lhz r0, 0x0(r6)
    add r0, r5, r0
    sth r0, 0x0(r31)
    lhz r0, 0x0(r6)
    addi r6, r6, 0x2
    lhz r5, 0x2(r31)
    nor r0, r0, r0
    add r0, r5, r0
    sth r0, 0x2(r31)
    bdnz @L_800A0B58
@L_800A0B84:
    addi r30, r31, 0x40
    lwz r0, 0x40(r31)
    cmplw r4, r0
    bge @L_800A0B98
    stw r4, 0x0(r30)
@L_800A0B98:
    lwz r29, 0x0(r30)
    lis r3, -32758
    addi r5, r3, 1612
    subfic r27, r29, 0x40
    add r28, r31, r29
    li r3, 0x0
    li r4, 0x1
    bl EXILock
    cmpwi r3, 0x0
    bne @L_800A0BC8
    li r0, 0x0
    b @L_800A0C78
@L_800A0BC8:
    li r3, 0x0
    li r4, 0x1
    li r5, 0x3
    bl EXISelect
    cmpwi r3, 0x0
    bne @L_800A0BF0
    li r3, 0x0
    bl EXIUnlock
    li r0, 0x0
    b @L_800A0C78
@L_800A0BF0:
    rlwinm r3, r29, 6, 0, 25
    addi r0, r3, 0x100
    oris r0, r0, 0xa000
    stw r0, 0x10(r1)
    addi r4, r1, 0x10
    li r3, 0x0
    li r5, 0x4
    li r6, 0x1
    li r7, 0x0
    bl EXIImm
    cntlzw r0, r3
    rlwinm r29, r0, 27, 5, 31
    li r3, 0x0
    bl EXISync
    cntlzw r0, r3
    rlwinm r0, r0, 27, 5, 31
    addi r4, r28, 0x0
    addi r5, r27, 0x0
    or r29, r29, r0
    li r3, 0x0
    li r6, 0x1
    bl fn_80098368
    cntlzw r0, r3
    rlwinm r0, r0, 27, 5, 31
    or r29, r29, r0
    li r3, 0x0
    bl EXIDeselect
    cntlzw r0, r3
    rlwinm r0, r0, 27, 5, 31
    or r29, r29, r0
    li r3, 0x0
    bl EXIUnlock
    cntlzw r0, r29
    rlwinm r0, r0, 27, 5, 31
@L_800A0C78:
    stw r0, 0x4c(r31)
    lwz r0, 0x4c(r31)
    cmpwi r0, 0x0
    beq @L_800A0C90
    li r0, 0x40
    stw r0, 0x0(r30)
@L_800A0C90:
    li r0, 0x0
    stw r0, 0x48(r31)
    lwz r3, 0x44(r31)
    bl OSRestoreInterrupts
    lwz r3, 0x4c(r31)
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
