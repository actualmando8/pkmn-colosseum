    nofralloc
    lfs f4, lbl_8047C9F0(r2)
    li r0, 0x1
    lfs f1, 0x0(r3)
    stwu r1, -0x30(r1)
    fmuls f2, f4, f1
    lfs f0, 0x4(r3)
    lwz r4, lbl_8047AA80(r13)
    lfs f1, 0x8(r3)
    fmuls f0, f4, f0
    fctiwz f3, f2
    fmuls f1, f4, f1
    stb r0, 0x19(r4)
    fctiwz f2, f0
    lfs f0, 0xc(r3)
    stfd f3, 0x8(r1)
    lwz r3, lbl_8047AA80(r13)
    lwz r0, 0xc(r1)
    fctiwz f1, f1
    stfd f2, 0x10(r1)
    fmuls f0, f4, f0
    stb r0, 0x1c(r3)
    lwz r0, 0x14(r1)
    fctiwz f0, f0
    lwz r3, lbl_8047AA80(r13)
    stfd f1, 0x18(r1)
    stb r0, 0x1d(r3)
    lwz r0, 0x1c(r1)
    lwz r3, lbl_8047AA80(r13)
    stfd f0, 0x20(r1)
    stb r0, 0x1e(r3)
    lwz r0, 0x24(r1)
    lwz r3, lbl_8047AA80(r13)
    stb r0, 0x1f(r3)
    lwz r3, lbl_8047AA80(r13)
    lbz r0, 0x1c(r3)
    cmplwi r0, 0x0
    bne @L_800D3774
    lbz r0, 0x1d(r3)
    cmplwi r0, 0x0
    bne @L_800D3774
    lbz r0, 0x1e(r3)
    cmplwi r0, 0x0
    bne @L_800D3774
    lbz r0, 0x1f(r3)
    cmplwi r0, 0x0
    bne @L_800D3774
    li r0, 0x0
    stb r0, 0x19(r3)
    @L_800D3774:
    addi r1, r1, 0x30
    blr
