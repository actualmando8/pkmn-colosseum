    nofralloc
    lis r4, lbl_80267DD8@ha
    slwi r0, r3, 2
    addi r3, r4, lbl_80267DD8@l
    lwzx r3, r3, r0
    blr
