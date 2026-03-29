    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    mr r9, r4
    mr r8, r5
    mr r7, r6
    lis r4, lbl_803A1D40@ha
    mr r6, r8
    addi r5, r4, lbl_803A1D40@l
    li r0, 0x1
    stb r0, 0x4(r5)
    mr r4, r3
    mr r5, r9
    li r3, 0x5
    li r8, 0x1
    bl fn_8001BAC4
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
