    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r7, 0x0
    stw r0, 0x54(r1)
    stmw r26, 0x38(r1)
    mr r31, r6
    mr r26, r4
    mr r29, r5
    mr r30, r3
    mr r4, r31
    addi r5, r1, 0x1c
    addi r6, r1, 0x8
    bl fn_802367CC
    mr r27, r3
    mr r3, r30
    mr r4, r31
    bl fn_802364BC
    mr r28, r3
    mr r3, r30
    mr r4, r26
    mr r5, r29
    mr r6, r31
    li r7, 0x0
    bl fn_8025C264
    mr r29, r3
    addi r7, r1, 0x8
    clrlwi r6, r28, 16
    addi r5, r1, 0x1c
    clrlwi r0, r27, 16
    li r3, 0x0
    li r10, 0x0
    b @L_80258688
@L_80258654:
    clrlslwi r8, r10, 16, 1
    lhax r9, r7, r8
    extsh. r4, r9
    blt @L_80258684
    lhzx r4, r5, r8
    cmplw r6, r4
    bne @L_80258684
    mr r3, r30
    mr r4, r31
    clrlwi r5, r9, 24
    bl fn_802381C4
    b @L_80258694
@L_80258684:
    addi r10, r10, 0x1
@L_80258688:
    clrlwi r4, r10, 16
    cmplw r4, r0
    blt @L_80258654
@L_80258694:
    clrlwi. r0, r3, 24
    bne @L_802586A4
    li r3, 0x0
    b @L_802586E0
@L_802586A4:
    mr r3, r30
    mr r4, r31
    li r5, 0x29
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802586C8
    li r3, 0x0
    b @L_802586E0
@L_802586C8:
    cmpwi r29, 0x0
    bne @L_802586D8
    li r3, 0x0
    b @L_802586E0
@L_802586D8:
    cmpwi r29, -0x1
    li r3, 0x1
@L_802586E0:
    lmw r26, 0x38(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
