    nofralloc
    li r0, 0x0
    cmpwi r0, 0xc
    bgelr
    lis r3, lbl_80404A98@ha
    addi r3, r3, lbl_80404A98@l
    li r0, 0x0
    addi r4, r3, 0xc
    stb r0, 0x0(r3)
    stb r0, 0x1(r3)
    stb r0, 0x2(r3)
    stb r0, 0x3(r3)
    stb r0, 0x4(r3)
    stb r0, 0x5(r3)
    stb r0, 0x6(r3)
    stb r0, 0x7(r3)
    stb r0, 0x8(r3)
    stb r0, 0x9(r3)
    stb r0, 0xa(r3)
    stb r0, 0xb(r3)
    stb r0, 0x0(r4)
    stb r0, 0x1(r4)
    stb r0, 0x2(r4)
    stb r0, 0x3(r4)
    stb r0, 0x4(r4)
    stb r0, 0x5(r4)
    stb r0, 0x6(r4)
    stb r0, 0x7(r4)
    stb r0, 0x8(r4)
    stb r0, 0x9(r4)
    stb r0, 0xa(r4)
    stb r0, 0xb(r4)
    blr
