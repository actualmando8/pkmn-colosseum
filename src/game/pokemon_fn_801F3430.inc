    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r5
    mr r31, r3
    bl fn_802062FC
    clrlwi. r0, r3, 24
    bne @L_801F345C
    li r3, 0x1
    b @L_801F3488
    @L_801F345C:
    lwz r0, 0x0(r30)
    mr r3, r31
    clrlwi r4, r0, 16
    bl fn_802026E4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_801F3484
    lwz r3, 0x4(r30)
    addi r0, r3, 0x1
    stw r0, 0x4(r30)
    @L_801F3484:
    li r3, 0x1
    @L_801F3488:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
