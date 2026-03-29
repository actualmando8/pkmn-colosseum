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
    lfs f1, lbl_8047CD58(r2)
    lfs f0, lbl_80478B10(r13)
    lfs f3, lbl_8047CD5C(r2)
    lfs f2, lbl_80478B14(r13)
    fdivs f29, f1, f0
    lfs f0, lbl_8047CD60(r2)
    lfs f1, lbl_8047CD64(r2)
    fdivs f28, f3, f2
    fmuls f31, f29, f0
    fmuls f30, f28, f0
    bl fn_800CE220
    frsp f0, f1
    lis r4, 0x4330
    lha r5, lbl_8047AC70(r13)
    addi r3, r1, 0x20
    lha r0, lbl_8047AC72(r13)
    fdivs f3, f30, f0
    xoris r5, r5, 0x8000
    xoris r0, r0, 0x8000
    stw r5, 0x64(r1)
    lfd f2, lbl_8047CD50(r2)
    stw r4, 0x60(r1)
    lfd f0, 0x60(r1)
    stw r0, 0x6c(r1)
    fsubs f1, f0, f2
    stw r4, 0x68(r1)
    lfd f0, 0x68(r1)
    fsubs f1, f31, f1
    fsubs f0, f0, f2
    fsubs f2, f30, f0
    bl fn_800E01F4
    lha r5, lbl_8047AC70(r13)
    lis r4, 0x4330
    lha r0, lbl_8047AC72(r13)
    addi r3, r1, 0x14
    xoris r5, r5, 0x8000
    stw r4, 0x70(r1)
    xoris r0, r0, 0x8000
    lfd f2, lbl_8047CD50(r2)
    stw r5, 0x74(r1)
    lfs f3, lbl_8047CD68(r2)
    lfd f0, 0x70(r1)
    stw r0, 0x7c(r1)
    fsubs f1, f0, f2
    stw r4, 0x78(r1)
    lfd f0, 0x78(r1)
    fsubs f1, f31, f1
    fsubs f0, f0, f2
    fsubs f2, f30, f0
    bl fn_800E01F4
    lfs f1, lbl_8047CD68(r2)
    addi r3, r1, 0x8
    lfs f2, lbl_8047CD6C(r2)
    fmr f3, f1
    bl fn_800E01F4
    addi r3, r1, 0x2c
    addi r4, r1, 0x20
    addi r5, r1, 0x8
    addi r6, r1, 0x14
    bl fn_800E0218
    fdivs f0, f29, f28
    lfs f1, lbl_8047CD70(r2)
    lfs f3, lbl_8047CD74(r2)
    lfs f4, lbl_8047CD78(r2)
    fneg f2, f0
    bl fn_800D9BD0
    bl fn_800D834C
    addi r3, r1, 0x2c
    bl fn_800D7FE4
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    lis r3, 0x8000
    bl fn_800D888C
    li r3, 0x2
    li r4, 0x2
    li r5, 0x1
    bl fn_800DA2BC
    li r3, 0x0
    li r4, 0x7
    li r5, 0x0
    li r6, 0x1
    li r7, 0x7
    li r8, 0x0
    bl fn_800DA100
    li r3, 0x0
    li r4, 0x2
    li r5, 0x0
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lwz r0, 0xc4(r1)
    lfd f28, 0x80(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
