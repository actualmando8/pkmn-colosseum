    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r0, 0x1
    li r3, 0x393
    stb r0, lbl_8047A439(r13)
    bl fn_800FF730
    bl fn_800F0308
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
