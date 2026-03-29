    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    lwz r4, 0x60(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8004D2D8
    bge @L_8004D2A4
    cmpwi r0, 0x0
    beq @L_8004D2B0
    b @L_8004D334
    @L_8004D2A4:
    cmpwi r0, 0x4
    bge @L_8004D334
    b @L_8004D310
    @L_8004D2B0:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8004D334
    lwz r3, 0x4(r31)
    li r4, 0x1c2
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    b @L_8004D334
    @L_8004D2D8:
    lwz r3, 0x0(r4)
    lfs f0, lbl_8047BE18(r2)
    lfs f2, 0x0(r3)
    lfs f1, lbl_8047BE1C(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, 0x0(r3)
    cror eq, gt, eq
    bne @L_8004D334
    lwz r3, 0x0(r4)
    lfs f0, 0x0(r3)
    fsubs f0, f0, f1
    stfs f0, 0x0(r3)
    b @L_8004D334
    @L_8004D310:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8004D334
    lwz r3, 0x4(r31)
    li r4, 0x1c6
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    @L_8004D334:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
