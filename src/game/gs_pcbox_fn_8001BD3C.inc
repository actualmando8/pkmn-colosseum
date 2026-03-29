    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r7, r6
    lis r6, lbl_803A1D40@ha
    li r0, 0x1
    addi r8, r6, lbl_803A1D40@l
    stb r0, 0x4(r8)
    mr r6, r5
    mr r8, r4
    li r4, 0x0
    li r5, 0x0
    bl fn_8001BAC4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
