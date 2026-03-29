    nofralloc
    stwu r1, -0x20(r1)
    lhz r0, 0x94(r3)
    lis r5, lbl_802E4F68@ha
    lwz r7, 0x60(r3)
    addi r3, r5, lbl_802E4F68@l
    sth r0, 0x8(r1)
    li r5, 0x0
    lha r6, 0x6(r4)
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002B48C
    addi r3, r3, 0x8
    li r5, 0x1
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002B48C
    addi r3, r3, 0x8
    li r5, 0x2
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002B48C
    addi r3, r3, 0x8
    li r5, 0x3
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002B48C
    addi r3, r3, 0x8
    li r5, 0x4
    lwz r0, 0x0(r3)
    cmpw r6, r0
    beq @L_8002B48C
    li r5, 0x5
    @L_8002B48C:
    cmpwi r5, 0x5
    blt @L_8002B49C
    li r3, 0x0
    b @L_8002B4AC
    @L_8002B49C:
    lis r3, lbl_802E4F68@ha
    slwi r5, r5, 3
    addi r0, r3, lbl_802E4F68@l
    add r3, r0, r5
    @L_8002B4AC:
    cmplwi r3, 0x0
    bne @L_8002B4BC
    li r3, 0x0
    b @L_8002B58C
    @L_8002B4BC:
    lbz r6, 0x9(r1)
    lha r3, 0x4(r3)
    extsb r6, r6
    mulli r0, r6, 0x1f
    add r0, r3, r0
    extsh r0, r0
    sth r0, 0x52(r4)
    lwz r3, 0x14(r7)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    bne @L_8002B508
    lwz r3, 0xc(r7)
    lha r5, 0x52(r4)
    lfs f0, 0x0(r3)
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    add r0, r5, r0
    sth r0, 0x52(r4)
    @L_8002B508:
    lwz r5, 0xc(r7)
    lis r0, 0x4330
    lbz r3, 0x8(r1)
    lfs f0, 0x0(r5)
    extsb r3, r3
    stw r0, 0x18(r1)
    fctiwz f0, f0
    add r0, r6, r3
    mulli r0, r0, 0x1f
    lfd f1, lbl_8047B998(r2)
    lfs f2, lbl_8047B984(r2)
    stfd f0, 0x10(r1)
    lwz r3, 0x14(r1)
    add r0, r3, r0
    xoris r0, r0, 0x8000
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f1
    fmuls f2, f2, f0
    lfs f1, lbl_8047B988(r2)
    lfs f0, lbl_8047B98C(r2)
    b @L_8002B564
    @L_8002B560:
    fsubs f2, f2, f1
    @L_8002B564:
    fcmpo cr0, f2, f0
    bgt @L_8002B560
    lfs f1, lbl_8047B988(r2)
    lfs f0, lbl_8047B990(r2)
    b @L_8002B57C
    @L_8002B578:
    fadds f2, f2, f1
    @L_8002B57C:
    fcmpo cr0, f2, f0
    blt @L_8002B578
    stfs f2, 0x70(r4)
    li r3, 0x0
    @L_8002B58C:
    addi r1, r1, 0x20
    blr
