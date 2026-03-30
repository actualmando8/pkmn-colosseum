    nofralloc
    lwz r0, lbl_80478D40(r13)
    clrlwi r5, r3, 16
    lis r4, lbl_80375AC8@ha
    clrlslwi r3, r3, 16, 3
    cmplw r5, r0
    addi r0, r4, lbl_80375AC8@l
    add r3, r0, r3
    bltlr
    li r3, 0x0
    blr
