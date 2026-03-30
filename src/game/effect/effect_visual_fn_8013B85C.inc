    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    cmplwi r3, 0x0
    mr r30, r4
    beq @L_8013BA6C
    mr r31, r3
    lis r5, 0x4330
    lhz r3, 0xcc(r3)
    li r4, 0x1
    lhz r0, 0xce(r31)
    stw r3, 0xc(r1)
    lfd f2, lbl_8047D208(r2)
    stw r5, 0x8(r1)
    lwz r3, 0x0(r31)
    lfd f0, 0x8(r1)
    stw r0, 0x14(r1)
    fsubs f1, f0, f2
    stw r5, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f2
    fdivs f31, f1, f0
    bl fn_800E4014
    fmr f1, f31
    mr r3, r31
    bl fn_8013BC10
    lhz r4, 0x1e(r31)
    srwi r3, r4, 31
    clrlwi r0, r4, 31
    xor r0, r0, r3
    subf. r0, r3, r0
    beq @L_8013B928
    srawi r29, r4, 1
    lwz r3, 0xbc(r31)
    addi r28, r29, 0x1
    addi r4, r31, 0x5c
    mr r5, r28
    bl fn_800E09E8
    mulli r0, r28, 0xc
    lwz r3, 0xbc(r31)
    mr r5, r29
    addi r4, r31, 0x8c
    add r3, r3, r0
    bl fn_800E09E8
    b @L_8013B954
    @L_8013B928:
    srawi r28, r4, 1
    lwz r3, 0xbc(r31)
    mr r5, r28
    addi r4, r31, 0x5c
    bl fn_800E09E8
    mulli r0, r28, 0xc
    lwz r3, 0xbc(r31)
    mr r5, r28
    addi r4, r31, 0x8c
    add r3, r3, r0
    bl fn_800E09E8
    @L_8013B954:
    lhz r4, 0x1c(r31)
    li r5, 0x0
    lhz r8, 0x1e(r31)
    lwz r7, 0x4(r31)
    b @L_8013BA3C
    @L_8013B968:
    cmpwi r8, 0x0
    lwz r6, 0xbc(r31)
    mr r3, r8
    ble @L_8013BA38
    srwi. r0, r8, 3
    mtctr r0
    beq @L_8013BA18
    @L_8013B984:
    lfs f0, 0x4(r6)
    stfs f0, 0x4(r7)
    lfs f0, 0x8(r6)
    stfs f0, 0x8(r7)
    lfs f0, 0x10(r6)
    stfs f0, 0x10(r7)
    lfs f0, 0x14(r6)
    stfs f0, 0x14(r7)
    lfs f0, 0x1c(r6)
    stfs f0, 0x1c(r7)
    lfs f0, 0x20(r6)
    stfs f0, 0x20(r7)
    lfs f0, 0x28(r6)
    stfs f0, 0x28(r7)
    lfs f0, 0x2c(r6)
    stfs f0, 0x2c(r7)
    lfs f0, 0x34(r6)
    stfs f0, 0x34(r7)
    lfs f0, 0x38(r6)
    stfs f0, 0x38(r7)
    lfs f0, 0x40(r6)
    stfs f0, 0x40(r7)
    lfs f0, 0x44(r6)
    stfs f0, 0x44(r7)
    lfs f0, 0x4c(r6)
    stfs f0, 0x4c(r7)
    lfs f0, 0x50(r6)
    stfs f0, 0x50(r7)
    lfs f0, 0x58(r6)
    stfs f0, 0x58(r7)
    lfs f0, 0x5c(r6)
    addi r6, r6, 0x60
    stfs f0, 0x5c(r7)
    addi r7, r7, 0x60
    bdnz @L_8013B984
    andi. r3, r3, 0x7
    beq @L_8013BA38
    @L_8013BA18:
    mtctr r3
    @L_8013BA1C:
    lfs f0, 0x4(r6)
    stfs f0, 0x4(r7)
    lfs f0, 0x8(r6)
    addi r6, r6, 0xc
    stfs f0, 0x8(r7)
    addi r7, r7, 0xc
    bdnz @L_8013BA1C
    @L_8013BA38:
    addi r5, r5, 0x1
    @L_8013BA3C:
    cmpw r5, r4
    blt @L_8013B968
    mr r3, r31
    bl fn_8013BA98
    lhz r0, 0xcc(r31)
    add r0, r0, r30
    sth r0, 0xcc(r31)
    lhz r3, 0xcc(r31)
    lhz r0, 0xce(r31)
    subf r0, r0, r3
    srwi r3, r0, 31
    b @L_8013BA70
    @L_8013BA6C:
    li r3, 0x0
    @L_8013BA70:
    psq_l f31, 0x38(r1), 0, 0
    lwz r0, 0x44(r1)
    lfd f31, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
