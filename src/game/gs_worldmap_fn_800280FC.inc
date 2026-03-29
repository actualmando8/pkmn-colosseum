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
    beq @L_80028178
    bge @L_80028138
    cmpwi r0, 0x0
    beq @L_80028144
    b @L_800281D4
    @L_80028138:
    cmpwi r0, 0x4
    bge @L_800281D4
    b @L_800281B0
    @L_80028144:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_800281D4
    lwz r3, 0x4(r30)
    li r4, 0x56
    bl fn_801080CC
    lfs f0, lbl_8047B930(r2)
    li r0, 0x1
    lwz r3, 0x30(r31)
    stfs f0, 0x0(r3)
    stb r0, 0x2(r30)
    b @L_800281D4
    @L_80028178:
    lwz r3, 0x30(r31)
    lfs f0, lbl_8047B950(r2)
    lfs f2, 0x0(r3)
    lfs f1, lbl_8047B934(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, 0x0(r3)
    cror eq, gt, eq
    bne @L_800281D4
    lwz r3, 0x30(r31)
    lfs f0, 0x0(r3)
    fsubs f0, f0, f1
    stfs f0, 0x0(r3)
    b @L_800281D4
    @L_800281B0:
    lbz r0, 0x2(r30)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_800281D4
    lwz r3, 0x4(r30)
    li r4, 0x5a
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r30)
    @L_800281D4:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
