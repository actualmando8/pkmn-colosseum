    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x5
    li r4, 0x0
    stw r0, 0x14(r1)
    li r0, 0x0
    li r5, 0x0
    li r6, 0x0
    stb r0, lbl_80478820(r13)
    li r7, 0x1
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x5
    bl fn_80102510
    li r0, 0x1
    li r3, 0x0
    stb r0, lbl_80478820(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
