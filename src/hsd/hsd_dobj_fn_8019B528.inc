    nofralloc
    cmplwi r3, 0x0
    stwu r1, -0x10(r1)
    beq @L_8019B5E0
    b @L_8019B5D8
    @L_8019B538:
    cmplwi r3, 0x0
    beq @L_8019B5D4
    lwz r4, 0x8(r3)
    lis r0, 0x4330
    stw r0, 0x8(r1)
    li r6, 0x0
    lfd f6, lbl_8047DA58(r2)
    li r5, 0x0
    stw r4, 0x4(r3)
    li r4, 0x0
    li r0, 0x0
    lfs f4, lbl_8047DA3C(r2)
    lha r7, 0x18(r3)
    cmplwi r3, 0x0
    lfs f3, lbl_8047DA3C(r2)
    xoris r7, r7, 0x8000
    lfs f2, lbl_8047DA3C(r2)
    stw r7, 0xc(r1)
    lfs f0, lbl_8047DA3C(r2)
    lfd f5, 0x8(r1)
    fsubs f5, f5, f6
    fadds f5, f5, f1
    stfs f5, 0x1c(r3)
    stb r6, 0x11(r3)
    stb r5, 0x12(r3)
    lbz r5, 0x10(r3)
    rlwinm r5, r5, 0, 26, 24
    stb r5, 0x10(r3)
    sth r4, 0x16(r3)
    sth r0, 0x1a(r3)
    stfs f4, 0x20(r3)
    stfs f3, 0x24(r3)
    stfs f2, 0x28(r3)
    stfs f0, 0x2c(r3)
    beq @L_8019B5D4
    lbz r0, 0x10(r3)
    rlwinm r0, r0, 0, 24, 27
    ori r0, r0, 0x1
    stb r0, 0x10(r3)
    @L_8019B5D4:
    lwz r3, 0x0(r3)
    @L_8019B5D8:
    cmplwi r3, 0x0
    bne @L_8019B538
    @L_8019B5E0:
    addi r1, r1, 0x10
    blr
