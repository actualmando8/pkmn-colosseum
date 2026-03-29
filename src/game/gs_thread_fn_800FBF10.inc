    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_80478B08(r13)
    lbz r0, 0x1d(r3)
    extsb r0, r0
    slwi r0, r0, 2
    add r3, r3, r0
    lwz r3, 0xc(r3)
    bl fn_800EF504
    lwz r3, lbl_80478B08(r13)
    li r4, 0x2
    li r0, 0x1
    sth r4, 0x18(r3)
    lwz r3, lbl_80478B08(r13)
    sth r0, 0x1a(r3)
    lwz r3, lbl_80478B08(r13)
    lbz r0, 0x1d(r3)
    xori r0, r0, 0x1
    extsb r0, r0
    stb r0, 0x1d(r3)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
