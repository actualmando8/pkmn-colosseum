    nofralloc
    stwu r1, -0xe0(r1)
    mflr r0
    lis r5, -32704
    stw r0, 0xe4(r1)
    addi r6, r5, 19560
    stmw r27, 0xcc(r1)
    mr r30, r3
    lwz r5, 0x0(r6)
    cmplwi r5, 0x0
    bne @L_8010C900
    li r3, 0x0
    b @L_8010CA1C
@L_8010C900:
    lwz r0, 0x4(r5)
    cmplw r4, r0
    blt @L_8010C914
    li r3, 0x0
    b @L_8010CA1C
@L_8010C914:
    lwz r5, 0x3704(r6)
    mulli r0, r4, 0x28
    mulli r4, r5, 0xdc0
    add r4, r6, r4
    addi r27, r4, 0x4
    add r27, r27, r0
    bl fn_800A2D38
    lhz r0, 0x24(r27)
    rlwinm. r0, r0, 0, 31, 31
    beq @L_8010CA18
    lis r3, -32729
    lis r4, -32729
    addi r6, r3, 8260
    addi r31, r27, 0xc
    lwzu r27, 8224(r4)
    addi r3, r1, 0x44
    lwz r5, 0x0(r6)
    lwz r28, 0x4(r4)
    lwz r29, 0x8(r4)
    lwz r12, 0xc(r4)
    lwz r11, 0x10(r4)
    lwz r10, 0x14(r4)
    lwz r9, 0x18(r4)
    lwz r8, 0x1c(r4)
    lwz r7, 0x20(r4)
    lwz r4, 0x4(r6)
    lwz r0, 0x8(r6)
    stw r27, 0x74(r1)
    stw r28, 0x78(r1)
    stw r29, 0x7c(r1)
    stw r12, 0x80(r1)
    stw r11, 0x84(r1)
    stw r10, 0x88(r1)
    stw r9, 0x8c(r1)
    stw r8, 0x90(r1)
    stw r7, 0x94(r1)
    stw r5, 0x8(r1)
    stw r4, 0xc(r1)
    stw r0, 0x10(r1)
    bl fn_800A2D38
    li r27, 0x0
    addi r28, r1, 0x8
@L_8010C9BC:
    lwz r0, 0x0(r28)
    addi r4, r1, 0x74
    addi r3, r1, 0x14
    mulli r5, r0, 0xc
    rlwinm r0, r0, 2, 0, 29
    lfsx f1, r31, r0
    add r4, r4, r5
    bl fn_800A3244
    addi r4, r1, 0x44
    addi r3, r1, 0x14
    mr r5, r4
    bl fn_800A2D98
    addi r27, r27, 0x1
    addi r28, r28, 0x4
    cmpwi r27, 0x3
    blt @L_8010C9BC
    addi r3, r1, 0x44
    addi r4, r1, 0x98
    bl fn_800A2D64
    mr r3, r30
    mr r5, r30
    addi r4, r1, 0x98
    bl fn_800A2D98
@L_8010CA18:
    li r3, 0x1
@L_8010CA1C:
    lmw r27, 0xcc(r1)
    lwz r0, 0xe4(r1)
    mtlr r0
    addi r1, r1, 0xe0
    blr
