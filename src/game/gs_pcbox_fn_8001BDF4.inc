    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lis r6, lbl_803A1D40@ha
    li r0, 0x1
    addi r7, r6, lbl_803A1D40@l
    mr r6, r4
    stb r0, 0x4(r7)
    mr r7, r5
    li r4, 0x0
    li r5, 0x0
    li r8, 0x1
    bl fn_8001BAC4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
