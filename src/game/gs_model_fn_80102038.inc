    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    fmr f2, f1
    lfs f1, lbl_8047CDC0(r13)
    bl fn_801096AC
    li r3, 0x1
    bl fn_80109664
    bl fn_80109764
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
