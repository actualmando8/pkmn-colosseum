    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_8047ABDC(r13)
    bl fn_8009B614
    li r0, 0x0
    stw r0, lbl_8047ABDC(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
