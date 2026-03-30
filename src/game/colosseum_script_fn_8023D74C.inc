    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r6, 0x0
    li r7, 0x1
    stw r0, 0x44(r1)
    stmw r26, 0x28(r1)
    mr r30, r5
    mr r28, r3
    mr r29, r4
    addi r5, r1, 0x10
    bl fn_802367CC
    mr r31, r3
    mr r3, r28
    mr r4, r29
    bl fn_80235A3C
    mr r27, r3
    mr r3, r28
    mr r4, r29
    li r26, 0x0
    bl fn_80235714
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023D7E8
    mr r4, r28
    li r3, 0x0
    li r5, 0x20c
    bl fn_80239984
    mr r26, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x20c
    bl fn_80239EE8
@L_8023D7E8:
    mr r3, r28
    mr r4, r29
    bl fn_80235714
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023D844
    mr r3, r26
    mr r4, r28
    li r5, 0x20e
    bl fn_80239984
    mr r26, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x20e
    bl fn_80239EE8
@L_8023D844:
    rlwinm r3, r27, 0, 24, 31
    subic. r27, r3, 0x6
    bge @L_8023D854
    li r27, 0x0
@L_8023D854:
    li r3, 0x0
    li r4, 0x20f
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    mullw r27, r27, r3
    mr r3, r26
    mr r4, r27
    bl fn_802399FC
    mr r26, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    stw r27, 0x8(r1)
    mr r5, r3
    mr r4, r28
    subi r3, r6, 0x139c
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x20f
    bl fn_80239CCC
    addi r4, r1, 0x10
    rlwinm r0, r31, 0, 16, 31
    li r5, 0x0
    b @L_8023D928
@L_8023D8C0:
    clrlslwi r3, r5, 16, 1
    lhzx r3, r4, r3
    cmplwi r3, 0xcd
    beq @L_8023D8D8
    cmplwi r3, 0x12d
    bne @L_8023D924
@L_8023D8D8:
    mr r3, r26
    mr r4, r28
    li r5, 0x20d
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x20d
    bl fn_80239EE8
    b @L_8023D934
@L_8023D924:
    addi r5, r5, 0x1
@L_8023D928:
    rlwinm r3, r5, 0, 16, 31
    cmplw r3, r0
    blt @L_8023D8C0
@L_8023D934:
    mr r3, r26
    lmw r26, 0x28(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
