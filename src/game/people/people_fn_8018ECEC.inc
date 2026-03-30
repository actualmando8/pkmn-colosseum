    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stfd f30, 0x90(r1)
    psq_st f30, 0x98(r1), 0, 0
    stfd f29, 0x80(r1)
    psq_st f29, 0x88(r1), 0, 0
    stfd f28, 0x70(r1)
    psq_st f28, 0x78(r1), 0, 0
    stfd f27, 0x60(r1)
    psq_st f27, 0x68(r1), 0, 0
    stmw r26, 0x48(r1)
    mr. r31, r3
    fmr f29, f1
    li r27, 0x1
    li r26, 0x0
    beq @L_8018F050
    bl fn_8018FBD4
    mr. r28, r3
    beq @L_8018F050
    lwz r0, 0x18(r31)
    cmplwi r0, 0x0
    beq @L_8018EF9C
    lwz r3, 0x30(r31)
    bl fn_8018F6F4
    mr r29, r3
    bl fn_8018F698
    mr r30, r3
    mr r3, r29
    bl fn_8018F678
    fmr f28, f1
    mr r3, r29
    bl fn_8018F658
    fmr f27, f1
    mr r3, r29
    bl fn_8018F638
    fmr f31, f1
    mr r3, r29
    bl fn_8018F618
    extsb. r0, r30
    fmr f30, f1
    blt @L_8018F050
    mr r3, r28
    extsb r4, r30
    bl fn_800EE150
    addi r4, r1, 0x2c
    mr r28, r3
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r28
    bl fn_800EE828
    mr r3, r31
    li r4, 0x2
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EDE0
    li r26, 0x1
    b @L_8018EE10
    @L_8018EDE0:
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f29, f0
    bge @L_8018EDF4
    li r26, 0x1
    b @L_8018EE10
    @L_8018EDF4:
    lwz r3, 0x18(r31)
    addi r4, r1, 0x2c
    bl fn_800E0040
    fcmpo cr0, f1, f29
    cror eq, lt, eq
    bne @L_8018EE10
    li r26, 0x1
    @L_8018EE10:
    clrlwi. r0, r26, 24
    beq @L_8018EF90
    mr r3, r31
    addi r4, r1, 0x20
    bl fn_8018FC2C
    lfs f29, 0x24(r1)
    addi r3, r1, 0x8
    lwz r4, 0x18(r31)
    addi r5, r1, 0x2c
    bl fn_800E0168
    lfs f1, 0x8(r1)
    lfs f2, 0x10(r1)
    bl fn_800CE2D8
    frsp f0, f1
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f0, f29
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f1, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f1, f0
    ble @L_8018EE78
    lfd f0, lbl_8047D7F0(r2)
    fsub f1, f1, f0
    frsp f1, f1
    b @L_8018EE90
    @L_8018EE78:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f1, f0
    bge @L_8018EE90
    lfd f0, lbl_8047D7F0(r2)
    fadd f1, f0, f1
    frsp f1, f1
    @L_8018EE90:
    fmr f29, f1
    fcmpo cr0, f1, f28
    bge @L_8018EEC0
    mr r3, r31
    li r4, 0x2
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EEB8
    fmr f29, f28
    b @L_8018EEE8
    @L_8018EEB8:
    li r27, 0x0
    b @L_8018EEE8
    @L_8018EEC0:
    fcmpo cr0, f1, f27
    ble @L_8018EEE8
    mr r3, r31
    li r4, 0x2
    bl fn_8018FB94
    clrlwi. r0, r3, 24
    beq @L_8018EEE4
    fmr f29, f27
    b @L_8018EEE8
    @L_8018EEE4:
    li r27, 0x0
    @L_8018EEE8:
    clrlwi. r0, r27, 24
    beq @L_8018EF80
    stfs f29, 0xb8(r31)
    addi r3, r1, 0x14
    addi r5, r1, 0x2c
    lwz r4, 0x18(r31)
    bl fn_800E0168
    lfs f1, 0x18(r1)
    lfs f0, lbl_8047D898(r2)
    lfs f3, 0x1c(r1)
    fadds f2, f1, f0
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f3, f0
    stfs f2, 0x18(r1)
    ble @L_8018EF28
    b @L_8018EF2C
    @L_8018EF28:
    fneg f3, f3
    @L_8018EF2C:
    lfs f1, 0x14(r1)
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    ble @L_8018EF40
    b @L_8018EF44
    @L_8018EF40:
    fneg f1, f1
    @L_8018EF44:
    fadds f1, f1, f3
    bl fn_800CE2D8
    frsp f1, f1
    lfd f0, lbl_8047D7B0(r2)
    fsub f0, f1, f0
    frsp f0, f0
    fcmpo cr0, f0, f31
    bge @L_8018EF6C
    fmr f0, f31
    b @L_8018EF78
    @L_8018EF6C:
    fcmpo cr0, f0, f30
    ble @L_8018EF78
    fmr f0, f30
    @L_8018EF78:
    stfs f0, 0xb4(r31)
    b @L_8018EF9C
    @L_8018EF80:
    lfs f0, lbl_8047D7A0(r2)
    stfs f0, 0xb4(r31)
    stfs f0, 0xb8(r31)
    b @L_8018EF9C
    @L_8018EF90:
    lfs f0, lbl_8047D7A0(r2)
    stfs f0, 0xb4(r31)
    stfs f0, 0xb8(r31)
    @L_8018EF9C:
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x3c(r1)
    lfd f2, lbl_8047D7D0(r2)
    stw r0, 0x38(r1)
    lfs f0, lbl_8047D89C(r2)
    lfd f1, 0x38(r1)
    lfs f3, 0xb4(r31)
    fsubs f1, f1, f2
    lfs f2, 0xc(r31)
    fcmpo cr0, f2, f3
    fmuls f4, f0, f1
    ble @L_8018EFEC
    fsubs f0, f2, f4
    stfs f0, 0xc(r31)
    lfs f0, 0xc(r31)
    fcmpo cr0, f0, f3
    bge @L_8018F008
    stfs f3, 0xc(r31)
    b @L_8018F008
    @L_8018EFEC:
    bge @L_8018F008
    fadds f0, f2, f4
    stfs f0, 0xc(r31)
    lfs f0, 0xc(r31)
    fcmpo cr0, f0, f3
    ble @L_8018F008
    stfs f3, 0xc(r31)
    @L_8018F008:
    lfs f1, 0xb8(r31)
    lfs f0, 0x10(r31)
    fcmpo cr0, f0, f1
    ble @L_8018F034
    fsubs f0, f0, f4
    stfs f0, 0x10(r31)
    lfs f0, 0x10(r31)
    fcmpo cr0, f0, f1
    bge @L_8018F050
    stfs f1, 0x10(r31)
    b @L_8018F050
    @L_8018F034:
    bge @L_8018F050
    fadds f0, f0, f4
    stfs f0, 0x10(r31)
    lfs f0, 0x10(r31)
    fcmpo cr0, f0, f1
    ble @L_8018F050
    stfs f1, 0x10(r31)
    @L_8018F050:
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    psq_l f30, 0x98(r1), 0, 0
    lfd f30, 0x90(r1)
    psq_l f29, 0x88(r1), 0, 0
    lfd f29, 0x80(r1)
    psq_l f28, 0x78(r1), 0, 0
    lfd f28, 0x70(r1)
    psq_l f27, 0x68(r1), 0, 0
    lfd f27, 0x60(r1)
    lmw r26, 0x48(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
