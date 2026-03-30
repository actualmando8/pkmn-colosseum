    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lfs f1, lbl_8047BA30(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
