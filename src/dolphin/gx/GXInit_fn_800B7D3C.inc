    nofralloc
    lwz r3, -29632(r2)
    li r4, 0x0
    stw r4, 0x14(r3)
    lwz r0, 0x14(r3)
    rlwinm r0, r0, 0, 23, 20
    ori r0, r0, 0x200
    stw r0, 0x14(r3)
    stw r4, 0x18(r3)
    stb r4, 0x41c(r3)
    stb r4, 0x41d(r3)
    lwz r0, 0x4f4(r3)
    ori r0, r0, 0x8
    stw r0, 0x4f4(r3)
    blr
