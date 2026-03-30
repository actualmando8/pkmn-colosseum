    nofralloc
    mflr r0
    lis r9, -32719
    stw r0, 0x4(r1)
    stwu r1, -0x48(r1)
    stmw r25, 0x2c(r1)
    addi r26, r3, 0x0
    addi r27, r4, 0x0
    addi r28, r5, 0x0
    addi r29, r6, 0x0
    addi r30, r7, 0x0
    addi r31, r8, 0x0
    addi r25, r9, 16200
    bl OSDisableInterrupts
    lwz r0, 0x44(r25)
    cmpwi r0, -0x1
    beq @L_800D00FC
    bl OSRestoreInterrupts
    li r3, 0x0
    b @L_800D02A8
@L_800D00FC:
    lis r6, 0xcc00
    rlwinm r0, r26, 3, 0, 28
    lwz r7, 0x6438(r6)
    lis r4, 0xf00
    sraw r0, r4, r0
    and r7, r7, r0
    stw r7, 0x6438(r6)
    addi r0, r28, 0x3
    rlwinm r4, r0, 30, 2, 31
    stw r26, 0x44(r25)
    cmplwi r4, 0x0
    li r5, 0x0
    stw r31, 0x54(r25)
    stw r30, 0x4c(r25)
    stw r29, 0x50(r25)
    ble @L_800D01DC
    cmplwi r4, 0x8
    subi r7, r4, 0x8
    ble @L_800D0290
    addi r0, r7, 0x7
    rlwinm r0, r0, 29, 3, 31
    cmplwi r7, 0x0
    mtctr r0
    addi r7, r27, 0x0
    addi r6, r6, 0x6400
    ble @L_800D0290
@L_800D0164:
    lwz r0, 0x0(r7)
    addi r5, r5, 0x8
    stw r0, 0x80(r6)
    lwz r0, 0x4(r7)
    stw r0, 0x84(r6)
    lwz r0, 0x8(r7)
    stw r0, 0x88(r6)
    lwz r0, 0xc(r7)
    stw r0, 0x8c(r6)
    lwz r0, 0x10(r7)
    stw r0, 0x90(r6)
    lwz r0, 0x14(r7)
    stw r0, 0x94(r6)
    lwz r0, 0x18(r7)
    stw r0, 0x98(r6)
    lwz r0, 0x1c(r7)
    addi r7, r7, 0x20
    stw r0, 0x9c(r6)
    addi r6, r6, 0x20
    bdnz @L_800D0164
    b @L_800D0290
@L_800D01B8:
    subf r0, r5, r4
    cmplw r5, r4
    mtctr r0
    bge @L_800D01DC
@L_800D01C8:
    lwz r0, 0x0(r6)
    addi r6, r6, 0x4
    stw r0, 0x80(r7)
    addi r7, r7, 0x4
    bdnz @L_800D01C8
@L_800D01DC:
    lis r4, 0xcc00
    addi r6, r4, 0x6400
    lwzu r0, 0x34(r6)
    li r4, 0x1
    stw r0, 0x24(r1)
    cmplwi r31, 0x0
    lbz r0, 0x24(r1)
    rlwimi r0, r4, 7, 24, 24
    stb r0, 0x24(r1)
    beq @L_800D0208
    b @L_800D020C
@L_800D0208:
    li r4, 0x0
@L_800D020C:
    lbz r0, 0x24(r1)
    rlwimi r0, r4, 6, 25, 25
    cmplwi r28, 0x80
    stb r0, 0x24(r1)
    bne @L_800D0228
    li r0, 0x0
    b @L_800D022C
@L_800D0228:
    mr r0, r28
@L_800D022C:
    rlwinm r4, r0, 0, 24, 31
    lbz r0, 0x25(r1)
    rlwimi r0, r4, 0, 25, 31
    cmplwi r30, 0x80
    stb r0, 0x25(r1)
    bne @L_800D024C
    li r0, 0x0
    b @L_800D0250
@L_800D024C:
    mr r0, r30
@L_800D0250:
    rlwinm r4, r0, 0, 24, 31
    lbz r0, 0x26(r1)
    rlwimi r0, r4, 0, 25, 31
    stb r0, 0x26(r1)
    li r4, 0x1
    lbz r0, 0x27(r1)
    rlwimi r0, r26, 1, 29, 30
    stb r0, 0x27(r1)
    lbz r0, 0x27(r1)
    rlwimi r0, r4, 0, 31, 31
    stb r0, 0x27(r1)
    lwz r0, 0x24(r1)
    stw r0, 0x0(r6)
    bl OSRestoreInterrupts
    li r3, 0x1
    b @L_800D02A8
@L_800D0290:
    lis r6, 0xcc00
    rlwinm r0, r5, 2, 0, 29
    addi r7, r6, 0x6400
    add r6, r27, r0
    add r7, r7, r0
    b @L_800D01B8
@L_800D02A8:
    lmw r25, 0x2c(r1)
    lwz r0, 0x4c(r1)
    addi r1, r1, 0x48
    mtlr r0
    blr
