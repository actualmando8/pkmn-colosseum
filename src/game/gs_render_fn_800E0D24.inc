    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stfd f29, 0x20(r1)
    psq_st f29, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lis r3, lbl_804011B8@ha
    lfs f29, lbl_8047CB38(r2)
    lfs f30, lbl_8047CB30(r2)
    addi r30, r3, lbl_804011B8@l
    lfd f31, lbl_8047CB40(r2)
    li r29, 0x0
    lis r31, 0x4330
    @L_800E0D70:
    xoris r0, r29, 0x8000
    stw r31, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f31
    fmuls f0, f30, f0
    fmuls f1, f29, f0
    bl fn_800CDBE0
    frsp f0, f1
    addi r29, r29, 0x1
    cmpwi r29, 0xb5
    stfs f0, 0x0(r30)
    addi r30, r30, 0x4
    blt @L_800E0D70
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    psq_l f29, 0x28(r1), 0, 0
    lfd f29, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r0, 0x54(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
