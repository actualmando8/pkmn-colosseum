    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x1(r30)
    lwz r31, 0x60(r30)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_8002C17C
    bge @L_8002C120
    cmpwi r0, 0x0
    beq @L_8002C12C
    b @L_8002C268
    @L_8002C120:
    cmpwi r0, 0x4
    bge @L_8002C268
    b @L_8002C244
    @L_8002C12C:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8002C268
    li r3, 0x60
    li r4, 0x76
    bl fn_801080CC
    lfs f0, lbl_8047B980(r2)
    li r4, 0x0
    lwz r3, 0xc(r31)
    li r0, 0x1
    stfs f0, 0x0(r3)
    lwz r3, 0x14(r31)
    stw r4, 0x0(r3)
    lwz r3, 0x18(r31)
    stfs f0, 0x0(r3)
    lwz r3, 0x10(r31)
    stfs f0, 0x0(r3)
    stb r0, 0x2(r30)
    b @L_8002C268
    @L_8002C17C:
    lwz r3, 0xc(r31)
    lfs f1, lbl_8047B980(r2)
    lfs f2, 0x0(r3)
    fcmpo cr0, f2, f1
    ble @L_8002C1AC
    lfs f0, lbl_8047B9C0(r2)
    fsubs f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, 0x0(r3)
    bge @L_8002C1AC
    lwz r3, 0xc(r31)
    stfs f1, 0x0(r3)
    @L_8002C1AC:
    lwz r3, 0xc(r31)
    lfs f1, lbl_8047B980(r2)
    lfs f2, 0x0(r3)
    fcmpo cr0, f2, f1
    bge @L_8002C1DC
    lfs f0, lbl_8047B9C0(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, 0x0(r3)
    ble @L_8002C1DC
    lwz r3, 0xc(r31)
    stfs f1, 0x0(r3)
    @L_8002C1DC:
    lwz r3, 0x18(r31)
    lfs f1, lbl_8047B9C4(r2)
    lfs f2, 0x0(r3)
    lfs f0, lbl_8047B97C(r2)
    fadds f1, f2, f1
    stfs f1, 0x0(r3)
    lwz r3, 0x18(r31)
    lfs f1, 0x0(r3)
    fcmpo cr0, f1, f0
    ble @L_8002C20C
    lfs f0, lbl_8047B980(r2)
    stfs f0, 0x0(r3)
    @L_8002C20C:
    lwz r3, 0x10(r31)
    lfs f0, lbl_8047B9C8(r2)
    lfs f2, 0x0(r3)
    lfs f1, lbl_8047B97C(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, 0x0(r3)
    cror eq, gt, eq
    bne @L_8002C268
    lwz r3, 0x10(r31)
    lfs f0, 0x0(r3)
    fsubs f0, f0, f1
    stfs f0, 0x0(r3)
    b @L_8002C268
    @L_8002C244:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8002C268
    li r3, 0x60
    li r4, 0x7a
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r30)
    @L_8002C268:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
