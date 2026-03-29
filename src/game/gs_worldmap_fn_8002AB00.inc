    nofralloc
    lwz r3, 0x60(r3)
    lis r5, lbl_80266E70@ha
    addi r6, r5, lbl_80266E70@l
    li r0, 0xff
    lbz r5, 0x1c(r3)
    li r3, 0x0
    mulli r7, r5, 0x3
    lbzx r5, r6, r7
    add r7, r6, r7
    lbz r6, 0x1(r7)
    stb r5, 0x64(r4)
    lbz r5, 0x2(r7)
    stb r6, 0x65(r4)
    stb r5, 0x66(r4)
    stb r0, 0x67(r4)
    blr
