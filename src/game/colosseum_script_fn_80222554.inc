    nofralloc
    lwz r3, lbl_8047B610(r13)
    li r0, -0x1
    lwz r5, 0x1(r3)
    lwz r3, 0x5(r3)
    lwz r4, 0x0(r5)
    xor r0, r3, r0
    and r0, r4, r0
    stw r0, 0x0(r5)
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x9
    stw r0, lbl_8047B610(r13)
    blr
