    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x1
    li r4, 0x0
    stw r0, 0x14(r1)
    bl fn_802271E0
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
