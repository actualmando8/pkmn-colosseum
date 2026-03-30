    nofralloc
    lwz r3, lbl_8047B610(r13)
    lwz r4, 0x1(r3)
    lhz r0, 0x5(r3)
    lhz r3, 0x0(r4)
    xori r0, r0, 0xffff
    and r0, r3, r0
    sth r0, 0x0(r4)
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
    blr
