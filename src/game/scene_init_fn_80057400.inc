    nofralloc
    lis r3, lbl_803A9768@ha
    addi r4, r3, lbl_803A9768@l
    lwz r3, 0x278(r4)
    addi r0, r3, 0x1
    cmpwi r0, 0x1
    stw r0, 0x278(r4)
    blelr
    li r0, 0x0
    stw r0, 0x278(r4)
    blr
