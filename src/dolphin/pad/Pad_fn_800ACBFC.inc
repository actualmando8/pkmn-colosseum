    nofralloc
    mflr r0
    lis r3, 0xcc00
    stw r0, 0x4(r1)
    addi r3, r3, 0x5000
    stwu r1, -0x2c0(r1)
    stmw r14, 0x278(r1)
    @L_800ACC14:
    lhz r0, 0x16(r3)
    clrlwi. r0, r0, 31
    beq @L_800ACC14
    lis r23, 0xcc00
    addi r0, r23, 0x5000
    stw r0, 0x248(r1)
    lis r3, 0x100
    addi r0, r1, 0x1f3
    stw r3, lbl_8047A910(r13)
    lis r5, 0xdeae
    lwz r4, 0x248(r1)
    clrrwi r22, r0, 5
    addi r0, r1, 0x1b3
    lhz r4, 0x12(r4)
    addi r7, r1, 0xb3
    clrrwi r7, r7, 5
    clrrwi r4, r4, 6
    stw r7, 0x21c(r1)
    ori r6, r4, 0x23
    lwz r4, 0x248(r1)
    addi r10, r1, 0x173
    sth r6, 0x12(r4)
    subi r5, r5, 0x4111
    lis r4, 0xbad1
    clrrwi r21, r0, 5
    stw r5, 0x0(r22)
    subi r0, r4, 0x4530
    stw r0, 0x0(r21)
    addi r4, r1, 0x33
    addi r6, r1, 0x73
    stw r5, 0x4(r22)
    clrrwi r4, r4, 5
    clrrwi r6, r6, 5
    stw r0, 0x4(r21)
    addi r9, r1, 0x133
    addi r8, r1, 0xf3
    stw r5, 0x8(r22)
    addi r19, r3, 0x0
    addi r3, r22, 0x0
    stw r0, 0x8(r21)
    clrrwi r24, r10, 5
    clrrwi r28, r9, 5
    stw r5, 0xc(r22)
    clrrwi r20, r8, 5
    stw r0, 0xc(r21)
    stw r5, 0x10(r22)
    stw r0, 0x10(r21)
    stw r5, 0x14(r22)
    stw r0, 0x14(r21)
    stw r5, 0x18(r22)
    stw r0, 0x18(r21)
    stw r5, 0x1c(r22)
    lwz r5, 0x248(r1)
    stw r4, 0x214(r1)
    li r4, 0x3
    addi r5, r5, 0x12
    sth r4, 0x270(r1)
    li r4, 0x20
    stw r6, 0x218(r1)
    stw r0, 0x1c(r21)
    stw r5, 0x248(r1)
    bl DCFlushRange
    addi r3, r21, 0x0
    li r4, 0x20
    bl DCFlushRange
    li r0, 0x0
    stw r0, lbl_8047A914(r13)
    addi r3, r28, 0x0
    li r4, 0x20
    bl DCInvalidateRange
    srwi r3, r28, 16
    lhz r0, 0x5020(r23)
    stw r3, 0x23c(r1)
    addi r25, r23, 0x5000
    clrrwi r3, r0, 10
    lwz r0, 0x23c(r1)
    addi r26, r23, 0x5000
    srwi r17, r19, 16
    or r0, r3, r0
    sthu r0, 0x20(r25)
    clrlwi r3, r28, 16
    addi r27, r23, 0x5000
    lhz r0, 0x5022(r23)
    clrlwi r18, r19, 16
    stw r3, 0x24c(r1)
    addi r28, r23, 0x5000
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x24c(r1)
    addi r30, r23, 0x5000
    addi r29, r23, 0x5000
    or r0, r3, r0
    sthu r0, 0x22(r26)
    lhz r0, 0x5024(r23)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sthu r0, 0x24(r27)
    lhz r0, 0x5026(r23)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sthu r0, 0x26(r28)
    lhz r0, 0x5028(r23)
    ori r0, r0, 0x8000
    sth r0, 0x5028(r23)
    lhz r0, 0x5028(r23)
    clrrwi r0, r0, 10
    nop
    sthu r0, 0x28(r30)
    lhz r0, 0x502a(r23)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sthu r0, 0x2a(r29)
    @L_800ACDD0:
    addi r31, r23, 0x5000
    lhzu r0, 0xa(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ACDD0
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    srwi r0, r22, 16
    lhz r3, 0x0(r25)
    stw r0, 0x244(r1)
    clrrwi r3, r3, 10
    lwz r0, 0x244(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    clrlwi r0, r22, 16
    stw r0, 0x250(r1)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x250(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ACE7C:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ACE7C
    lhz r0, 0x0(r31)
    li r4, -0x89
    addi r3, r24, 0x0
    and r0, r0, r4
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    li r4, 0x0
    li r5, 0x20
    bl memset
    addi r3, r24, 0x0
    li r4, 0x20
    bl DCFlushRange
    srwi r0, r24, 16
    lhz r3, 0x0(r25)
    stw r0, 0x240(r1)
    clrrwi r3, r3, 10
    lwz r0, 0x240(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    clrlwi r0, r24, 16
    stw r0, 0x254(r1)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x254(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    ori r0, r0, 0x8000
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ACF3C:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ACF3C
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    lwz r3, 0x0(r24)
    lwz r0, 0x0(r22)
    cmplw r3, r0
    bne @L_800AE3D0
    addi r3, r20, 0x0
    li r4, 0x20
    bl DCInvalidateRange
    srwi r0, r20, 16
    lhz r4, 0x0(r25)
    stw r0, 0x234(r1)
    addis r3, r19, 0x20
    clrrwi r4, r4, 10
    lwz r0, 0x234(r1)
    srwi r16, r3, 16
    clrlwi r23, r3, 16
    or r0, r4, r0
    sth r0, 0x0(r25)
    clrlwi r0, r20, 16
    stw r0, 0x258(r1)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x258(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r16
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r23
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    ori r0, r0, 0x8000
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD00C:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD00C
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    lwz r3, 0x21c(r1)
    li r4, 0x20
    bl DCInvalidateRange
    lwz r0, 0x21c(r1)
    addis r3, r19, 0x100
    lhz r4, 0x0(r25)
    srwi r14, r3, 16
    srwi r0, r0, 16
    stw r0, 0x22c(r1)
    clrrwi r4, r4, 10
    clrlwi r15, r3, 16
    lwz r0, 0x22c(r1)
    or r0, r4, r0
    sth r0, 0x0(r25)
    lwz r0, 0x21c(r1)
    clrlwi r0, r0, 16
    stw r0, 0x25c(r1)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x25c(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r14
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r15
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    ori r0, r0, 0x8000
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD0D4:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD0D4
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    lwz r3, 0x218(r1)
    li r4, 0x20
    bl DCInvalidateRange
    lwz r0, 0x218(r1)
    addi r3, r19, 0x200
    lhz r4, 0x0(r25)
    srwi r0, r0, 16
    stw r0, 0x224(r1)
    clrrwi r4, r4, 10
    lwz r0, 0x224(r1)
    or r0, r4, r0
    sth r0, 0x0(r25)
    srwi r0, r3, 16
    clrlwi r3, r3, 16
    stw r0, 0x230(r1)
    lwz r0, 0x218(r1)
    stw r3, 0x264(r1)
    clrlwi r0, r0, 16
    stw r0, 0x260(r1)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x260(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r3, r0, 10
    lwz r0, 0x230(r1)
    or r0, r3, r0
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x264(r1)
    or r0, r3, r0
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    ori r0, r0, 0x8000
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD1AC:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD1AC
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    lwz r3, 0x214(r1)
    li r4, 0x20
    bl DCInvalidateRange
    lwz r0, 0x214(r1)
    addis r3, r19, 0x40
    lhz r4, 0x0(r25)
    srwi r0, r0, 16
    stw r0, 0x220(r1)
    clrrwi r4, r4, 10
    lwz r0, 0x220(r1)
    or r0, r4, r0
    sth r0, 0x0(r25)
    srwi r0, r3, 16
    clrlwi r3, r3, 16
    stw r0, 0x228(r1)
    lwz r0, 0x214(r1)
    stw r3, 0x26c(r1)
    clrlwi r0, r0, 16
    stw r0, 0x268(r1)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x268(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r3, r0, 10
    lwz r0, 0x228(r1)
    or r0, r3, r0
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x26c(r1)
    or r0, r3, r0
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    ori r0, r0, 0x8000
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD284:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD284
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    srwi r0, r21, 16
    lhz r3, 0x0(r25)
    stw r0, 0x238(r1)
    clrlwi r20, r21, 16
    clrrwi r3, r3, 10
    lwz r0, 0x238(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r20
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r16
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r23
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD324:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD324
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x244(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x250(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD3B8:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD3B8
    lhz r0, 0x0(r31)
    li r4, -0x89
    addi r3, r24, 0x0
    and r0, r0, r4
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    li r4, 0x0
    li r5, 0x20
    bl memset
    addi r3, r24, 0x0
    li r4, 0x20
    bl DCFlushRange
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x240(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x254(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r16
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r23
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    ori r0, r0, 0x8000
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD468:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD468
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    lwz r3, 0x0(r24)
    lwz r0, 0x0(r22)
    cmplw r3, r0
    bne @L_800AD540
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x23c(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x24c(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD510:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD510
    lhz r4, 0x0(r31)
    li r3, -0x89
    lis r0, 0x20
    and r3, r4, r3
    ori r3, r3, 0x20
    sth r3, 0x0(r31)
    addis r19, r19, 0x20
    stw r0, lbl_8047A914(r13)
    b @L_800AE3B4
    @L_800AD540:
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x238(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r20
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r14
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r15
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD5B0:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD5B0
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x244(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x250(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD644:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD644
    lhz r0, 0x0(r31)
    li r4, -0x89
    addi r3, r24, 0x0
    and r0, r0, r4
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    li r4, 0x0
    li r5, 0x20
    bl memset
    addi r3, r24, 0x0
    li r4, 0x20
    bl DCFlushRange
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x240(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x254(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r14
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r15
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    ori r0, r0, 0x8000
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD6F4:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD6F4
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    lwz r3, 0x0(r24)
    lwz r0, 0x0(r22)
    cmplw r3, r0
    bne @L_800AD86C
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x23c(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x24c(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD79C:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD79C
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x234(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x258(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r16
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r23
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD830:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD830
    lhz r4, 0x0(r31)
    li r0, -0x89
    lis r3, 0x40
    and r0, r4, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    addis r19, r19, 0x40
    lhz r0, 0x270(r1)
    stw r3, lbl_8047A914(r13)
    ori r0, r0, 0x8
    sth r0, 0x270(r1)
    b @L_800AE3B4
    @L_800AD86C:
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x238(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r20
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r3, r0, 10
    lwz r0, 0x230(r1)
    or r0, r3, r0
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x264(r1)
    or r0, r3, r0
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD8E4:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD8E4
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x244(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x250(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AD978:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AD978
    lhz r0, 0x0(r31)
    li r4, -0x89
    addi r3, r24, 0x0
    and r0, r0, r4
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    li r4, 0x0
    li r5, 0x20
    bl memset
    addi r3, r24, 0x0
    li r4, 0x20
    bl DCFlushRange
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x240(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x254(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r3, r0, 10
    lwz r0, 0x230(r1)
    or r0, r3, r0
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x264(r1)
    or r0, r3, r0
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    ori r0, r0, 0x8000
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADA30:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADA30
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    lwz r3, 0x0(r24)
    lwz r0, 0x0(r22)
    cmplw r3, r0
    bne @L_800ADC3C
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x23c(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x24c(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADAD8:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADAD8
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x234(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x258(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r16
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r23
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADB6C:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADB6C
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x22c(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x25c(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r14
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r15
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADC00:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADC00
    lhz r4, 0x0(r31)
    li r0, -0x89
    lis r3, 0x80
    and r0, r4, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    addis r19, r19, 0x80
    lhz r0, 0x270(r1)
    stw r3, lbl_8047A914(r13)
    ori r0, r0, 0x10
    sth r0, 0x270(r1)
    b @L_800AE3B4
    @L_800ADC3C:
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x238(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r20
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r3, r0, 10
    lwz r0, 0x228(r1)
    or r0, r3, r0
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x26c(r1)
    or r0, r3, r0
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADCB4:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADCB4
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x244(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x250(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADD48:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADD48
    lhz r0, 0x0(r31)
    li r4, -0x89
    addi r3, r24, 0x0
    and r0, r0, r4
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    li r4, 0x0
    li r5, 0x20
    bl memset
    addi r3, r24, 0x0
    li r4, 0x20
    bl DCFlushRange
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x240(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x254(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r3, r0, 10
    lwz r0, 0x228(r1)
    or r0, r3, r0
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x26c(r1)
    or r0, r3, r0
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    ori r0, r0, 0x8000
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADE00:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADE00
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    bl fn_80098034
    lwz r0, 0x0(r24)
    lwz r3, 0x0(r22)
    cmplw r0, r3
    bne @L_800AE0A8
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x23c(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x24c(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADEA8:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADEA8
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x234(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x258(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r16
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r23
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADF3C:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADF3C
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x22c(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x25c(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r14
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r15
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800ADFD0:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800ADFD0
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x224(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x260(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r3, r0, 10
    lwz r0, 0x230(r1)
    or r0, r3, r0
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x264(r1)
    or r0, r3, r0
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AE06C:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AE06C
    lhz r4, 0x0(r31)
    li r0, -0x89
    lis r3, 0x100
    and r0, r4, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    addis r19, r19, 0x100
    lhz r0, 0x270(r1)
    stw r3, lbl_8047A914(r13)
    ori r0, r0, 0x18
    sth r0, 0x270(r1)
    b @L_800AE3B4
    @L_800AE0A8:
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x23c(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x24c(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r17
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r18
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AE11C:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AE11C
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x234(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x258(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r16
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r23
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AE1B0:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AE1B0
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x22c(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x25c(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r0, r0, 10
    or r0, r0, r14
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r0, r0, 0, 27, 15
    or r0, r0, r15
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AE244:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AE244
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x224(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x260(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r3, r0, 10
    lwz r0, 0x230(r1)
    or r0, r3, r0
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x264(r1)
    or r0, r3, r0
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AE2E0:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AE2E0
    lhz r3, 0x0(r31)
    li r0, -0x89
    and r0, r3, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    lhz r0, 0x0(r25)
    clrrwi r3, r0, 10
    lwz r0, 0x220(r1)
    or r0, r3, r0
    sth r0, 0x0(r25)
    lhz r0, 0x0(r26)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x268(r1)
    or r0, r3, r0
    sth r0, 0x0(r26)
    lhz r0, 0x0(r27)
    clrrwi r3, r0, 10
    lwz r0, 0x228(r1)
    or r0, r3, r0
    sth r0, 0x0(r27)
    lhz r0, 0x0(r28)
    rlwinm r3, r0, 0, 27, 15
    lwz r0, 0x26c(r1)
    or r0, r3, r0
    sth r0, 0x0(r28)
    lhz r0, 0x0(r30)
    rlwinm r0, r0, 0, 17, 15
    sth r0, 0x0(r30)
    lhz r0, 0x0(r30)
    clrrwi r0, r0, 10
    nop
    sth r0, 0x0(r30)
    lhz r0, 0x0(r29)
    rlwinm r0, r0, 0, 27, 15
    ori r0, r0, 0x20
    sth r0, 0x0(r29)
    @L_800AE37C:
    lhz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_800AE37C
    lhz r4, 0x0(r31)
    li r0, -0x89
    lis r3, 0x200
    and r0, r4, r0
    ori r0, r0, 0x20
    sth r0, 0x0(r31)
    addis r19, r19, 0x200
    lhz r0, 0x270(r1)
    stw r3, lbl_8047A914(r13)
    ori r0, r0, 0x20
    sth r0, 0x270(r1)
    @L_800AE3B4:
    lwz r3, 0x248(r1)
    lhz r0, 0x0(r3)
    clrrwi r3, r0, 6
    lhz r0, 0x270(r1)
    or r0, r3, r0
    lwz r3, 0x248(r1)
    sth r0, 0x0(r3)
    @L_800AE3D0:
    lis r3, 0xc000
    stw r19, 0xd0(r3)
    stw r19, lbl_8047A90C(r13)
    lwz r0, 0x2c4(r1)
    lmw r14, 0x278(r1)
    addi r1, r1, 0x2c0
    mtlr r0
    blr
