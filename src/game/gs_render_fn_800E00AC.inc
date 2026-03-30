    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lfs f0, lbl_8047CAD8(r2)
    stw r0, 0x14(r1)
    mr r0, r3
    fdivs f1, f0, f1
    mr r3, r4
    mr r4, r0
    bl fn_800A3AC0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
