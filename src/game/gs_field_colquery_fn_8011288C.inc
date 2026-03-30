    nofralloc
    lis r5, lbl_80408378@ha
    addi r5, r5, lbl_80408378@l
    stw r4, 0x28(r5)
    stw r3, 0x2c(r5)
    blr
