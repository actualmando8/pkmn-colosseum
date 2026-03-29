    nofralloc
    lbz r0, 0x95(r3)
    lis r5, lbl_80266918@ha
    addi r6, r5, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    lbzx r0, r6, r0
    stb r0, 0x64(r4)
    lbz r0, 0x95(r3)
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r5, r6, r0
    lbz r0, 0x1(r5)
    stb r0, 0x65(r4)
    lbz r0, 0x95(r3)
    li r3, 0x0
    extsb r0, r0
    mulli r0, r0, 0x4c
    add r5, r6, r0
    lbz r0, 0x2(r5)
    stb r0, 0x66(r4)
    blr
