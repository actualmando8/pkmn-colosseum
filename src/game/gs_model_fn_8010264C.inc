    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    bl fn_801046B8
    mr r4, r3
    mr r3, r30
    mr r7, r31
    li r5, 0x0
    li r6, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
