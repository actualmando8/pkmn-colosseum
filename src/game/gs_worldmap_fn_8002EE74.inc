    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stfd f29, 0x50(r1)
    psq_st f29, 0x58(r1), 0, 0
    stfd f28, 0x40(r1)
    psq_st f28, 0x48(r1), 0, 0
    stfd f27, 0x30(r1)
    psq_st f27, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    lwz r0, lbl_8047A428(r13)
    lis r3, lbl_803A2688@ha
    addi r3, r3, lbl_803A2688@l
    li r4, 0x3
    clrlwi r5, r0, 16
    bl fn_8012A5B0
    mr r0, r3
    li r3, 0xd9
    mr r30, r0
    li r4, 0x0
    bl fn_801021F8
    mr r3, r30
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002EFF8
    mr r3, r30
    bl fn_80075FEC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8002EFF8
    mr r3, r30
    bl fn_8011E8DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002EFF8
    li r3, 0xd9
    bl fn_80104704
    mr r31, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r31, 0x0
    mr r31, r3
    beq @L_8002EF50
    cmplwi r31, 0x0
    beq @L_8002EF50
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43e1
    stw r0, 0x4c(r31)
    @L_8002EF50:
    li r3, 0x26
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lfs f27, lbl_8047B9D4(r2)
    lfd f31, lbl_8047B9E0(r2)
    lis r31, 0x4330
    lfd f29, lbl_8047B9E8(r2)
    lfs f28, lbl_8047B9DC(r2)
    b @L_8002EFB0
    @L_8002EF78:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x1c(r1)
    stw r31, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
    @L_8002EFB0:
    fcmpo cr0, f27, f28
    blt @L_8002EF78
    li r3, 0xd9
    bl fn_80104704
    mr r31, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r31, 0x0
    beq @L_8002EFEC
    cmplwi r3, 0x0
    beq @L_8002EFEC
    li r0, 0x0
    li r4, 0x0
    stw r0, 0x4c(r3)
    bl fn_80109220
    @L_8002EFEC:
    li r0, 0x7
    stw r0, lbl_8047A42C(r13)
    b @L_8002F244
    @L_8002EFF8:
    mr r3, r30
    bl fn_8011E850
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8002F0EC
    li r3, 0xd9
    bl fn_80104704
    mr r31, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r31, 0x0
    mr r31, r3
    beq @L_8002F044
    cmplwi r31, 0x0
    beq @L_8002F044
    li r4, 0x1
    bl fn_80109220
    li r0, 0x44be
    stw r0, 0x4c(r31)
    @L_8002F044:
    li r3, 0x26
    li r4, 0x0
    li r5, 0x0
    bl fn_80166AB8
    lfs f27, lbl_8047B9D4(r2)
    lfd f28, lbl_8047B9E0(r2)
    lis r31, 0x4330
    lfd f30, lbl_8047B9E8(r2)
    lfs f31, lbl_8047B9DC(r2)
    b @L_8002F0A4
    @L_8002F06C:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x18(r1)
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
    @L_8002F0A4:
    fcmpo cr0, f27, f31
    blt @L_8002F06C
    li r3, 0xd9
    bl fn_80104704
    mr r31, r3
    li r4, 0x10b2
    bl fn_801046C8
    cmplwi r31, 0x0
    beq @L_8002F0E0
    cmplwi r3, 0x0
    beq @L_8002F0E0
    li r0, 0x0
    li r4, 0x0
    stw r0, 0x4c(r3)
    bl fn_80109220
    @L_8002F0E0:
    li r0, 0x7
    stw r0, lbl_8047A42C(r13)
    b @L_8002F244
    @L_8002F0EC:
    li r3, 0x2
    bl fn_80103CC0
    li r0, 0x1
    stw r0, 0x8(r1)
    bl fn_801046B8
    mr r4, r3
    addi r5, r1, 0x8
    li r3, 0xe3
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0xe3
    bl fn_80104704
    mr r31, r3
    li r4, 0x102a
    bl fn_801046C8
    cmplwi r31, 0x0
    mr r31, r3
    beq @L_8002F158
    cmplwi r31, 0x0
    beq @L_8002F158
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43e4
    stw r0, 0x4c(r31)
    @L_8002F158:
    li r3, 0xe3
    bl fn_80104704
    mr r31, r3
    li r4, 0x1029
    bl fn_801046C8
    cmplwi r31, 0x0
    mr r31, r3
    beq @L_8002F190
    cmplwi r31, 0x0
    beq @L_8002F190
    li r4, 0x1
    bl fn_80109220
    li r0, 0x43e5
    stw r0, 0x4c(r31)
    @L_8002F190:
    li r3, 0xe3
    li r4, 0x1
    bl fn_801045A8
    li r3, 0xe3
    bl fn_801043A4
    mr r31, r3
    li r3, 0xe3
    bl fn_801023E4
    mr r30, r3
    li r3, 0xe3
    bl fn_80102510
    cmpwi r31, -0x1
    bne @L_8002F1C8
    li r30, -0x1
    @L_8002F1C8:
    li r3, 0x1
    bl fn_80103CC0
    bl fn_80102004
    cmpwi r3, 0x1
    bne @L_8002F204
    li r3, 0x2
    li r4, 0x4448
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0xd9
    bl fn_80102510
    li r0, 0x0
    stw r0, lbl_8047A42C(r13)
    b @L_8002F244
    @L_8002F204:
    cmpwi r30, 0x0
    beq @L_8002F230
    bge @L_8002F21C
    cmpwi r30, -0x1
    bge @L_8002F23C
    b @L_8002F244
    @L_8002F21C:
    cmpwi r30, 0x2
    bge @L_8002F244
    li r0, 0xa
    stw r0, lbl_8047A42C(r13)
    b @L_8002F244
    @L_8002F230:
    li r0, 0xb
    stw r0, lbl_8047A42C(r13)
    b @L_8002F244
    @L_8002F23C:
    li r0, 0x7
    stw r0, lbl_8047A42C(r13)
    @L_8002F244:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    psq_l f29, 0x58(r1), 0, 0
    lfd f29, 0x50(r1)
    psq_l f28, 0x48(r1), 0, 0
    lfd f28, 0x40(r1)
    psq_l f27, 0x38(r1), 0, 0
    lfd f27, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r0, 0x84(r1)
    lwz r30, 0x28(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
