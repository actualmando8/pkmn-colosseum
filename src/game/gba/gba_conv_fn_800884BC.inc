    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r26, 0x58(r1)
    lis r6, -32729
    lis r9, -32704
    addi r8, r6, -2936
    li r7, 0x0
    lwz r29, 0x0(r8)
    addi r6, r9, -19720
    lwz r30, 0x4(r8)
    lwz r31, 0x8(r8)
    lwz r12, 0xc(r8)
    lhz r0, 0x10(r8)
    sth r3, 0x0(r6)
    sth r4, 0x2(r6)
    sth r5, 0x4(r6)
    stw r29, 0x8(r1)
    stw r30, 0xc(r1)
    stw r31, 0x10(r1)
    stw r12, 0x14(r1)
    sth r0, 0x18(r1)
    rlwinm r11, r0, 0, 16, 31
    rlwinm r4, r11, 0, 16, 31
    mr r10, r29
    mr r9, r30
    mr r8, r31
    mr r5, r12
    rlwinm r3, r4, 0, 16, 31
@L_80088530:
    stw r29, 0x1c(r1)
    addi r26, r1, 0x44
    lhz r27, 0x0(r6)
    li r28, 0x0
    stw r30, 0x20(r1)
    stw r31, 0x24(r1)
    stw r12, 0x28(r1)
    sth r11, 0x2c(r1)
    li r0, 0x3
    mtctr r0
@L_80088558:
    stw r10, 0x30(r1)
    li r0, 0x0
    stw r9, 0x34(r1)
    stw r8, 0x38(r1)
    stw r5, 0x3c(r1)
    sth r4, 0x40(r1)
    cmpwi r0, 0x0
    stw r10, 0x44(r1)
    stw r9, 0x48(r1)
    stw r8, 0x4c(r1)
    stw r5, 0x50(r1)
    sth r3, 0x54(r1)
    blt @L_800885A4
    cmpwi r0, 0x3
    bge @L_800885A4
    cmpwi r28, 0x0
    blt @L_800885A4
    cmpwi r28, 0x3
    blt @L_800885AC
@L_800885A4:
    li r0, 0x0
    b @L_800885B0
@L_800885AC:
    lhz r0, 0x0(r26)
@L_800885B0:
    rlwinm r0, r0, 0, 16, 31
    cmplw r0, r27
    bne @L_800885C4
    li r0, 0x1
    b @L_80088684
@L_800885C4:
    li r0, 0x1
    cmpwi r0, 0x0
    stw r10, 0x44(r1)
    stw r9, 0x48(r1)
    stw r8, 0x4c(r1)
    stw r5, 0x50(r1)
    sth r3, 0x54(r1)
    blt @L_800885FC
    cmpwi r0, 0x3
    bge @L_800885FC
    cmpwi r28, 0x0
    blt @L_800885FC
    cmpwi r28, 0x3
    blt @L_80088604
@L_800885FC:
    li r0, 0x0
    b @L_80088608
@L_80088604:
    lhz r0, 0x2(r26)
@L_80088608:
    rlwinm r0, r0, 0, 16, 31
    cmplw r0, r27
    bne @L_8008861C
    li r0, 0x1
    b @L_80088684
@L_8008861C:
    li r0, 0x2
    cmpwi r0, 0x0
    stw r10, 0x44(r1)
    stw r9, 0x48(r1)
    stw r8, 0x4c(r1)
    stw r5, 0x50(r1)
    sth r3, 0x54(r1)
    blt @L_80088654
    cmpwi r0, 0x3
    bge @L_80088654
    cmpwi r28, 0x0
    blt @L_80088654
    cmpwi r28, 0x3
    blt @L_8008865C
@L_80088654:
    li r0, 0x0
    b @L_80088660
@L_8008865C:
    lhz r0, 0x4(r26)
@L_80088660:
    rlwinm r0, r0, 0, 16, 31
    cmplw r0, r27
    bne @L_80088674
    li r0, 0x1
    b @L_80088684
@L_80088674:
    addi r26, r26, 0x6
    addi r28, r28, 0x1
    bdnz @L_80088558
    li r0, 0x0
@L_80088684:
    cmpwi r0, 0x0
    bne @L_80088694
    li r3, 0x2
    b @L_800886BC
@L_80088694:
    addi r6, r6, 0x2
    addi r7, r7, 0x1
    cmpwi r7, 0x3
    blt @L_80088530
    li r3, 0x387
    bl fn_800FF730
    bl fn_800F0308
    lis r3, -32704
    addi r3, r3, -19720
    lwz r3, 0x8(r3)
@L_800886BC:
    lmw r26, 0x58(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
