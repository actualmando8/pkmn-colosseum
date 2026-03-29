    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r27, 0x3c(r1)
    mr r27, r3
    li r3, 0x98
    bl fn_8005D858
    lha r29, 0xc(r3)
    li r3, 0x98
    bl fn_8005D858
    lis r4, lbl_803A20DC@ha
    lha r28, 0xe(r3)
    addi r31, r4, lbl_803A20DC@l
    li r30, 0x0
    @L_80028868:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    beq @L_80028920
    lis r3, 0x4330
    xoris r4, r29, 0x8000
    xoris r0, r28, 0x8000
    stw r4, 0xc(r1)
    lfd f3, lbl_8047B948(r2)
    mr r8, r27
    stw r3, 0x8(r1)
    li r9, 0x98
    lfs f5, 0xc(r31)
    li r10, 0x0
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f3
    lfs f4, lbl_8047B940(r2)
    stw r3, 0x10(r1)
    lfs f0, 0x4(r31)
    fmuls f6, f1, f5
    lfd f1, 0x10(r1)
    lfs f2, 0x8(r31)
    fsubs f3, f1, f3
    lwz r3, 0x14(r31)
    fnmsubs f1, f6, f4, f0
    fadds f0, f4, f6
    lbz r0, 0x18(r31)
    fmuls f5, f3, f5
    fadds f3, f4, f1
    or r7, r3, r0
    fctiwz f1, f0
    fnmsubs f2, f5, f4, f2
    fadds f0, f4, f5
    fctiwz f3, f3
    stfd f1, 0x28(r1)
    fadds f1, f4, f2
    fctiwz f0, f0
    lwz r5, 0x2c(r1)
    stfd f3, 0x18(r1)
    fctiwz f1, f1
    stfd f0, 0x30(r1)
    lwz r3, 0x1c(r1)
    stfd f1, 0x20(r1)
    lwz r6, 0x34(r1)
    lwz r4, 0x24(r1)
    bl fn_80104160
    @L_80028920:
    addi r31, r31, 0x24
    addi r30, r30, 0x1
    cmpwi r30, 0x1e
    blt @L_80028868
    li r3, 0x0
    lmw r27, 0x3c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
