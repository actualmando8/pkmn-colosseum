    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    rlwinm r0, r5, 0, 16, 31
    cmplwi r0, 0x9
    stmw r25, 0x14(r1)
    mr r27, r5
    mr r31, r3
    mr r30, r4
    mr r29, r6
    bne @L_80237970
    li r3, 0x3f
    b @L_80237CA4
@L_80237970:
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
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802379FC
    mr r3, r30
    bl fn_80207BF4
    b @L_80237A00
@L_802379FC:
    li r3, 0x0
@L_80237A00:
    rlwinm r0, r3, 0, 16, 31
    subfic r0, r0, 0x1a
    cntlzw r0, r0
    rlwinm r0, r0, 27, 24, 31
    cmplwi r0, 0x1
    bne @L_80237A2C
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x4
    bne @L_80237A2C
    li r3, 0x43
    b @L_80237CA4
@L_80237A2C:
    li r28, 0x0
    mr r26, r28
    b @L_80237AF4
@L_80237A38:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r25, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80237AD0
    mr r3, r31
    mr r4, r30
    mr r5, r25
    bl fn_801F8424
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80237AC0
    mr r3, r30
    mr r4, r26
    bl fn_80207B8C
    b @L_80237AD4
@L_80237AC0:
    mr r3, r30
    mr r4, r26
    bl fn_80207B8C
    b @L_80237AD4
@L_80237AD0:
    li r3, 0x9
@L_80237AD4:
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x9
    beq @L_80237AF0
    clrlslwi r0, r28, 16, 1
    addi r4, r1, 0x8
    sthx r3, r4, r0
    addi r28, r28, 0x1
@L_80237AF0:
    addi r26, r26, 0x1
@L_80237AF4:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_80237A38
    rlwinm. r0, r28, 0, 16, 31
    bne @L_80237B10
    li r3, 0x3f
    b @L_80237CA4
@L_80237B10:
    mr r3, r27
    mr r5, r28
    addi r4, r1, 0x8
    bl fn_8010C650
    rlwinm. r0, r27, 0, 16, 31
    mr r27, r3
    beq @L_80237B34
    cmplwi r0, 0x1
    bne @L_80237BD8
@L_80237B34:
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80237B8C
    mr r3, r30
    li r4, 0x19
    bl fn_802026E4
    b @L_80237B90
@L_80237B8C:
    li r3, 0x0
@L_80237B90:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80237BD8
    addi r4, r1, 0x8
    rlwinm r0, r28, 0, 16, 31
    li r5, 0x0
    b @L_80237BCC
@L_80237BAC:
    clrlslwi r3, r5, 16, 1
    lhzx r3, r4, r3
    cmplwi r3, 0x9
    beq @L_80237BC8
    cmplwi r3, 0x7
    bne @L_80237BC8
    li r27, 0x3f
@L_80237BC8:
    addi r5, r5, 0x1
@L_80237BCC:
    rlwinm r3, r5, 0, 16, 31
    cmplw r3, r0
    blt @L_80237BAC
@L_80237BD8:
    mr r3, r31
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
    mr r3, r31
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80237C68
    mr r3, r30
    bl fn_80207BF4
    b @L_80237C6C
@L_80237C68:
    li r3, 0x0
@L_80237C6C:
    rlwinm r0, r3, 0, 16, 31
    subfic r0, r0, 0x19
    cntlzw r0, r0
    rlwinm r0, r0, 27, 24, 31
    cmplwi r0, 0x1
    bne @L_80237CA0
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x41
    beq @L_80237CA0
    extsh. r0, r29
    ble @L_80237CA0
    li r3, 0x43
    b @L_80237CA4
@L_80237CA0:
    mr r3, r27
@L_80237CA4:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
