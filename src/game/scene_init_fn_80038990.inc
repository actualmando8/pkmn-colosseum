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
    lfd f29, lbl_8047BA80(r2)
    lis r31, 0x4330
    lfd f31, lbl_8047BA88(r2)
    @L_800389C4:
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r31, 0x8(r1)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f30, f0, f29
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r31, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f31
    fdivs f0, f0, f30
    stfs f0, lbl_8047A494(r13)
    bl fn_800F0308
    b @L_800389C4
