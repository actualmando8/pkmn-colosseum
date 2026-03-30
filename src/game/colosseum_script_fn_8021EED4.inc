    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r4, 0x0
    li r5, 0xff
    stw r0, 0x14(r1)
    li r6, 0x0
    lwz r3, lbl_8047B610(r13)
    lhz r3, 0x1(r3)
    bl fn_80166A50
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x3
    stw r0, lbl_8047B610(r13)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
