    nofralloc
    lis r4, lbl_80404ACC@ha
    addi r4, r4, lbl_80404ACC@l
    lbz r0, 0x92(r4)
    stb r3, 0x92(r4)
    mr r3, r0
    blr
