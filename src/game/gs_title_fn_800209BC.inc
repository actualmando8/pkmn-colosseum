    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lis r3, lbl_803A1FC8@ha
    lfs f1, lbl_8047B874(r2)
    addi r3, r3, lbl_803A1FC8@l
    lfs f0, 0x24(r3)
    fmuls f1, f1, f0
    bl fn_800CE148
    frsp f2, f1
    lis r3, lbl_803A1FC8@ha
    lfs f1, lbl_8047B88C(r2)
    addi r3, r3, lbl_803A1FC8@l
    lfs f0, lbl_8047B888(r2)
    fmadds f0, f1, f2, f0
    stfs f0, 0x6c(r31)
    stfs f0, 0x68(r31)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x1
    bne @L_80020A28
    lbz r0, 0x4(r31)
    ori r0, r0, 0x2
    extsb r0, r0
    stb r0, 0x4(r31)
    b @L_80020A38
    @L_80020A28:
    lbz r0, 0x4(r31)
    rlwinm r0, r0, 0, 31, 29
    extsb r0, r0
    stb r0, 0x4(r31)
    @L_80020A38:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
