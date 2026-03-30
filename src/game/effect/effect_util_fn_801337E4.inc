    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    li r0, 0x0
    lwz r3, lbl_80478848(r13)
    stb r0, lbl_8047AED1(r13)
    bl fn_80102510
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
