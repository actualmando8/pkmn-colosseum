    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r31, r3
    lha r0, 0x6(r4)
    lis r3, lbl_802EF0A8@ha
    addi r6, r3, lbl_802EF0A8@l
    mulli r4, r0, 0x1c
    lha r3, 0x3bba(r6)
    lha r0, 0x3bbc(r6)
    lha r5, 0x3bbe(r6)
    add r4, r6, r4
    lha r6, 0x3bc0(r6)
    lha r7, 0x2(r4)
    lha r4, 0x4(r4)
    subf r3, r7, r3
    subf r4, r4, r0
    bl fn_800FE38C
    lbz r0, 0x95(r31)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    srwi r0, r3, 16
    lis r3, lbl_80266918@ha
    sth r0, 0x8(r1)
    addi r3, r3, lbl_80266918@l
    lbz r0, 0x95(r31)
    extsb r4, r0
    mulli r0, r4, 0x4c
    add r3, r3, r0
    lwz r12, 0x14(r3)
    cmplwi r12, 0x0
    beq @L_80016274
    lfs f0, lbl_8047A2D4(r13)
    addi r5, r1, 0x8
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r3, 0x14(r1)
    mtctr r12
    bctrl
    @L_80016274:
    lfs f0, lbl_8047B748(r2)
    lfs f1, lbl_8047A2D4(r13)
    fcmpu cr0, f0, f1
    beq @L_80016320
    fcmpo cr0, f1, f0
    ble @L_800162B0
    lbz r0, 0x95(r31)
    extsb r3, r0
    addi r30, r3, 0x1
    cmpwi r30, 0x6
    blt @L_800162A4
    li r30, 0x1
    @L_800162A4:
    lfs f0, lbl_8047B750(r2)
    fsubs f31, f1, f0
    b @L_800162D0
    @L_800162B0:
    lbz r0, 0x95(r31)
    extsb r3, r0
    subi r30, r3, 0x1
    cmpwi r30, 0x1
    bge @L_800162C8
    li r30, 0x5
    @L_800162C8:
    lfs f0, lbl_8047B750(r2)
    fadds f31, f0, f1
    @L_800162D0:
    mulli r4, r30, 0x4c
    lis r3, lbl_80266918@ha
    addi r0, r3, lbl_80266918@l
    add r3, r0, r4
    lwz r31, 0x14(r3)
    cmplwi r31, 0x0
    beq @L_80016320
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    fctiwz f0, f31
    srwi r0, r3, 16
    mr r12, r31
    mr r4, r30
    sth r0, 0x8(r1)
    addi r5, r1, 0x8
    stfd f0, 0x10(r1)
    lwz r3, 0x14(r1)
    mtctr r12
    bctrl
    @L_80016320:
    bl fn_800FE35C
    li r3, 0x0
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
