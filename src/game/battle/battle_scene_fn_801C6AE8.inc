    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stmw r23, 0xc(r1)
    mr r25, r3
    mr r26, r4
    mr r27, r5
    lis r4, -32698
    li r3, 0x4
    addi r4, r4, 28752
    lwz r4, 0x0(r4)
    addi r28, r4, 0x1
    bl fn_800D67BC
    mullw r24, r26, r28
    lwz r0, -21608(r13)
    lis r3, -32698
    rlwinm r0, r0, 2, 0, 29
    addi r31, r3, 28752
    lfs f31, 0x20(r31)
    add r3, r25, r24
    addi r30, r31, 0x28
    mulli r6, r3, 0xc
    lwzx r5, r30, r0
    lwz r4, 0x30(r31)
    rlwinm r0, r3, 3, 0, 28
    lwz r3, 0x38(r31)
    add r5, r5, r6
    lfs f1, 0x0(r5)
    add r23, r4, r6
    lfs f0, 0x8(r5)
    add r29, r3, r0
    lfs f30, 0x24(r31)
    fmuls f1, f1, f31
    lfs f2, 0x4(r5)
    fmuls f3, f0, f31
    fmuls f2, f2, f30
    bl fn_800D6680
    mr r7, r27
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, 0x0(r29)
    li r3, 0x0
    lfs f2, 0x4(r29)
    bl fn_800D59B8
    lfs f1, 0x0(r23)
    lfs f2, 0x4(r23)
    lfs f3, 0x8(r23)
    bl fn_800D5F34
    addi r29, r25, 0x1
    lwz r0, -21608(r13)
    add r7, r29, r24
    lwz r4, 0x30(r31)
    rlwinm r0, r0, 2, 0, 29
    lwz r3, 0x38(r31)
    mulli r6, r7, 0xc
    lwzx r5, r30, r0
    rlwinm r0, r7, 3, 0, 28
    add r5, r5, r6
    add r23, r3, r0
    lfs f1, 0x0(r5)
    add r24, r4, r6
    lfs f2, 0x4(r5)
    lfs f0, 0x8(r5)
    fmuls f1, f1, f31
    fmuls f2, f2, f30
    fmuls f3, f0, f31
    bl fn_800D6680
    mr r7, r27
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, 0x0(r23)
    li r3, 0x0
    lfs f2, 0x4(r23)
    bl fn_800D59B8
    lfs f1, 0x0(r24)
    lfs f2, 0x4(r24)
    lfs f3, 0x8(r24)
    bl fn_800D5F34
    addi r0, r26, 0x1
    lwz r3, -21608(r13)
    mullw r24, r0, r28
    lwz r4, 0x30(r31)
    rlwinm r0, r3, 2, 0, 29
    lwz r3, 0x38(r31)
    lwzx r5, r30, r0
    add r0, r25, r24
    mulli r6, r0, 0xc
    rlwinm r0, r0, 3, 0, 28
    add r5, r5, r6
    add r26, r3, r0
    lfs f1, 0x0(r5)
    add r25, r4, r6
    lfs f2, 0x4(r5)
    lfs f0, 0x8(r5)
    fmuls f1, f1, f31
    fmuls f2, f2, f30
    fmuls f3, f0, f31
    bl fn_800D6680
    mr r7, r27
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, 0x0(r26)
    li r3, 0x0
    lfs f2, 0x4(r26)
    bl fn_800D59B8
    lfs f1, 0x0(r25)
    lfs f2, 0x4(r25)
    lfs f3, 0x8(r25)
    bl fn_800D5F34
    lwz r0, -21608(r13)
    add r7, r29, r24
    mulli r6, r7, 0xc
    lwz r4, 0x30(r31)
    rlwinm r0, r0, 2, 0, 29
    lwz r3, 0x38(r31)
    lwzx r5, r30, r0
    rlwinm r0, r7, 3, 0, 28
    add r5, r5, r6
    add r24, r4, r6
    lfs f1, 0x0(r5)
    add r25, r3, r0
    lfs f2, 0x4(r5)
    lfs f0, 0x8(r5)
    fmuls f1, f1, f31
    fmuls f2, f2, f30
    fmuls f3, f0, f31
    bl fn_800D6680
    mr r7, r27
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, 0x0(r25)
    li r3, 0x0
    lfs f2, 0x4(r25)
    bl fn_800D59B8
    lfs f1, 0x0(r24)
    lfs f2, 0x4(r24)
    lfs f3, 0x8(r24)
    bl fn_800D5F34
    bl fn_800D6728
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    lmw r23, 0xc(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
