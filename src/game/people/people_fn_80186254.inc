    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lfs f1, lbl_8047D7EC(r2)
    li r5, 0x0
    stw r0, 0x14(r1)
    li r6, 0x64
    lfs f2, lbl_8047D838(r2)
    bl fn_80186284
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
