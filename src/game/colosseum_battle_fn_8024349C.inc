    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    mr r31, r3
    mr r24, r4
    mr r25, r5
    mr r26, r6
    mr r4, r31
    addi r5, r1, 0x8
    li r28, 0x0
    li r3, 0x0
    li r6, 0x1
    bl fn_801F1C18
    addi r29, r1, 0x8
    clrlwi r30, r3, 16
    li r27, 0x0
    b @L_80243510
@L_802434E8:
    clrlslwi r0, r27, 16, 2
    mr r3, r31
    lwzx r5, r29, r0
    mr r4, r26
    bl fn_80236D60
    cmpwi r3, 0x0
    ble @L_8024350C
    li r28, 0x1
    b @L_8024351C
@L_8024350C:
    addi r27, r27, 0x1
@L_80243510:
    clrlwi r0, r27, 16
    cmplw r0, r30
    blt @L_802434E8
@L_8024351C:
    clrlwi r0, r28, 24
    cmplwi r0, 0x1
    bne @L_80243574
    mr r4, r31
    li r3, 0x0
    li r5, 0x181
    bl fn_80239984
    mr r0, r3
    mr r3, r24
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r25
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x181
    bl fn_80239EE8
    b @L_802435BC
@L_80243574:
    mr r4, r31
    li r3, 0x0
    li r5, 0x182
    bl fn_80239984
    mr r0, r3
    mr r3, r24
    mr r29, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r25
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x182
    bl fn_80239EE8
@L_802435BC:
    mr r3, r29
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
