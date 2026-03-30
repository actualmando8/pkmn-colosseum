    nofralloc
    lis r4, lbl_804673F8@ha
    li r3, 0x0
    addi r5, r4, lbl_804673F8@l
    lbz r4, 0x8(r5)
    extlwi r0, r4, 2, 24
    srawi r0, r0, 31
    xori r0, r0, 0x1
    extsb r0, r0
    rlwimi r4, r0, 7, 24, 24
    stb r4, 0x8(r5)
    blr
