    nofralloc
    lwz r3, lbl_8047B610(r13)
    lwz r4, 0x1(r3)
    lbz r0, 0x5(r3)
    lbz r3, 0x0(r4)
    or r0, r3, r0
    stb r0, 0x0(r4)
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
    blr
