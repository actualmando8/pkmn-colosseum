    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stmw r22, 0x68(r1)
    mr r27, r4
    mr r28, r5
    mr r26, r3
    li r29, 0x0
    li r4, 0x0
    li r5, 0x0
    bl fn_80235B04
    mr r30, r3
    mr r4, r26
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1A6C
    clrlwi r0, r30, 24
    mr r31, r3
    cmplwi r0, 0x1
    beq @L_80245008
    mr r4, r26
    li r3, 0x0
    li r5, 0x152
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x152
    bl fn_80239EE8
@L_80245008:
    addi r23, r1, 0x8
    clrlwi r25, r31, 16
    li r22, 0x0
    b @L_802450C0
@L_80245018:
    clrlslwi r24, r22, 16, 2
    mr r3, r26
    lwzx r4, r23, r24
    li r5, 0xa
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80245070
    lwzx r4, r23, r24
    mr r3, r26
    li r5, 0xc
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80245070
    lwzx r4, r23, r24
    mr r3, r26
    li r5, 0x22
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802450BC
@L_80245070:
    mr r3, r29
    mr r4, r26
    li r5, 0x153
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x153
    bl fn_80239EE8
    b @L_802450CC
@L_802450BC:
    addi r22, r22, 0x1
@L_802450C0:
    clrlwi r0, r22, 16
    cmplw r0, r25
    blt @L_80245018
@L_802450CC:
    addi r25, r1, 0x8
    clrlwi r31, r31, 16
    li r23, 0x0
    b @L_80245148
@L_802450DC:
    clrlslwi r0, r23, 16, 2
    mr r3, r26
    lwzx r4, r25, r0
    bl fn_80238980
    clrlwi r0, r3, 16
    cmplwi r0, 0x181
    bne @L_80245144
    mr r3, r29
    mr r4, r26
    li r5, 0x154
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x154
    bl fn_80239EE8
    b @L_80245154
@L_80245144:
    addi r23, r23, 0x1
@L_80245148:
    clrlwi r0, r23, 16
    cmplw r0, r31
    blt @L_802450DC
@L_80245154:
    clrlwi r0, r30, 24
    cmplwi r0, 0x1
    bne @L_802451A8
    mr r3, r29
    mr r4, r26
    li r5, 0x155
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x155
    bl fn_80239EE8
@L_802451A8:
    mr r3, r29
    lmw r22, 0x68(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
