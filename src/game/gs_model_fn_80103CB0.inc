    nofralloc
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lbz r3, 0x92(r3)
    blr
