    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stfd f30, 0x20(r1)
    psq_st f30, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    lwz r3, 0x0(r3)
    rlwinm. r0, r3, 0, 19, 19
    bne @L_800ED4AC
    rlwinm. r0, r3, 0, 24, 24
    beq @L_800ED244
    lwz r3, 0x10(r30)
    li r4, 0x1cb
    lfs f1, 0xc4(r30)
    bl fn_801A32A0
    lwz r3, 0x14(r30)
    li r4, 0x1cb
    lfs f1, 0xc8(r30)
    bl fn_801A32A0
    lwz r3, 0x10(r30)
    bl fn_801A1B40
    lwz r3, 0x14(r30)
    bl fn_801A1B40
    mr r3, r30
    bl fn_800EB5A0
    b @L_800ED4A0
    @L_800ED244:
    rlwinm. r0, r3, 0, 14, 14
    lwz r31, 0x8(r30)
    beq @L_800ED254
    lwz r31, 0x10(r31)
    @L_800ED254:
    lfs f31, lbl_8047CC5C(r2)
    rlwinm. r0, r3, 0, 26, 26
    fmr f30, f31
    beq @L_800ED2B0
    rlwinm r0, r3, 0, 20, 20
    cmplwi r0, 0x800
    beq @L_800ED288
    lwz r0, 0x114(r30)
    cmpwi r0, 0x0
    beq @L_800ED290
    lwz r0, 0x118(r30)
    cmplwi r0, 0x0
    beq @L_800ED290
    @L_800ED288:
    li r4, 0x1
    b @L_800ED294
    @L_800ED290:
    li r4, 0x0
    @L_800ED294:
    lwz r3, 0x8c(r30)
    lfs f1, 0x98(r30)
    lfs f2, 0x9c(r30)
    lfs f3, 0xa0(r30)
    lfs f4, 0x94(r30)
    bl fn_800ED8C4
    fmr f31, f1
    @L_800ED2B0:
    lwz r3, 0x0(r30)
    rlwinm. r0, r3, 0, 25, 25
    beq @L_800ED308
    rlwinm r0, r3, 0, 20, 20
    cmplwi r0, 0x800
    beq @L_800ED2E0
    lwz r0, 0x114(r30)
    cmpwi r0, 0x0
    beq @L_800ED2E8
    lwz r0, 0x118(r30)
    cmplwi r0, 0x0
    beq @L_800ED2E8
    @L_800ED2E0:
    li r4, 0x1
    b @L_800ED2EC
    @L_800ED2E8:
    li r4, 0x0
    @L_800ED2EC:
    lwz r3, 0xa4(r30)
    lfs f1, 0xb0(r30)
    lfs f2, 0xb4(r30)
    lfs f3, 0xb8(r30)
    lfs f4, 0xac(r30)
    bl fn_800ED8C4
    fmr f30, f1
    @L_800ED308:
    fmr f1, f31
    lis r3, fn_801C027C@ha
    lis r5, 0x1
    li r4, 0x6
    addi r6, r3, fn_801C027C@l
    mr r3, r31
    subi r5, r5, 0x64d1
    li r7, 0x1
    creqv 6, 6, 6
    bl fn_801C028C
    fmr f1, f30
    lis r3, fn_801C027C@ha
    addi r6, r3, fn_801C027C@l
    li r4, 0x6
    mr r3, r31
    li r5, 0x64db
    li r7, 0x1
    creqv 6, 6, 6
    bl fn_801C028C
    mr r3, r31
    bl fn_801A1B40
    lwz r0, 0x0(r30)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_800ED3A0
    li r0, 0x0
    lis r3, fn_800EE044@ha
    stw r0, 0xc(r1)
    addi r6, r3, fn_800EE044@l
    mr r3, r31
    addi r8, r1, 0xc
    li r4, 0x6
    li r5, 0x20
    li r7, 0x2
    crxor 6, 6, 6
    bl fn_801C028C
    lwz r3, 0xc(r1)
    lfs f0, 0x4(r3)
    stfs f0, 0x9c(r30)
    @L_800ED3A0:
    lwz r0, 0x0(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_800ED3E4
    li r0, 0x0
    lis r3, fn_800EE044@ha
    stw r0, 0x8(r1)
    addi r6, r3, fn_800EE044@l
    mr r3, r31
    addi r8, r1, 0x8
    li r4, 0x6
    li r5, 0x480
    li r7, 0x2
    crxor 6, 6, 6
    bl fn_801C028C
    lwz r3, 0x8(r1)
    lfs f0, 0x4(r3)
    stfs f0, 0xb4(r30)
    @L_800ED3E4:
    lwz r0, 0x0(r30)
    rlwinm. r0, r0, 0, 26, 26
    beq @L_800ED3F8
    mr r3, r30
    bl fn_800EDA98
    @L_800ED3F8:
    lwz r0, 0x0(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800ED454
    lwz r3, 0x8(r30)
    lwz r31, 0x10(r3)
    cmplwi r31, 0x0
    beq @L_800ED454
    bne @L_800ED428
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800ED428:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800ED444
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800ED444
    li r3, 0x1
    @L_800ED444:
    cmpwi r3, 0x0
    beq @L_800ED454
    mr r3, r31
    bl fn_8019D9DC
    @L_800ED454:
    lwz r31, 0x8(r30)
    cmplwi r31, 0x0
    beq @L_800ED4A0
    bne @L_800ED474
    la r3, lbl_8047CC68(r2)
    li r4, 0x25d
    la r5, lbl_8047CC70(r2)
    bl fn_80196E10
    @L_800ED474:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800ED490
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800ED490
    li r3, 0x1
    @L_800ED490:
    cmpwi r3, 0x0
    beq @L_800ED4A0
    mr r3, r31
    bl fn_8019D9DC
    @L_800ED4A0:
    lwz r0, 0x0(r30)
    ori r0, r0, 0x1000
    stw r0, 0x0(r30)
    @L_800ED4AC:
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    psq_l f30, 0x28(r1), 0, 0
    lfd f30, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r0, 0x44(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
