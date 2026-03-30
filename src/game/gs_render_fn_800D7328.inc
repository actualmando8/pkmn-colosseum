    nofralloc
    mulli r0, r3, 0xc
    lwz r4, lbl_8047AA80(r13)
    lis r3, 0xcc01
    add r4, r4, r0
    lbz r0, 0x4e8(r4)
    stb r0, -0x8000(r3)
    blr
