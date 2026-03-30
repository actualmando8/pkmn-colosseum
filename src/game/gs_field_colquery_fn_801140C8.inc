    nofralloc
    lis r3, lbl_80408378@ha
    li r0, 0x0
    addi r3, r3, lbl_80408378@l
    stb r0, 0x51(r3)
    blr
