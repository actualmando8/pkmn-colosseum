    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80014FB8
    bge @L_80014F7C
    cmpwi r0, 0x0
    beq @L_80014F88
    b @L_80015004
    @L_80014F7C:
    cmpwi r0, 0x4
    bge @L_80015004
    b @L_80014FE0
    @L_80014F88:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80015004
    li r3, 0x5a
    li r4, 0x66
    bl fn_801080CC
    lfs f0, lbl_8047B748(r2)
    li r0, 0x1
    stfs f0, lbl_8047A2C0(r13)
    stb r0, 0x2(r31)
    b @L_80015004
    @L_80014FB8:
    lfs f2, lbl_8047A2C0(r13)
    lfs f1, lbl_8047B74C(r2)
    lfs f0, lbl_8047B744(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A2C0(r13)
    ble @L_80015004
    lfs f0, lbl_8047B748(r2)
    stfs f0, lbl_8047A2C0(r13)
    b @L_80015004
    @L_80014FE0:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80015004
    li r3, 0x5a
    li r4, 0x6a
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    @L_80015004:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
