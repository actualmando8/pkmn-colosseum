    nofralloc
    mulli r4, r4, 0x1c
    li r0, 0x1
    add r3, r3, r4
    stb r0, 0x8(r3)
    stw r5, 0xc(r3)
    stw r6, 0x10(r3)
    stw r7, 0x14(r3)
    stb r8, 0x18(r3)
    stw r9, 0x1c(r3)
    stb r10, 0x20(r3)
    blr
