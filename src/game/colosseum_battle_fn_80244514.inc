    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x54(r1)
    stmw r22, 0x28(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r4, r27
    addi r5, r1, 0x8
    li r31, 0x0
    li r3, 0x0
    li r6, 0x0
    bl fn_801F1C18
    mr r25, r3
    mr r3, r27
    mr r4, r30
    bl fn_80236520
    addi r24, r1, 0x8
    mr r23, r3
    clrlwi r26, r25, 16
    li r22, 0x0
    b @L_802445F4
@L_80244574:
    clrlslwi r25, r22, 16, 2
    mr r3, r27
    lwzx r4, r24, r25
    bl fn_8023715C
    lwzx r4, r24, r25
    mr r25, r3
    mr r3, r27
    bl fn_80236FFC
    clrlwi r4, r25, 16
    clrlwi r0, r3, 16
    cmplw r4, r0
    bge @L_802445F0
    mr r4, r27
    li r3, 0x0
    li r5, 0x164
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x164
    bl fn_80239EE8
    b @L_80244600
@L_802445F0:
    addi r22, r22, 0x1
@L_802445F4:
    clrlwi r0, r22, 16
    cmplw r0, r26
    blt @L_80244574
@L_80244600:
    clrlwi. r0, r23, 16
    beq @L_80244688
    cmplwi r0, 0xffff
    beq @L_80244688
    cmplwi r0, 0x165
    beq @L_80244688
    cmplwi r0, 0x163
    beq @L_80244688
    mr r3, r27
    mr r4, r23
    mr r5, r30
    bl fn_802395C8
    bl fn_8010C4A0
    clrlwi r0, r3, 24
    cmplwi r0, 0x2
    bne @L_80244688
    mr r3, r31
    mr r4, r27
    li r5, 0x165
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x165
    bl fn_80239EE8
@L_80244688:
    mr r3, r31
    lmw r22, 0x28(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
