    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stfd f29, 0x90(r1)
    psq_st f29, 0x98(r1), 0, 0
    stfd f28, 0x80(r1)
    psq_st f28, 0x88(r1), 0, 0
    stmw r27, 0x6c(r1)
    mr r28, r3
    lwz r31, 0x4(r3)
    lwz r30, 0x0(r3)
    cmplwi r31, 0x0
    lbz r27, 0x18(r3)
    beq @L_8013EFCC
    cmplwi r30, 0x0
    bne @L_8013EA98
    b @L_8013EFCC
    @L_8013EA98:
    lwz r29, 0x8(r30)
    cmplwi r29, 0x0
    beq @L_8013EFCC
    mr r3, r30
    bl fn_800EC134
    lis r4, lbl_80363CD8@ha
    mr r3, r30
    addi r4, r4, lbl_80363CD8@l
    bl fn_800E6478
    cmplwi r29, 0x0
    bne @L_8013EAD4
    la r3, lbl_8047D2E0(r2)
    li r4, 0x3e4
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013EAD4:
    addic. r0, r1, 0x2c
    bne @L_8013EAF0
    lis r4, lbl_80272FD0@ha
    la r3, lbl_8047D2E0(r2)
    addi r5, r4, lbl_80272FD0@l
    li r4, 0x3e5
    bl fn_80196E10
    @L_8013EAF0:
    lwz r3, 0x38(r29)
    cmplwi r29, 0x0
    lwz r0, 0x3c(r29)
    stw r3, 0x2c(r1)
    stw r0, 0x30(r1)
    lwz r0, 0x40(r29)
    stw r0, 0x34(r1)
    bne @L_8013EB20
    la r3, lbl_8047D2E0(r2)
    li r4, 0x351
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013EB20:
    addic. r0, r1, 0x44
    bne @L_8013EB38
    la r3, lbl_8047D2E0(r2)
    li r4, 0x352
    la r5, lbl_8047D2F0(r2)
    bl fn_80196E10
    @L_8013EB38:
    lwz r4, 0x2c(r29)
    mr r3, r30
    lwz r0, 0x30(r29)
    stw r4, 0x44(r1)
    stw r0, 0x48(r1)
    lwz r0, 0x34(r29)
    stw r0, 0x4c(r1)
    bl fn_800E3C5C
    mr r5, r3
    addi r3, r1, 0x14
    addi r4, r5, 0x1c
    addi r5, r5, 0x10
    bl fn_800E019C
    addi r3, r1, 0x14
    lfs f1, lbl_8047D2D4(r2)
    mr r4, r3
    bl fn_800E013C
    mr r3, r30
    bl fn_800E4514
    mr r0, r3
    addi r3, r1, 0x14
    mr r4, r0
    mr r5, r3
    bl fn_800DFF98
    addi r3, r1, 0x14
    addi r5, r1, 0x2c
    mr r4, r3
    bl fn_800E0168
    li r3, 0x2
    bl fn_800D377C
    li r3, 0x2
    bl fn_800D4604
    bl fn_800D2248
    mr r3, r31
    li r4, 0x0
    bl fn_800D3410
    xoris r5, r27, 0x8000
    lis r0, 0x4330
    stw r5, 0x54(r1)
    mr r3, r30
    lfs f1, 0x28(r28)
    li r4, 0x20
    stw r0, 0x50(r1)
    lfd f3, lbl_8047D2F8(r2)
    fmr f29, f1
    lfd f0, 0x50(r1)
    lfs f4, lbl_8047D2A8(r2)
    fsubs f0, f0, f3
    stw r5, 0x5c(r1)
    fsubs f2, f1, f4
    stw r0, 0x58(r1)
    fdivs f30, f4, f0
    lfd f1, 0x58(r1)
    stw r5, 0x64(r1)
    stw r0, 0x60(r1)
    lfd f0, 0x60(r1)
    fsubs f1, f1, f3
    fsubs f0, f0, f3
    fdivs f28, f2, f1
    fnmsubs f31, f0, f30, f4
    bl fn_800E69C4
    mr r3, r30
    bl fn_800E66B8
    lis r3, lbl_80363CD8@ha
    li r4, 0x1
    addi r31, r3, lbl_80363CD8@l
    li r3, 0x4
    li r0, 0x5
    stb r4, 0x4(r31)
    li r28, 0x0
    stb r3, 0x5(r31)
    stb r0, 0x6(r31)
    b @L_8013EE24
    @L_8013EC5C:
    fsubs f29, f29, f28
    addi r3, r1, 0x38
    fadds f31, f31, f30
    addi r4, r1, 0x44
    fmr f1, f29
    bl fn_800E013C
    lfs f3, 0x38(r1)
    addi r3, r1, 0x20
    lfs f1, 0x44(r1)
    addi r4, r1, 0x2c
    lfs f2, 0x3c(r1)
    addi r5, r1, 0x8
    lfs f0, 0x48(r1)
    fsubs f3, f3, f1
    lfs f4, 0x14(r1)
    lfs f1, 0x40(r1)
    fsubs f2, f2, f0
    lfs f0, 0x4c(r1)
    fmuls f4, f4, f3
    lfs f3, 0x18(r1)
    fsubs f0, f1, f0
    lfs f1, 0x1c(r1)
    fmuls f2, f3, f2
    stfs f4, 0x8(r1)
    fmuls f0, f1, f0
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_800E0168
    cmplwi r29, 0x0
    bne @L_8013ECE4
    la r3, lbl_8047D2E0(r2)
    li r4, 0x316
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013ECE4:
    addic. r0, r1, 0x38
    bne @L_8013ECFC
    la r3, lbl_8047D2E0(r2)
    li r4, 0x317
    la r5, lbl_8047D2F0(r2)
    bl fn_80196E10
    @L_8013ECFC:
    lwz r3, 0x38(r1)
    lwz r0, 0x3c(r1)
    stw r3, 0x2c(r29)
    stw r0, 0x30(r29)
    lwz r0, 0x40(r1)
    stw r0, 0x34(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_8013ED68
    cmplwi r29, 0x0
    beq @L_8013ED68
    bne @L_8013ED3C
    la r3, lbl_8047D2E0(r2)
    li r4, 0x25d
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013ED3C:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_8013ED58
    rlwinm. r0, r4, 0, 25, 25
    beq @L_8013ED58
    li r3, 0x1
    @L_8013ED58:
    cmpwi r3, 0x0
    bne @L_8013ED68
    mr r3, r29
    bl fn_8019D620
    @L_8013ED68:
    cmplwi r29, 0x0
    bne @L_8013ED80
    la r3, lbl_8047D2E0(r2)
    li r4, 0x3a9
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013ED80:
    addic. r0, r1, 0x20
    bne @L_8013ED9C
    lis r4, lbl_80272FD0@ha
    la r3, lbl_8047D2E0(r2)
    addi r5, r4, lbl_80272FD0@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_8013ED9C:
    lwz r3, 0x20(r1)
    lwz r0, 0x24(r1)
    stw r3, 0x38(r29)
    stw r0, 0x3c(r29)
    lwz r0, 0x28(r1)
    stw r0, 0x40(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_8013EE08
    cmplwi r29, 0x0
    beq @L_8013EE08
    bne @L_8013EDDC
    la r3, lbl_8047D2E0(r2)
    li r4, 0x25d
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013EDDC:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_8013EDF8
    rlwinm. r0, r4, 0, 25, 25
    beq @L_8013EDF8
    li r3, 0x1
    @L_8013EDF8:
    cmpwi r3, 0x0
    bne @L_8013EE08
    mr r3, r29
    bl fn_8019D620
    @L_8013EE08:
    fmr f1, f31
    mr r3, r30
    bl fn_800E6804
    mr r3, r30
    li r4, 0x3010
    bl fn_800E3760
    addi r28, r28, 0x1
    @L_8013EE24:
    cmpw r28, r27
    blt @L_8013EC5C
    cmplwi r29, 0x0
    bne @L_8013EE44
    la r3, lbl_8047D2E0(r2)
    li r4, 0x3a9
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013EE44:
    addic. r0, r1, 0x2c
    bne @L_8013EE60
    lis r4, lbl_80272FD0@ha
    la r3, lbl_8047D2E0(r2)
    addi r5, r4, lbl_80272FD0@l
    li r4, 0x3aa
    bl fn_80196E10
    @L_8013EE60:
    lwz r3, 0x2c(r1)
    lwz r0, 0x30(r1)
    stw r3, 0x38(r29)
    stw r0, 0x3c(r29)
    lwz r0, 0x34(r1)
    stw r0, 0x40(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_8013EECC
    cmplwi r29, 0x0
    beq @L_8013EECC
    bne @L_8013EEA0
    la r3, lbl_8047D2E0(r2)
    li r4, 0x25d
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013EEA0:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_8013EEBC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_8013EEBC
    li r3, 0x1
    @L_8013EEBC:
    cmpwi r3, 0x0
    bne @L_8013EECC
    mr r3, r29
    bl fn_8019D620
    @L_8013EECC:
    cmplwi r29, 0x0
    bne @L_8013EEE4
    la r3, lbl_8047D2E0(r2)
    li r4, 0x316
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013EEE4:
    addic. r0, r1, 0x44
    bne @L_8013EEFC
    la r3, lbl_8047D2E0(r2)
    li r4, 0x317
    la r5, lbl_8047D2F0(r2)
    bl fn_80196E10
    @L_8013EEFC:
    lwz r3, 0x44(r1)
    lwz r0, 0x48(r1)
    stw r3, 0x2c(r29)
    stw r0, 0x30(r29)
    lwz r0, 0x4c(r1)
    stw r0, 0x34(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_8013EF68
    cmplwi r29, 0x0
    beq @L_8013EF68
    bne @L_8013EF3C
    la r3, lbl_8047D2E0(r2)
    li r4, 0x25d
    la r5, lbl_8047D2E8(r2)
    bl fn_80196E10
    @L_8013EF3C:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_8013EF58
    rlwinm. r0, r4, 0, 25, 25
    beq @L_8013EF58
    li r3, 0x1
    @L_8013EF58:
    cmpwi r3, 0x0
    bne @L_8013EF68
    mr r3, r29
    bl fn_8019D620
    @L_8013EF68:
    li r3, 0x2
    li r4, 0x5
    li r0, 0x0
    stb r3, 0x4(r31)
    lfs f1, lbl_8047D2A8(r2)
    mr r3, r30
    stb r4, 0x5(r31)
    stb r0, 0x6(r31)
    bl fn_800E6804
    mr r3, r30
    li r4, 0x3010
    bl fn_800E3760
    mr r3, r30
    bl fn_800E65CC
    mr r3, r30
    bl fn_800E68D8
    mr r3, r30
    bl fn_800E65CC
    bl fn_800D3190
    li r3, 0x1
    bl fn_800D4604
    li r3, 0x1
    bl fn_800D377C
    mr r3, r30
    bl fn_800E638C
    @L_8013EFCC:
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    lmw r27, 0x6c(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
