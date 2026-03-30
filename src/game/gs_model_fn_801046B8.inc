    nofralloc
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r3, 0x4(r3)
    blr
