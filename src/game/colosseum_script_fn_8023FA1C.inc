    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r6, 0x0
    li r7, 0x1
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr r26, r5
    mr r31, r3
    mr r25, r4
    addi r5, r1, 0x8
    li r28, 0x0
    bl fn_802367CC
    addi r29, r1, 0x8
    rlwinm r30, r3, 0, 16, 31
    li r27, 0x0
    b @L_8023FAD4
@L_8023FA5C:
    clrlslwi r0, r27, 16, 1
    lhzx r4, r29, r0
    cmplwi r4, 0x10c
    beq @L_8023FAD0
    mr r3, r31
    mr r5, r25
    bl fn_802395C8
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd
    bne @L_8023FAD0
    mr r4, r31
    li r3, 0x0
    li r5, 0x1d2
    bl fn_80239984
    mr r0, r3
    mr r3, r25
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r26
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1d2
    bl fn_80239EE8
    b @L_8023FAE0
@L_8023FAD0:
    addi r27, r27, 0x1
@L_8023FAD4:
    rlwinm r0, r27, 0, 16, 31
    cmplw r0, r30
    blt @L_8023FA5C
@L_8023FAE0:
    mr r3, r31
    mr r4, r25
    li r5, 0x24
    bl fn_80236BFC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023FB44
    mr r3, r28
    mr r4, r31
    li r5, 0x1d3
    bl fn_80239984
    mr r0, r3
    mr r3, r25
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r26
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1d3
    bl fn_80239EE8
@L_8023FB44:
    mr r3, r28
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
