    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    mr r31, r4
    lis r3, lbl_802E52B8@ha
    lbz r0, lbl_8047A47C(r13)
    addi r3, r3, lbl_802E52B8@l
    lfs f2, lbl_8047A484(r13)
    lfs f1, 0x0(r3)
    lis r4, lbl_802E52A8@ha
    extsb r0, r0
    lfs f0, lbl_8047BA58(r2)
    fsubs f31, f2, f1
    slwi r0, r0, 2
    addi r3, r4, lbl_802E52A8@l
    lfsx f1, r3, r0
    fcmpo cr0, f31, f0
    stfs f1, lbl_8047A488(r13)
    bge @L_80037FA0
    lfs f0, lbl_8047BA60(r2)
    fadds f31, f31, f0
    @L_80037FA0:
    lfs f0, lbl_8047BA60(r2)
    fcmpo cr0, f31, f0
    cror eq, gt, eq
    bne @L_80037FB4
    fsubs f31, f31, f0
    @L_80037FB4:
    bl fn_80105624
    lfs f1, lbl_8047BA64(r2)
    lfs f0, lbl_8047A494(r13)
    lfs f3, lbl_8047A484(r13)
    lfs f4, lbl_8047A488(r13)
    fmuls f2, f1, f0
    fcmpu cr0, f3, f4
    beq @L_80038078
    fsubs f1, f4, f3
    lfs f0, lbl_8047BA58(r2)
    fcmpo cr0, f1, f0
    bge @L_80037FE8
    fneg f2, f2
    @L_80037FE8:
    frsp f3, f2
    lfs f1, lbl_8047A484(r13)
    lfs f0, lbl_8047BA60(r2)
    stfs f2, lbl_8047A48C(r13)
    fadds f1, f1, f3
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A484(r13)
    cror eq, gt, eq
    bne @L_80038014
    fsubs f0, f1, f0
    stfs f0, lbl_8047A484(r13)
    @L_80038014:
    lfs f1, lbl_8047A484(r13)
    lfs f0, lbl_8047BA58(r2)
    fcmpo cr0, f1, f0
    bge @L_80038030
    lfs f0, lbl_8047BA60(r2)
    fadds f0, f1, f0
    stfs f0, lbl_8047A484(r13)
    @L_80038030:
    lfs f1, lbl_8047A484(r13)
    lfs f0, lbl_8047BA58(r2)
    fsubs f1, f1, f4
    fcmpo cr0, f1, f0
    ble @L_80038048
    b @L_8003804C
    @L_80038048:
    fneg f1, f1
    @L_8003804C:
    lfs f0, lbl_8047BA58(r2)
    fcmpo cr0, f3, f0
    ble @L_8003805C
    b @L_80038060
    @L_8003805C:
    fneg f3, f3
    @L_80038060:
    fcmpo cr0, f1, f3
    bge @L_800380B8
    lfs f0, lbl_8047BA58(r2)
    stfs f4, lbl_8047A484(r13)
    stfs f0, lbl_8047A48C(r13)
    b @L_800380B8
    @L_80038078:
    lbz r5, lbl_8047A47D(r13)
    lbz r4, lbl_8047A47C(r13)
    extsb r3, r5
    extsb r0, r4
    cmpw r3, r0
    beq @L_800380B8
    subf r0, r4, r5
    extsb r0, r0
    cmpwi r0, 0x0
    ble @L_800380AC
    fneg f0, f2
    stfs f0, lbl_8047A48C(r13)
    b @L_800380B4
    @L_800380AC:
    bge @L_800380B4
    stfs f2, lbl_8047A48C(r13)
    @L_800380B4:
    stb r4, lbl_8047A47D(r13)
    @L_800380B8:
    fmr f1, f31
    bl fn_800CE148
    frsp f3, f1
    lfs f2, lbl_8047BA6C(r2)
    lfs f0, lbl_8047BA68(r2)
    fmr f1, f31
    fmadds f0, f2, f3, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    sth r0, 0x50(r31)
    bl fn_800CDBE0
    frsp f2, f1
    lfs f1, lbl_8047BA6C(r2)
    lfs f0, lbl_8047BA70(r2)
    fmadds f0, f1, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x52(r31)
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
