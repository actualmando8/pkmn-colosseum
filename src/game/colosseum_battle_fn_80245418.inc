    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x94(r1)
    stmw r24, 0x70(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    mr r24, r6
    mr r4, r29
    addi r5, r1, 0x8
    li r26, 0x0
    li r3, 0x0
    li r6, 0x0
    bl fn_801F1A6C
    mr r28, r3
    mr r3, r29
    mr r4, r24
    bl fn_80237310
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802454B8
    mr r4, r29
    li r3, 0x0
    li r5, 0x14c
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x14c
    bl fn_80239EE8
@L_802454B8:
    addi r27, r1, 0x8
    clrlwi r28, r28, 16
    li r25, 0x0
    b @L_80245554
@L_802454C8:
    mr r3, r24
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    clrlslwi r0, r25, 16, 2
    lwzx r4, r27, r0
    cmplw r3, r4
    beq @L_80245550
    mr r3, r29
    li r5, 0x8
    bl fn_802384B4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80245550
    mr r3, r26
    mr r4, r29
    li r5, 0x14d
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x14d
    bl fn_80239EE8
    b @L_80245560
@L_80245550:
    addi r25, r25, 0x1
@L_80245554:
    clrlwi r0, r25, 16
    cmplw r0, r28
    blt @L_802454C8
@L_80245560:
    mr r3, r26
    lmw r24, 0x70(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
