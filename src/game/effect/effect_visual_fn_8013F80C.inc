    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    mr r30, r4
    stw r29, 0x24(r1)
    mr. r29, r3
    beq @L_8013F95C
    lwz r0, 0x4(r29)
    cmplwi r0, 0x0
    beq @L_8013F848
    lwz r0, lbl_8047AEE8(r13)
    cmplwi r0, 0x0
    bne @L_8013F850
    @L_8013F848:
    li r3, 0x0
    b @L_8013F960
    @L_8013F850:
    lhz r0, 0x2a(r29)
    cmplwi r0, 0x0
    beq @L_8013F888
    lwz r3, 0x8(r29)
    lwz r4, 0xc(r29)
    bl fn_800EE150
    mr. r31, r3
    beq @L_8013F888
    addi r4, r29, 0x2c
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r31
    bl fn_800EE828
    @L_8013F888:
    lhz r3, 0xb2(r29)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    lfd f2, lbl_8047D310(r2)
    stw r3, 0xc(r1)
    lfs f3, 0x14(r29)
    lfd f0, 0x8(r1)
    stw r0, 0x10(r1)
    fsubs f1, f0, f2
    lfs f0, lbl_8047D300(r2)
    stw r0, 0x18(r1)
    fmuls f1, f3, f1
    stfs f1, 0x10(r29)
    lhz r3, 0xb0(r29)
    lhz r0, 0xb2(r29)
    stw r3, 0x14(r1)
    lfs f3, 0x1c(r29)
    stw r0, 0x1c(r1)
    lfd f1, 0x10(r1)
    fcmpu cr0, f3, f0
    lfd f0, 0x18(r1)
    fsubs f1, f1, f2
    fsubs f0, f0, f2
    fdivs f4, f1, f0
    beq @L_8013F90C
    fcmpo cr0, f4, f3
    cror eq, lt, eq
    bne @L_8013F90C
    fdivs f0, f4, f3
    lfs f1, 0x10(r29)
    fmuls f0, f1, f0
    stfs f0, 0x10(r29)
    b @L_8013F928
    @L_8013F90C:
    lfs f0, lbl_8047D308(r2)
    lfs f2, 0x10(r29)
    fsubs f1, f0, f4
    fsubs f0, f0, f3
    fdivs f0, f1, f0
    fmuls f0, f2, f0
    stfs f0, 0x10(r29)
    @L_8013F928:
    lfs f0, lbl_8047D308(r2)
    lfs f1, 0x24(r29)
    fsubs f0, f0, f4
    fmuls f0, f1, f0
    stfs f0, 0x20(r29)
    lhz r0, 0xb0(r29)
    add r0, r0, r30
    sth r0, 0xb0(r29)
    lhz r3, 0xb0(r29)
    lhz r0, 0xb2(r29)
    subf r0, r0, r3
    srwi r3, r0, 31
    b @L_8013F960
    @L_8013F95C:
    li r3, 0x0
    @L_8013F960:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
