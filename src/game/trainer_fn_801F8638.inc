    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr. r29, r3
    mr r30, r4
    bne @L_801F865C
    li r3, -0x1
    b @L_801F86AC
    @L_801F865C:
    cmplwi r30, 0x0
    bne @L_801F866C
    li r3, -0x1
    b @L_801F86AC
    @L_801F866C:
    li r31, 0x0
    b @L_801F869C
    @L_801F8674:
    mr r3, r29
    clrlwi r6, r31, 16
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    cmplw r30, r3
    bne @L_801F8698
    mr r3, r31
    b @L_801F86AC
    @L_801F8698:
    addi r31, r31, 0x1
    @L_801F869C:
    extsh r0, r31
    cmpwi r0, 0x6
    blt @L_801F8674
    li r3, -0x1
    @L_801F86AC:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
