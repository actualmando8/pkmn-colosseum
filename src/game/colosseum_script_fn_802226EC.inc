    nofralloc
    lwz r4, lbl_8047B610(r13)
    lwz r3, 0x1(r4)
    lbz r4, 0x5(r4)
    lbz r0, 0x0(r3)
    subf r0, r4, r0
    stb r0, 0x0(r3)
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
    blr
