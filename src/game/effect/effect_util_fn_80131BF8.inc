    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047AE4C(r13)
    li r4, 0x2
    bl fn_80131CE8
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
