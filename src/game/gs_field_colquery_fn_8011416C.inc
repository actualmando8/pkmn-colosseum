    nofralloc
    lis r6, lbl_80408378@ha
    li r0, 0x1
    addi r6, r6, lbl_80408378@l
    stw r3, 0x48(r6)
    stw r4, 0x4c(r6)
    stb r5, 0x50(r6)
    stb r0, 0x51(r6)
    blr
