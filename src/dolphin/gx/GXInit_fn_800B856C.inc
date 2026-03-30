    nofralloc
    li r0, 0x48
    lis r3, 0xcc01
    stb r0, -0x8000(r3)
    blr
