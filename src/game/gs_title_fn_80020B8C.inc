    nofralloc
    lis r3, lbl_803A1FC8@ha
    addi r3, r3, lbl_803A1FC8@l
    lfs f0, 0x10(r3)
    stfs f0, 0x70(r4)
    blr
