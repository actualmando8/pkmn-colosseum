    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r24, 0x50(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    bl fn_80235974
    mr r31, r3
    mr r3, r27
    mr r4, r28
    li r30, 0x0
    bl fn_80235714
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8023E548
    mr r4, r27
    li r3, 0x0
    li r5, 0x1f4
    bl fn_80239984
    mr r30, r3
    mr r3, r28
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1f4
    bl fn_80239EE8
@L_8023E548:
    mr r4, r27
    addi r5, r1, 0x10
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1C18
    addi r25, r1, 0x10
    rlwinm r26, r3, 0, 16, 31
    li r24, 0x0
    b @L_8023E5D4
@L_8023E570:
    clrlslwi r0, r24, 16, 2
    lwzx r4, r25, r0
    cmplw r28, r4
    beq @L_8023E5D0
    mr r3, r27
    addi r5, r1, 0x30
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    rlwinm. r4, r3, 0, 16, 31
    beq @L_8023E5D0
    addi r3, r1, 0x30
    li r5, 0x0
    b @L_8023E5C4
@L_8023E5A8:
    clrlslwi r0, r5, 16, 1
    lhzx r0, r3, r0
    cmplwi r0, 0x10a
    bne @L_8023E5C0
    li r0, 0x1
    b @L_8023E5E4
@L_8023E5C0:
    addi r5, r5, 0x1
@L_8023E5C4:
    rlwinm r0, r5, 0, 16, 31
    cmplw r0, r4
    blt @L_8023E5A8
@L_8023E5D0:
    addi r24, r24, 0x1
@L_8023E5D4:
    rlwinm r0, r24, 0, 16, 31
    cmplw r0, r26
    blt @L_8023E570
    li r0, 0x0
@L_8023E5E4:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023E634
    mr r3, r30
    mr r4, r27
    li r5, 0x1f5
    bl fn_80239984
    mr r30, r3
    mr r3, r28
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1f5
    bl fn_80239EE8
@L_8023E634:
    mr r3, r27
    mr r4, r28
    bl fn_80235714
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023E690
    mr r3, r30
    mr r4, r27
    li r5, 0x1f6
    bl fn_80239984
    mr r30, r3
    mr r3, r28
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1f6
    bl fn_80239EE8
@L_8023E690:
    rlwinm r3, r31, 0, 24, 31
    subic. r25, r3, 0x6
    bge @L_8023E6A0
    li r25, 0x0
@L_8023E6A0:
    li r3, 0x0
    li r4, 0x1f7
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    mullw r25, r25, r3
    mr r3, r30
    mr r4, r25
    bl fn_802399FC
    mr r24, r3
    mr r3, r28
    bl fn_80205B8C
    lis r6, 0x1
    stw r25, 0x8(r1)
    mr r5, r3
    mr r4, r27
    subi r3, r6, 0x139c
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1f7
    bl fn_80239CCC
    mr r3, r24
    lmw r24, 0x50(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
