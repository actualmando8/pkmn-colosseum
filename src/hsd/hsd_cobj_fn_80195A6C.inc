    nofralloc
    stwu r1, -0x110(r1)
    mflr r0
    stw r0, 0x114(r1)
    stfd f31, 0x100(r1)
    psq_st f31, 0x108(r1), 0, 0
    stfd f30, 0xf0(r1)
    psq_st f30, 0xf8(r1), 0, 0
    stfd f29, 0xe0(r1)
    psq_st f29, 0xe8(r1), 0, 0
    stfd f28, 0xd0(r1)
    psq_st f28, 0xd8(r1), 0, 0
    stfd f27, 0xc0(r1)
    psq_st f27, 0xc8(r1), 0, 0
    stmw r27, 0xac(r1)
    mr. r30, r3
    bne @L_80195AB4
    li r3, 0x0
    b @L_80195ED0
@L_80195AB4:
    bl fn_8019C7B0
    mr r28, r3
    bl fn_80197400
    cmpwi r28, 0x2
    stw r30, lbl_8047B234(r13)
    beq @L_80195E8C
    bge @L_80195AE0
    cmpwi r28, 0x0
    beq @L_80195BEC
    bge @L_80195E80
    b @L_80195E98
@L_80195AE0:
    cmpwi r28, 0x4
    bge @L_80195E98
    lfs f3, 0x10(r30)
    li r3, 0x0
    lfs f2, 0xc(r30)
    lfs f1, 0x18(r30)
    lfs f0, 0x14(r30)
    fsubs f3, f3, f2
    lwz r12, lbl_80478C58(r13)
    fsubs f4, f1, f0
    lfs f1, 0xc(r30)
    lfs f2, 0x14(r30)
    lfs f5, -23848(r2)
    lfs f6, -23792(r2)
    mtctr r12
    bctrl
    lhz r4, 0x1c(r30)
    lhz r3, 0x1e(r30)
    lhz r6, 0x20(r30)
    lhz r0, 0x22(r30)
    subf r5, r4, r3
    lhz r3, 0x1c(r30)
    lhz r4, 0x20(r30)
    subf r6, r6, r0
    bl fn_800BD7A0
    lbz r0, 0x50(r30)
    cmpwi r0, 0x2
    beq @L_80195B8C
    bge @L_80195B60
    cmpwi r0, 0x1
    bge @L_80195B6C
    b @L_80195BD8
@L_80195B60:
    cmpwi r0, 0x4
    bge @L_80195BD8
    b @L_80195BB4
@L_80195B6C:
    lfs f1, 0x40(r30)
    addi r3, r1, 0x48
    lfs f2, 0x44(r30)
    li r27, 0x0
    lfs f3, 0x38(r30)
    lfs f4, 0x3c(r30)
    bl C_MTXPerspective
    b @L_80195BD8
@L_80195B8C:
    lfs f1, 0x40(r30)
    addi r3, r1, 0x48
    lfs f2, 0x44(r30)
    li r27, 0x0
    lfs f3, 0x48(r30)
    lfs f4, 0x4c(r30)
    lfs f5, 0x38(r30)
    lfs f6, 0x3c(r30)
    bl fn_800A3874
    b @L_80195BD8
@L_80195BB4:
    lfs f1, 0x40(r30)
    addi r3, r1, 0x48
    lfs f2, 0x44(r30)
    li r27, 0x1
    lfs f3, 0x48(r30)
    lfs f4, 0x4c(r30)
    lfs f5, 0x38(r30)
    lfs f6, 0x3c(r30)
    bl fn_800A39E0
@L_80195BD8:
    addi r3, r1, 0x48
    mr r4, r27
    bl fn_800BD2E0
    li r3, 0x1
    b @L_80195EB4
@L_80195BEC:
    lis r4, lbl_80466BC0@ha
    lwz r3, lbl_80478C50(r13)
    addi r10, r4, lbl_80466BC0@l
    lis r8, 0x4330
    lhz r9, 0x4(r10)
    xoris r7, r3, 0x8000
    lis r6, 0x4330
    lwz r0, lbl_80478C54(r13)
    stw r9, 0x8c(r1)
    lis r5, 0x4330
    xoris r4, r0, 0x8000
    lhz r0, 0x6(r10)
    stw r8, 0x88(r1)
    lis r3, 0x4330
    lfd f2, -23776(r2)
    lfd f0, 0x88(r1)
    stw r7, 0x94(r1)
    lfd f1, -23856(r2)
    fsub f2, f0, f2
    stw r6, 0x90(r1)
    lfd f6, -23776(r2)
    lfd f0, 0x90(r1)
    stw r0, 0x9c(r1)
    fsub f1, f0, f1
    lbz r0, 0x18(r10)
    stw r5, 0x98(r1)
    lfd f4, -23856(r2)
    cmplwi r0, 0x0
    fdiv f30, f2, f1
    lfd f5, 0x98(r1)
    lfs f2, 0x10(r30)
    lfs f0, 0xc(r30)
    stw r4, 0xa4(r1)
    lfs f1, 0x14(r30)
    stw r3, 0xa0(r1)
    fmul f7, f2, f30
    lfd f3, 0xa0(r1)
    fsub f5, f5, f6
    fmul f28, f0, f30
    lfs f0, 0x18(r30)
    fsub f2, f3, f4
    frsp f7, f7
    frsp f28, f28
    fdiv f31, f5, f2
    fmul f27, f1, f31
    fmul f4, f0, f31
    frsp f27, f27
    fsubs f29, f7, f28
    frsp f4, f4
    fsubs f4, f4, f27
    beq @L_80195CE0
    lwz r12, lbl_80478C58(r13)
    fmr f1, f28
    fmr f2, f27
    lfs f5, -23848(r2)
    fmr f3, f29
    lfs f6, -23792(r2)
    li r3, 0x1
    mtctr r12
    bctrl
    b @L_80195D04
