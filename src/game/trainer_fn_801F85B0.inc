    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr. r29, r3
    mr r30, r4
    bne @L_801F85D4
    li r3, -0x1
    b @L_801F8624
    @L_801F85D4:
    cmplwi r30, 0x0
    bne @L_801F85E4
    li r3, -0x1
    b @L_801F8624
    @L_801F85E4:
    li r31, 0x0
    b @L_801F8614
    @L_801F85EC:
    mr r3, r29
    clrlwi r6, r31, 16
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    cmplw r30, r3
    bne @L_801F8610
    mr r3, r31
    b @L_801F8624
    @L_801F8610:
    addi r31, r31, 0x1
    @L_801F8614:
    extsh r0, r31
    cmpwi r0, 0x2
    blt @L_801F85EC
    li r3, -0x1
    @L_801F8624:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
