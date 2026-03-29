    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0xc(r1)
    lfd f1, lbl_8047B828(r2)
    stw r0, 0x8(r1)
    lfd f0, 0x8(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x14(r1)
    lfd f3, lbl_8047B830(r2)
    stw r0, 0x10(r1)
    lfs f2, lbl_8047B810(r2)
    lfd f0, 0x10(r1)
    lfs f1, lbl_8047A338(r13)
    fsubs f3, f0, f3
    lfs f0, lbl_8047B818(r2)
    fdivs f3, f3, f31
    fmadds f1, f3, f2, f1
    stfs f3, lbl_8047A344(r13)
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A338(r13)
    cror eq, gt, eq
    bne @L_8001EFF8
    lfs f0, lbl_8047B814(r2)
    stfs f0, lbl_8047A338(r13)
    @L_8001EFF8:
    lwz r0, lbl_8047A31C(r13)
    cmpwi r0, 0x1e
    blt @L_8001F1D0
    cmpwi r0, 0xc8
    beq @L_8001F1D0
    lfs f0, lbl_8047A334(r13)
    li r3, 0x46a
    fadds f0, f0, f3
    stfs f0, lbl_8047A334(r13)
    bl fn_801666BC
    cmpwi r3, 0x0
    bne @L_8001F044
    lwz r0, lbl_8047A31C(r13)
    lfs f0, lbl_8047B814(r2)
    cmpwi r0, 0x3e8
    stfs f0, lbl_8047A334(r13)
    beq @L_8001F044
    li r0, 0x28
    stw r0, lbl_8047A31C(r13)
    @L_8001F044:
    lis r3, lbl_803A1F88@ha
    lfs f0, lbl_8047A344(r13)
    addi r3, r3, lbl_803A1F88@l
    li r0, 0x3
    mtctr r0
    @L_8001F058:
    lfs f1, 0x10(r3)
    lfs f2, 0x28(r3)
    fcmpu cr0, f1, f2
    beq @L_8001F110
    fsubs f3, f2, f1
    lfs f2, lbl_8047B81C(r2)
    lfs f1, lbl_8047B820(r2)
    fmuls f2, f2, f3
    fmuls f4, f2, f0
    fcmpo cr0, f4, f1
    ble @L_8001F088
    fmr f4, f1
    @L_8001F088:
    lfs f1, lbl_8047B824(r2)
    fcmpo cr0, f4, f1
    cror eq, lt, eq
    bne @L_8001F09C
    fmr f4, f1
    @L_8001F09C:
    lfs f2, 0x10(r3)
    lfs f1, lbl_8047B814(r2)
    fadds f2, f2, f4
    fcmpo cr0, f4, f1
    stfs f2, 0x10(r3)
    lfs f3, 0x28(r3)
    lfs f1, 0x10(r3)
    fsubs f2, f3, f1
    ble @L_8001F0C4
    b @L_8001F0C8
    @L_8001F0C4:
    fneg f4, f4
    @L_8001F0C8:
    lfs f1, lbl_8047B814(r2)
    fcmpo cr0, f2, f1
    ble @L_8001F0DC
    fmr f1, f2
    b @L_8001F0E0
    @L_8001F0DC:
    fneg f1, f2
    @L_8001F0E0:
    fcmpo cr0, f1, f4
    cror eq, lt, eq
    beq @L_8001F10C
    lfs f1, lbl_8047B814(r2)
    fcmpo cr0, f2, f1
    ble @L_8001F0FC
    b @L_8001F100
    @L_8001F0FC:
    fneg f2, f2
    @L_8001F100:
    lfs f1, lbl_8047B818(r2)
    fcmpo cr0, f2, f1
    bge @L_8001F110
    @L_8001F10C:
    stfs f3, 0x10(r3)
    @L_8001F110:
    lfs f1, 0x1c(r3)
    lfs f2, 0x34(r3)
    fcmpu cr0, f1, f2
    beq @L_8001F1C8
    fsubs f3, f2, f1
    lfs f2, lbl_8047B81C(r2)
    lfs f1, lbl_8047B820(r2)
    fmuls f2, f2, f3
    fmuls f4, f2, f0
    fcmpo cr0, f4, f1
    ble @L_8001F140
    fmr f4, f1
    @L_8001F140:
    lfs f1, lbl_8047B824(r2)
    fcmpo cr0, f4, f1
    cror eq, lt, eq
    bne @L_8001F154
    fmr f4, f1
    @L_8001F154:
    lfs f2, 0x1c(r3)
    lfs f1, lbl_8047B814(r2)
    fadds f2, f2, f4
    fcmpo cr0, f4, f1
    stfs f2, 0x1c(r3)
    lfs f3, 0x34(r3)
    lfs f1, 0x1c(r3)
    fsubs f2, f3, f1
    ble @L_8001F17C
    b @L_8001F180
    @L_8001F17C:
    fneg f4, f4
    @L_8001F180:
    lfs f1, lbl_8047B814(r2)
    fcmpo cr0, f2, f1
    ble @L_8001F194
    fmr f1, f2
    b @L_8001F198
    @L_8001F194:
    fneg f1, f2
    @L_8001F198:
    fcmpo cr0, f1, f4
    cror eq, lt, eq
    beq @L_8001F1C4
    lfs f1, lbl_8047B814(r2)
    fcmpo cr0, f2, f1
    ble @L_8001F1B4
    b @L_8001F1B8
    @L_8001F1B4:
    fneg f2, f2
    @L_8001F1B8:
    lfs f1, lbl_8047B818(r2)
    fcmpo cr0, f2, f1
    bge @L_8001F1C8
    @L_8001F1C4:
    stfs f3, 0x1c(r3)
    @L_8001F1C8:
    addi r3, r3, 0x4
    bdnz @L_8001F058
    @L_8001F1D0:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
