    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r29, r6
    mr r27, r4
    mr r26, r3
    mr r28, r5
    mr r4, r29
    bl fn_80235AA0
    mr r30, r3
    mr r3, r26
    mr r4, r29
    bl fn_802359D8
    mr r31, r3
    mr r3, r26
    mr r4, r29
    mr r5, r28
    bl fn_8025CAA8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8025554C
    li r3, 0x0
    b @L_802555E4
@L_8025554C:
    clrlwi. r0, r30, 24
    bne @L_80255564
    clrlwi. r0, r31, 24
    bne @L_80255564
    li r3, 0x0
    b @L_802555E4
@L_80255564:
    mr r3, r26
    mr r4, r27
    mr r5, r29
    mr r6, r28
    li r7, 0xa0
    li r8, 0x1
    li r9, 0x1
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_802555BC
    mr r3, r26
    mr r4, r27
    mr r5, r29
    mr r6, r28
    li r7, 0xa0
    li r8, 0x4
    li r9, 0x1
    bl fn_8025C808
    clrlwi. r0, r3, 24
    bne @L_802555BC
    li r3, 0x0
    b @L_802555E4
@L_802555BC:
    mr r3, r26
    mr r4, r29
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802555E0
    li r3, 0x0
    b @L_802555E4
@L_802555E0:
    li r3, 0x1
@L_802555E4:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
