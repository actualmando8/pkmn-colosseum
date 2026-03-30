    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stmw r21, 0x94(r1)
    mr r26, r4
    mr r27, r5
    mr r25, r3
    li r28, 0x0
    li r4, 0x0
    li r5, 0x0
    bl fn_80235B04
    mr r29, r3
    mr r4, r25
    addi r5, r1, 0x28
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1A6C
    mr r30, r3
    mr r4, r25
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    clrlwi r0, r29, 24
    mr r31, r3
    cmplwi r0, 0x3
    beq @L_80244A48
    mr r4, r25
    li r3, 0x0
    li r5, 0x15b
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r25
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x15b
    bl fn_80239EE8
@L_80244A48:
    addi r22, r1, 0x28
    clrlwi r24, r30, 16
    li r21, 0x0
    b @L_80244B1C
@L_80244A58:
    clrlslwi r23, r21, 16, 2
    mr r3, r25
    lwzx r4, r22, r23
    li r5, 0x5
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80244ACC
    lwzx r4, r22, r23
    mr r3, r25
    li r5, 0x4
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80244ACC
    lwzx r4, r22, r23
    mr r3, r25
    li r5, 0x8
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80244ACC
    lwzx r4, r22, r23
    mr r3, r25
    li r5, 0x8
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80244B18
@L_80244ACC:
    mr r3, r28
    mr r4, r25
    li r5, 0x15c
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r25
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x15c
    bl fn_80239EE8
    b @L_80244B28
@L_80244B18:
    addi r21, r21, 0x1
@L_80244B1C:
    clrlwi r0, r21, 16
    cmplw r0, r24
    blt @L_80244A58
@L_80244B28:
    addi r24, r1, 0x8
    clrlwi r31, r31, 16
    li r22, 0x0
    b @L_80244BA4
@L_80244B38:
    clrlslwi r0, r22, 16, 2
    mr r3, r25
    lwzx r4, r24, r0
    bl fn_802377E8
    clrlwi r0, r3, 16
    cmplwi r0, 0x12f
    bne @L_80244BA0
    mr r3, r28
    mr r4, r25
    li r5, 0x15d
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r25
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x15d
    bl fn_80239EE8
    b @L_80244BB0
@L_80244BA0:
    addi r22, r22, 0x1
@L_80244BA4:
    clrlwi r0, r22, 16
    cmplw r0, r31
    blt @L_80244B38
@L_80244BB0:
    addi r31, r1, 0x28
    clrlwi r30, r30, 16
    li r22, 0x0
    b @L_80244C2C
@L_80244BC0:
    clrlslwi r0, r22, 16, 2
    mr r3, r25
    lwzx r4, r31, r0
    bl fn_80238980
    clrlwi r0, r3, 16
    cmplwi r0, 0x181
    bne @L_80244C28
    mr r3, r28
    mr r4, r25
    li r5, 0x15e
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r25
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x15e
    bl fn_80239EE8
    b @L_80244C38
@L_80244C28:
    addi r22, r22, 0x1
@L_80244C2C:
    clrlwi r0, r22, 16
    cmplw r0, r30
    blt @L_80244BC0
@L_80244C38:
    clrlwi r0, r29, 24
    cmplwi r0, 0x3
    bne @L_80244C8C
    mr r3, r28
    mr r4, r25
    li r5, 0x15f
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r25
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x15f
    bl fn_80239EE8
@L_80244C8C:
    mr r3, r28
    lmw r21, 0x94(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
