    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0x9
    stmw r23, 0x1c(r1)
    mr r30, r5
    mr r28, r3
    mr r29, r4
    mr r31, r6
    bne @L_80238B40
    li r3, 0x3f
    b @L_80238E1C
@L_80238B40:
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r26, r3, 0, 16, 31
    mr r3, r29
    bl fn_80205BE8
    mr r4, r26
    mr r26, r3
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80238BDC
    mr r3, r26
    bl fn_801248C4
    b @L_80238BE0
@L_80238BDC:
    li r3, 0x0
@L_80238BE0:
    rlwinm r0, r3, 0, 16, 31
    subfic r0, r0, 0x1a
    cntlzw r0, r0
    rlwinm r0, r0, 27, 24, 31
    cmplwi r0, 0x1
    bne @L_80238C0C
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_80238C0C
    li r3, 0x43
    b @L_80238E1C
@L_80238C0C:
    li r26, 0x0
    mr r27, r26
    b @L_80238D0C
@L_80238C18:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r23, r3, 0, 16, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r24, r3, 0, 16, 31
    mr r3, r29
    bl fn_80205BE8
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r25, r3, 0, 16, 31
    mr r4, r24
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80238CE8
    mr r3, r28
    mr r4, r29
    mr r5, r23
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80238CCC
    mr r4, r25
    rlwinm r6, r27, 0, 24, 31
    li r3, 0x0
    li r5, 0x16
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    b @L_80238CEC
@L_80238CCC:
    mr r4, r25
    rlwinm r6, r27, 0, 24, 31
    li r3, 0x0
    li r5, 0x16
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    b @L_80238CEC
@L_80238CE8:
    li r4, 0x9
@L_80238CEC:
    rlwinm r0, r4, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_80238D08
    clrlslwi r0, r26, 16, 1
    addi r3, r1, 0x8
    sthx r4, r3, r0
    addi r26, r26, 0x1
@L_80238D08:
    addi r27, r27, 0x1
@L_80238D0C:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_80238C18
    rlwinm. r0, r26, 0, 16, 31
    bne @L_80238D28
    li r3, 0x3f
    b @L_80238E1C
@L_80238D28:
    mr r3, r30
    mr r5, r26
    addi r4, r1, 0x8
    bl fn_8010C650
    mr r0, r3
    mr r3, r28
    mr r24, r0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r28
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r23, r3, 0, 16, 31
    mr r3, r29
    bl fn_80205BE8
    mr r4, r23
    mr r23, r3
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80238DE0
    mr r3, r23
    bl fn_801248C4
    b @L_80238DE4
@L_80238DE0:
    li r3, 0x0
@L_80238DE4:
    rlwinm r0, r3, 0, 16, 31
    subfic r0, r0, 0x19
    cntlzw r0, r0
    rlwinm r0, r0, 27, 24, 31
    cmplwi r0, 0x1
    bne @L_80238E18
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x41
    beq @L_80238E18
    extsh. r0, r31
    ble @L_80238E18
    li r3, 0x43
    b @L_80238E1C
@L_80238E18:
    mr r3, r24
@L_80238E1C:
    lmw r23, 0x1c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
