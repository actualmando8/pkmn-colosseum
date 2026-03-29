    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r4
    lis r4, lbl_803A6748@ha
    lfs f1, lbl_8047BAC0(r2)
    addi r4, r4, lbl_803A6748@l
    lfs f0, 0x44(r4)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r3)
    lwz r3, 0xc(r4)
    cmpwi r3, 0xa
    blt @L_8003C0B4
    lwz r0, 0x10(r4)
    cmpw r0, r3
    beq @L_8003C0A0
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8003C0C8
    @L_8003C0A0:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8003C128
    @L_8003C0B4:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_8003C128
    @L_8003C0C8:
    lfs f1, lbl_8047BAF4(r2)
    lfs f0, 0x28(r4)
    lfs f2, lbl_8047BAF0(r2)
    fmuls f0, f1, f0
    fmuls f0, f1, f0
    fmuls f1, f2, f0
    bl fn_800CE148
    lis r3, lbl_802EF0A8@ha
    lis r0, 0x4330
    addi r3, r3, lbl_802EF0A8@l
    stw r0, 0x8(r1)
    lha r0, 0x504c(r3)
    frsp f3, f1
    lfd f1, lbl_8047BAE0(r2)
    xoris r0, r0, 0x8000
    lfs f2, lbl_8047BAF4(r2)
    stw r0, 0xc(r1)
    lfd f0, 0x8(r1)
    fsubs f0, f0, f1
    fmadds f0, f2, f3, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x52(r31)
    @L_8003C128:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
