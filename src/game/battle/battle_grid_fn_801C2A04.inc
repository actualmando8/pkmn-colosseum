    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lwz r0, lbl_8047B390(r13)
    cmpwi r0, 0x0
    beq @L_801C2A4C
    lwz r0, lbl_8047B38C(r13)
    cmpwi r0, 0x0
    bne @L_801C2A4C
    lwz r31, lbl_8047B388(r13)
    b @L_801C2A44
    @L_801C2A34:
    lwz r12, 0x4(r31)
    mtctr r12
    bctrl
    lwz r31, 0x0(r31)
    @L_801C2A44:
    cmplwi r31, 0x0
    bne @L_801C2A34
    @L_801C2A4C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
