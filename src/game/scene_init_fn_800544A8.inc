    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80054530
    bge @L_800544DC
    cmpwi r0, 0x0
    beq @L_800544E8
    b @L_80054634
    @L_800544DC:
    cmpwi r0, 0x4
    bge @L_80054634
    b @L_80054610
    @L_800544E8:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80054634
    lwz r0, lbl_8047A544(r13)
    lfs f1, lbl_8047BE68(r2)
    lfs f0, lbl_8047BE60(r2)
    cmpwi r0, 0x0
    stfs f1, lbl_8047A550(r13)
    stfs f0, lbl_8047A54C(r13)
    stfs f1, lbl_8047A540(r13)
    beq @L_80054524
    li r3, 0x8f
    li r4, 0xff
    bl fn_801080CC
    @L_80054524:
    li r0, 0x1
    stb r0, 0x2(r31)
    b @L_80054634
    @L_80054530:
    lfs f3, lbl_8047A554(r13)
    lfs f2, lbl_8047BE68(r2)
    fcmpo cr0, f3, f2
    ble @L_80054564
    lfs f1, lbl_8047A558(r13)
    lfs f0, lbl_8047BE60(r2)
    fadds f1, f1, f3
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A558(r13)
    cror eq, gt, eq
    bne @L_80054564
    stfs f0, lbl_8047A558(r13)
    stfs f2, lbl_8047A554(r13)
    @L_80054564:
    lfs f2, lbl_8047A554(r13)
    lfs f1, lbl_8047BE68(r2)
    fcmpo cr0, f2, f1
    bge @L_80054594
    lfs f0, lbl_8047A558(r13)
    fadds f0, f0, f2
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A558(r13)
    cror eq, lt, eq
    bne @L_80054594
    stfs f1, lbl_8047A558(r13)
    stfs f1, lbl_8047A554(r13)
    @L_80054594:
    lfs f2, lbl_8047A550(r13)
    lfs f1, lbl_8047BE88(r2)
    lfs f0, lbl_8047BE60(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A550(r13)
    cror eq, gt, eq
    bne @L_800545BC
    fsubs f0, f1, f0
    stfs f0, lbl_8047A550(r13)
    @L_800545BC:
    lfs f2, lbl_8047A54C(r13)
    lfs f1, lbl_8047BE60(r2)
    fcmpo cr0, f2, f1
    bge @L_800545E4
    lfs f0, lbl_8047BE88(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A54C(r13)
    ble @L_800545E4
    stfs f1, lbl_8047A54C(r13)
    @L_800545E4:
    lfs f2, lbl_8047A540(r13)
    lfs f1, lbl_8047BE88(r2)
    lfs f0, lbl_8047BE60(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A540(r13)
    cror eq, gt, eq
    bne @L_80054634
    fsubs f0, f1, f0
    stfs f0, lbl_8047A540(r13)
    b @L_80054634
    @L_80054610:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80054634
    li r3, 0x8f
    li r4, 0x103
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    @L_80054634:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
