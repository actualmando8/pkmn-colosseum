    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r31, r3
    lis r3, lbl_803A6818@ha
    addi r4, r3, lbl_803A6818@l
    lfs f0, 0x50(r4)
    lfs f2, 0x4c(r4)
    fcmpu cr0, f0, f2
    beq @L_80040094
    lfs f1, 0x3c(r4)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCB8(r2)
    fdivs f0, f1, f0
    ble @L_8004007C
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x4c(r4)
    ble @L_80040094
    stfs f0, 0x4c(r4)
    b @L_80040094
    @L_8004007C:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x4c(r4)
    bge @L_80040094
    stfs f0, 0x4c(r4)
    @L_80040094:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x58(r3)
    lfs f2, 0x54(r3)
    fcmpu cr0, f0, f2
    beq @L_800400F4
    lfs f1, 0x3c(r3)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCB8(r2)
    fdivs f0, f1, f0
    ble @L_800400DC
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x54(r3)
    ble @L_800400F4
    stfs f0, 0x54(r3)
    b @L_800400F4
    @L_800400DC:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x54(r3)
    bge @L_800400F4
    stfs f0, 0x54(r3)
    @L_800400F4:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x60(r3)
    lfs f2, 0x5c(r3)
    fcmpu cr0, f0, f2
    beq @L_80040154
    lfs f1, 0x3c(r3)
    fcmpo cr0, f0, f2
    lfs f0, lbl_8047BCC0(r2)
    fdivs f0, f1, f0
    ble @L_8004013C
    fadds f1, f2, f0
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x5c(r3)
    ble @L_80040154
    stfs f0, 0x5c(r3)
    b @L_80040154
    @L_8004013C:
    fsubs f1, f2, f0
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f1, f0
    stfs f1, 0x5c(r3)
    bge @L_80040154
    stfs f0, 0x5c(r3)
    @L_80040154:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x1dc(r3)
    lfs f1, 0x1e0(r3)
    fcmpu cr0, f0, f1
    beq @L_80040214
    fsubs f3, f1, f0
    lfs f2, lbl_8047BCC4(r2)
    lfs f1, 0x3c(r3)
    lfs f0, lbl_8047BC98(r2)
    fmuls f2, f2, f3
    fmuls f4, f2, f1
    fcmpo cr0, f4, f0
    ble @L_80040190
    fmr f4, f0
    @L_80040190:
    lfs f0, lbl_8047BCC8(r2)
    fcmpo cr0, f4, f0
    cror eq, lt, eq
    bne @L_800401A4
    fmr f4, f0
    @L_800401A4:
    lfs f1, 0x1dc(r3)
    lfs f0, lbl_8047BC94(r2)
    fadds f1, f1, f4
    lfs f3, 0x1e0(r3)
    fcmpo cr0, f4, f0
    fsubs f2, f3, f1
    stfs f1, 0x1dc(r3)
    ble @L_800401C8
    b @L_800401CC
    @L_800401C8:
    fneg f4, f4
    @L_800401CC:
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_800401E0
    fmr f0, f2
    b @L_800401E4
    @L_800401E0:
    fneg f0, f2
    @L_800401E4:
    fcmpo cr0, f0, f4
    cror eq, lt, eq
    beq @L_80040210
    lfs f0, lbl_8047BC94(r2)
    fcmpo cr0, f2, f0
    ble @L_80040200
    b @L_80040204
    @L_80040200:
    fneg f2, f2
    @L_80040204:
    lfs f0, lbl_8047BCBC(r2)
    fcmpo cr0, f2, f0
    bge @L_80040214
    @L_80040210:
    stfs f3, 0x1dc(r3)
    @L_80040214:
    lfs f1, 0x50(r4)
    lfs f0, 0x4c(r4)
    fcmpu cr0, f1, f0
    bne @L_800402EC
    cmplwi r31, 0x0
    beq @L_800402EC
    lwz r3, 0x4(r31)
    bl fn_8005DA18
    bl fn_80105624
    lwz r0, lbl_804788C0(r13)
    lhz r30, 0x4(r3)
    cmpwi r0, 0x0
    beq @L_800402B0
    clrlwi r29, r30, 16
    rlwinm r0, r29, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_80040280
    li r3, 0x24
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r0, 0x1
    lis r3, lbl_803A6818@ha
    stb r0, 0x98(r31)
    addi r3, r3, lbl_803A6818@l
    li r0, 0x0
    stw r0, 0x18(r3)
    @L_80040280:
    rlwinm r0, r29, 0, 25, 25
    cmpwi r0, 0x0
    beq @L_800402B0
    li r3, 0x24
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    li r0, 0x1
    lis r3, lbl_803A6818@ha
    stb r0, 0x98(r31)
    addi r3, r3, lbl_803A6818@l
    stw r0, 0x18(r3)
    @L_800402B0:
    clrlwi r0, r30, 16
    rlwinm r0, r0, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_800402EC
    li r3, 0x25
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lis r3, lbl_803A6818@ha
    li r4, -0x1
    addi r3, r3, lbl_803A6818@l
    li r0, 0x1
    stw r4, 0x18(r3)
    stb r0, 0x98(r31)
    stb r0, 0x99(r31)
    @L_800402EC:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
