    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r5, 0x3
    mr r6, r7
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    mr r3, r31
    mr r4, r30
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r31
    mr r4, r30
    li r5, 0x3
    li r6, 0x0
    bl fn_802085C4
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
