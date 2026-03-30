    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r28, r5
    mr r31, r3
    lwz r0, 0x4(r5)
    lwz r30, 0x0(r5)
    clrlwi r29, r0, 16
    bl fn_802062FC
    clrlwi. r0, r3, 24
    bne @L_801F1FB4
    li r3, 0x1
    b @L_801F200C
    @L_801F1FB4:
    mr r3, r31
    mr r4, r30
    bl fn_80202B88
    clrlwi. r0, r3, 24
    bne @L_801F2008
    mr r3, r31
    li r4, 0x27
    bl fn_802026E4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F2008
    mr r3, r31
    bl fn_80205B8C
    mr r4, r29
    bl fn_80123B5C
    extsb. r0, r3
    blt @L_801F2008
    li r0, 0x1
    li r3, 0x0
    stw r0, 0x8(r28)
    b @L_801F200C
    @L_801F2008:
    li r3, 0x1
    @L_801F200C:
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
