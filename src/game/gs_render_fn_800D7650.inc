    nofralloc
    lwz r4, lbl_8047AA80(r13)
    lwz r0, 0x24(r4)
    cmplw r0, r3
    bne @L_800D7668
    li r0, 0x0
    stw r0, 0x24(r4)
    @L_800D7668:
    li r0, 0x0
    stb r0, 0x8(r3)
    stb r0, 0x24(r3)
    stb r0, 0x40(r3)
    stb r0, 0x5c(r3)
    stb r0, 0x78(r3)
    stb r0, 0x94(r3)
    stb r0, 0xb0(r3)
    stb r0, 0xcc(r3)
    stb r0, 0xe8(r3)
    stb r0, 0x104(r3)
    stb r0, 0x120(r3)
    stb r0, 0x13c(r3)
    stb r0, 0x158(r3)
    stb r0, 0x174(r3)
    blr
