    nofralloc
    mulli r0, r3, 0xc
    lwz r4, lbl_8047AA80(r13)
    lis r3, 0xcc01
    add r7, r4, r0
    lbz r6, 0x4eb(r7)
    lbz r5, 0x4ea(r7)
    lbz r4, 0x4e9(r7)
    lbz r0, 0x4e8(r7)
    stb r0, -0x8000(r3)
    stb r4, -0x8000(r3)
    stb r5, -0x8000(r3)
    stb r6, -0x8000(r3)
    blr
