    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    mr r31, r4
    lbz r0, 0x95(r3)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    srwi r0, r3, 16
    sth r0, 0x8(r1)
    lbz r4, 0x9(r1)
    extsb r4, r4
    mulli r3, r4, 0x1f
    addi r0, r3, 0x95
    extsh r0, r0
    sth r0, 0x52(r31)
    lwz r0, lbl_8047A2C8(r13)
    cmpwi r0, 0x0
    bne @L_8001673C
    lfs f0, lbl_8047A2D0(r13)
    lha r3, 0x52(r31)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    add r0, r3, r0
    sth r0, 0x52(r31)
    @L_8001673C:
    lfs f0, lbl_8047A2D0(r13)
    lis r0, 0x4330
    lbz r3, 0x8(r1)
    fctiwz f0, f0
    stw r0, 0x18(r1)
    extsb r0, r3
    lfd f1, lbl_8047B768(r2)
    add r0, r4, r0
    lfs f2, lbl_8047B754(r2)
    stfd f0, 0x10(r1)
    mulli r0, r0, 0x1f
    lwz r3, 0x14(r1)
    add r0, r3, r0
    xoris r0, r0, 0x8000
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f1
    fmuls f2, f2, f0
    lfs f1, lbl_8047B758(r2)
    lfs f0, lbl_8047B75C(r2)
    b @L_80016794
    @L_80016790:
    fsubs f2, f2, f1
    @L_80016794:
    fcmpo cr0, f2, f0
    bgt @L_80016790
    lfs f1, lbl_8047B758(r2)
    lfs f0, lbl_8047B760(r2)
    b @L_800167AC
    @L_800167A8:
    fadds f2, f2, f1
    @L_800167AC:
    fcmpo cr0, f2, f0
    blt @L_800167A8
    stfs f2, 0x70(r31)
    li r3, 0x0
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
