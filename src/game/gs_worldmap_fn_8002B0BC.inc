    nofralloc
    stwu r1, -0x20(r1)
    lhz r0, 0x94(r3)
    lwz r6, 0x60(r3)
    sth r0, 0x8(r1)
    lwz r3, 0x8(r6)
    lbz r5, 0x8(r1)
    addi r0, r3, 0x1
    extsb r3, r5
    addi r3, r3, 0xa
    cmpw r3, r0
    bge @L_8002B120
    lwz r3, 0x0(r6)
    lhz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_8002B120
    lfs f1, lbl_8047B97C(r2)
    lfs f0, lbl_8047A3E8(r13)
    lfs f2, lbl_8047B978(r2)
    fsubs f0, f1, f0
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    stb r0, 0x67(r4)
    b @L_8002B128
    @L_8002B120:
    li r0, 0x0
    stb r0, 0x67(r4)
    @L_8002B128:
    li r3, 0x0
    addi r1, r1, 0x20
    blr
