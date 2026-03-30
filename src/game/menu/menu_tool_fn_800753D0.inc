    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x1c(r1)
    lfd f1, lbl_8047C0B0(r2)
    stw r0, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x24(r1)
    lfd f1, lbl_8047C0B8(r2)
    stw r0, 0x20(r1)
    lwz r3, lbl_8047A610(r13)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fdivs f0, f0, f31
    stfs f0, 0x0(r3)
    lwz r3, lbl_8047A610(r13)
    lfs f1, 0x4(r3)
    bl fn_800CE148
    frsp f2, f1
    lfs f1, lbl_8047C09C(r2)
    lfs f0, lbl_8047C098(r2)
    lwz r3, lbl_8047A610(r13)
    fmadds f1, f1, f2, f0
    lfs f0, lbl_8047C0A0(r2)
    stfs f1, 0x18c(r3)
    lwz r3, lbl_8047A610(r13)
    lfs f1, 0x18c(r3)
    fcmpo cr0, f1, f0
    ble @L_80075468
    stfs f0, 0x18c(r3)
@L_80075468:
    lwz r3, lbl_8047A610(r13)
    li r4, 0x0
    addi r3, r3, 0x144
    bl fn_80109B90
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800754CC
    lwz r3, lbl_8047A610(r13)
    lwz r3, 0x168(r3)
    bl fn_801DAC3C
    cmplwi r3, 0x0
    beq @L_800754CC
    lwz r5, lbl_8047A610(r13)
    addi r4, r1, 0x8
    lfs f2, lbl_8047C0A4(r2)
    lfs f0, 0x0(r5)
    lfs f1, 0x8(r5)
    fmuls f2, f2, f0
    lfs f0, lbl_8047C0A8(r2)
    fadds f1, f1, f2
    stfs f1, 0x8(r5)
    stfs f0, 0x8(r1)
    stfs f0, 0x10(r1)
    stfs f2, 0xc(r1)
    bl fn_800E3DC4
@L_800754CC:
    lwz r3, lbl_8047A610(r13)
    lfs f0, lbl_8047C0AC(r2)
    lfs f2, 0x4(r3)
    lfs f1, 0x0(r3)
    fadds f1, f2, f1
    stfs f1, 0x4(r3)
    lwz r3, lbl_8047A610(r13)
    lfs f1, 0x4(r3)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_80075500
    lfs f0, lbl_8047C0A8(r2)
    stfs f0, 0x4(r3)
@L_80075500:
    psq_l f31, 0x38(r1), 0, 0
    lwz r0, 0x44(r1)
    lfd f31, 0x30(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
