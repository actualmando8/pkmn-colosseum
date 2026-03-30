    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lbz r5, 0x8b(r3)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r31, r5, r0
    li r6, 0x3cc6
    mr r5, r31
    bl fn_800FB680
    li r3, 0x0
    li r4, 0xd
    li r5, 0x0
    bl fn_8012A5B0
    mr r0, r3
    li r3, 0x34
    mr r4, r0
    bl fn_80132A38
    lha r4, 0x54(r30)
    mr r7, r31
    lha r6, 0x56(r30)
    li r3, 0x0
    addi r0, r4, 0x2
    li r4, 0x0
    li r8, 0x3cd3
    extsh r5, r0
    bl fn_800FBB34
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
