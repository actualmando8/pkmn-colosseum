    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x10(r1)
    psq_st f31, 0x18(r1), 0, 0
    lfs f0, lbl_8047CB20(r2)
    lfs f31, lbl_8047CB1C(r2)
    fcmpo cr0, f1, f0
    ble @L_800E0CD4
    lfs f31, lbl_8047CB24(r2)
    lfd f2, lbl_8047CB28(r2)
    bl fn_800CE318
    frsp f1, f1
    @L_800E0CD4:
    lfs f0, lbl_8047CB18(r2)
    fcmpo cr0, f1, f0
    ble @L_800E0CE8
    lfs f0, lbl_8047CB20(r2)
    fsubs f1, f0, f1
    @L_800E0CE8:
    lfs f2, lbl_8047CB34(r2)
    lfs f0, lbl_8047CB30(r2)
    fmadds f1, f2, f1, f0
    bl fn_800C46B0
    lis r4, lbl_804011B8@ha
    slwi r0, r3, 2
    addi r3, r4, lbl_804011B8@l
    lfsx f0, r3, r0
    fmuls f1, f31, f0
    psq_l f31, 0x18(r1), 0, 0
    lwz r0, 0x24(r1)
    lfd f31, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
