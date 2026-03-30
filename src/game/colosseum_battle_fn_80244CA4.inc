    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stmw r22, 0x88(r1)
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
    cmplwi r0, 0x4
    beq @L_80244D60
    mr r4, r25
    li r3, 0x0
    li r5, 0x156
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
    li r10, 0x156
    bl fn_80239EE8
@L_80244D60:
    addi r23, r1, 0x28
    clrlwi r24, r30, 16
    li r22, 0x0
    b @L_80244DE0
@L_80244D70:
    clrlslwi r0, r22, 16, 2
    mr r3, r25
    lwzx r4, r23, r0
    li r5, 0xf
    bl fn_80238E30
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80244DDC
    mr r3, r28
    mr r4, r25
    li r5, 0x157
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
    li r10, 0x157
    bl fn_80239EE8
    b @L_80244DEC
@L_80244DDC:
    addi r22, r22, 0x1
@L_80244DE0:
    clrlwi r0, r22, 16
    cmplw r0, r24
    blt @L_80244D70
@L_80244DEC:
    addi r24, r1, 0x8
    clrlwi r31, r31, 16
    li r23, 0x0
    b @L_80244E68
@L_80244DFC:
    clrlslwi r0, r23, 16, 2
    mr r3, r25
    lwzx r4, r24, r0
    bl fn_802377E8
    clrlwi r0, r3, 16
    cmplwi r0, 0x12f
    bne @L_80244E64
    mr r3, r28
    mr r4, r25
    li r5, 0x158
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
    li r10, 0x158
    bl fn_80239EE8
    b @L_80244E74
@L_80244E64:
    addi r23, r23, 0x1
@L_80244E68:
    clrlwi r0, r23, 16
    cmplw r0, r31
    blt @L_80244DFC
@L_80244E74:
    addi r31, r1, 0x28
    clrlwi r30, r30, 16
    li r23, 0x0
    b @L_80244EF0
@L_80244E84:
    clrlslwi r0, r23, 16, 2
    mr r3, r25
    lwzx r4, r31, r0
    bl fn_80238980
    clrlwi r0, r3, 16
    cmplwi r0, 0x181
    bne @L_80244EEC
    mr r3, r28
    mr r4, r25
    li r5, 0x159
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
    li r10, 0x159
    bl fn_80239EE8
    b @L_80244EFC
@L_80244EEC:
    addi r23, r23, 0x1
@L_80244EF0:
    clrlwi r0, r23, 16
    cmplw r0, r30
    blt @L_80244E84
@L_80244EFC:
    clrlwi r0, r29, 24
    cmplwi r0, 0x4
    bne @L_80244F50
    mr r3, r28
    mr r4, r25
    li r5, 0x15a
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
    li r10, 0x15a
    bl fn_80239EE8
@L_80244F50:
    mr r3, r28
    lmw r22, 0x88(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
