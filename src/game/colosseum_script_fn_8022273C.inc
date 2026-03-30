    nofralloc
    lwz r3, lbl_8047B610(r13)
    lbz r0, 0x5(r3)
    lwz r3, 0x1(r3)
    stb r0, 0x0(r3)
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
    blr
