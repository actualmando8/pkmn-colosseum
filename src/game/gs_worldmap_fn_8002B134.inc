    nofralloc
    stwu r1, -0x20(r1)
    lhz r0, 0x94(r3)
    lwz r3, 0x60(r3)
    sth r0, 0x8(r1)
    lbz r0, 0x8(r1)
    extsb r0, r0
    cmpwi r0, 0x0
    ble @L_8002B18C
    lwz r3, 0x0(r3)
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8002B18C
    lfs f1, lbl_8047B97C(r2)
    lfs f0, lbl_8047A3E8(r13)
    lfs f2, lbl_8047B978(r2)
    fsubs f0, f1, f0
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stb r0, 0x67(r4)
    b @L_8002B194
    @L_8002B18C:
    li r0, 0x0
    stb r0, 0x67(r4)
    @L_8002B194:
    li r3, 0x0
    addi r1, r1, 0x20
    blr
