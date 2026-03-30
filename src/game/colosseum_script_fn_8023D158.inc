    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r22, 0x28(r1)
    mr r22, r6
    mr r28, r4
    mr r27, r3
    mr r29, r5
    mr r4, r22
    li r30, 0x0
    bl fn_80236FFC
    mr r23, r3
    mr r3, r27
    mr r4, r22
    bl fn_8023715C
    mr r24, r3
    mr r3, r27
    mr r4, r22
    bl fn_80236520
    mr r31, r3
    mr r4, r27
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    addi r25, r1, 0x8
    rlwinm r26, r3, 0, 16, 31
    li r22, 0x0
    b @L_8023D254
@L_8023D1D0:
    clrlslwi r0, r22, 16, 2
    mr r3, r27
    lwzx r4, r25, r0
    bl fn_802377E8
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xca
    beq @L_8023D204
    cmplwi r0, 0x168
    beq @L_8023D204
    cmplwi r0, 0x12f
    beq @L_8023D204
    cmplwi r0, 0xd5
    bne @L_8023D250
@L_8023D204:
    mr r4, r27
    li r3, 0x0
    li r5, 0x217
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x217
    bl fn_80239EE8
    b @L_8023D260
@L_8023D250:
    addi r22, r22, 0x1
@L_8023D254:
    rlwinm r0, r22, 0, 16, 31
    cmplw r0, r26
    blt @L_8023D1D0
@L_8023D260:
    rlwinm r3, r24, 0, 16, 31
    rlwinm r0, r23, 0, 16, 31
    cmplw r3, r0
    ble @L_8023D2B8
    mr r3, r30
    mr r4, r27
    li r5, 0x218
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x218
    bl fn_80239EE8
@L_8023D2B8:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x11f
    bne @L_8023D30C
    mr r3, r30
    mr r4, r27
    li r5, 0x219
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x219
    bl fn_80239EE8
@L_8023D30C:
    mr r3, r30
    lmw r22, 0x28(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
