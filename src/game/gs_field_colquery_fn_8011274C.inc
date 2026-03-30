    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r3, 0x1
    bl fn_801C40F0
    li r0, 0x0
    li r3, 0x1
    stb r0, lbl_80478DD0(r13)
    bl fn_800D3074
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

