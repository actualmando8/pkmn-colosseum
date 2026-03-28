    nofralloc
    lwz r3, -21104(r13)
    li r0, 0x1
    stb r0, 0x6a9(r3)
    lwz r4, -21104(r13)
    lwz r3, 0x69c(r4)
    addi r0, r3, 0x2
    stw r0, 0x69c(r4)
    lwz r5, -21104(r13)
    lwz r4, 0x69c(r5)
    lbz r3, 0x0(r4)
    lbz r0, 0x1(r4)
    rlwimi r0, r3, 8, 16, 23
    sth r0, 0x6aa(r5)
    lwz r4, -21104(r13)
    lwz r3, 0x69c(r4)
    addi r0, r3, 0x2
    stw r0, 0x69c(r4)
    lwz r3, -21104(r13)
    lhz r0, 0x6aa(r3)
    sth r0, 0x6ac(r3)
    blr