@L_80195CE0:
    lwz r12, lbl_80478C58(r13)
    fmr f1, f28
    fmr f2, f27
    lfs f5, -23848(r2)
    fmr f3, f29
    lfs f6, -23792(r2)
    li r3, 0x0
    mtctr r12
    bctrl
@L_80195D04:
    lhz r5, 0x20(r30)
    lis r4, 0x4330
    lhz r3, 0x22(r30)
    lis r0, 0x4330
    stw r5, 0x94(r1)
    lis r6, 0x4330
    lhz r7, 0x1c(r30)
    lis r5, 0x4330
    stw r4, 0x90(r1)
    lhz r4, 0x1e(r30)
    stw r3, 0x8c(r1)
    lfd f3, -23776(r2)
    stw r0, 0x88(r1)
    lfd f2, 0x90(r1)
    lfd f1, -23776(r2)
    lfd f0, 0x88(r1)
    fsub f2, f2, f3
    stw r7, 0xa4(r1)
    fsub f0, f0, f1
    lfd f3, -23776(r2)
    stw r6, 0xa0(r1)
    fmul f27, f2, f31
    lfd f1, -23776(r2)
    lfd f2, 0xa0(r1)
    fmul f4, f0, f31
    stw r4, 0x9c(r1)
    stw r5, 0x98(r1)
    fsub f2, f2, f3
    frsp f27, f27
    lfd f0, 0x98(r1)
    fmul f28, f2, f30
    fsub f0, f0, f1
    frsp f4, f4
    frsp f28, f28
    fmul f7, f0, f30
    fsubs f4, f4, f27
    frsp f7, f7
    fmr f1, f4
    fsubs f29, f7, f28
    bl fn_800C46B0
    fmr f1, f29
    mr r27, r3
    bl fn_800C46B0
    fmr f1, f27
    mr r28, r3
    bl fn_800C46B0
    fmr f1, f28
    mr r29, r3
    bl fn_800C46B0
    mr r4, r29
    mr r5, r28
    mr r6, r27
    bl fn_800BD7A0
    lbz r0, 0x50(r30)
    cmpwi r0, 0x2
    beq @L_80195E20
    bge @L_80195DF4
    cmpwi r0, 0x1
    bge @L_80195E00
    b @L_80195E6C
@L_80195DF4:
    cmpwi r0, 0x4
    bge @L_80195E6C
    b @L_80195E48
@L_80195E00:
    lfs f1, 0x40(r30)
    addi r3, r1, 0x8
    lfs f2, 0x44(r30)
    li r31, 0x0
    lfs f3, 0x38(r30)
    lfs f4, 0x3c(r30)
    bl C_MTXPerspective
    b @L_80195E6C
@L_80195E20:
    lfs f1, 0x40(r30)
    addi r3, r1, 0x8
    lfs f2, 0x44(r30)
    li r31, 0x0
    lfs f3, 0x48(r30)
    lfs f4, 0x4c(r30)
    lfs f5, 0x38(r30)
    lfs f6, 0x3c(r30)
    bl fn_800A3874
    b @L_80195E6C
@L_80195E48:
    lfs f1, 0x40(r30)
    addi r3, r1, 0x8
    lfs f2, 0x44(r30)
    li r31, 0x1
    lfs f3, 0x48(r30)
    lfs f4, 0x4c(r30)
    lfs f5, 0x38(r30)
    lfs f6, 0x3c(r30)
    bl fn_800A39E0
@L_80195E6C:
    addi r3, r1, 0x8
    mr r4, r31
    bl fn_800BD2E0
    li r3, 0x1
    b @L_80195EB4
@L_80195E80:
    mr r3, r30
    bl fn_801963E0
    b @L_80195EB4
@L_80195E8C:
    mr r3, r30
    bl fn_801960C4
    b @L_80195EB4
@L_80195E98:
    lis r4, lbl_80274680@ha
    la r3, -23880(r2)
    addi r5, r4, lbl_80274680@l
    li r4, 0x2ab
    bl HSD_Panic
    li r3, 0x0
    b @L_80195ED0
@L_80195EB4:
    cmpwi r3, 0x0
    bne @L_80195EC4
    li r3, 0x0
    b @L_80195ED0
@L_80195EC4:
    mr r3, r30
    bl fn_80195F0C
    li r3, 0x1
@L_80195ED0:
    psq_l f31, 0x108(r1), 0, 0
    lfd f31, 0x100(r1)
    psq_l f30, 0xf8(r1), 0, 0
    lfd f30, 0xf0(r1)
    psq_l f29, 0xe8(r1), 0, 0
    lfd f29, 0xe0(r1)
    psq_l f28, 0xd8(r1), 0, 0
    lfd f28, 0xd0(r1)
    psq_l f27, 0xc8(r1), 0, 0
    lfd f27, 0xc0(r1)
    lmw r27, 0xac(r1)
    lwz r0, 0x114(r1)
    mtlr r0
    addi r1, r1, 0x110
    blr
