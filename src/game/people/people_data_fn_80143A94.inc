    nofralloc
    lwz r0, lbl_80478BE0(r13)
    clrlwi r5, r3, 24
    lis r4, lbl_80368630@ha
    clrlslwi r3, r3, 24, 4
    cmplw r5, r0
    addi r0, r4, lbl_80368630@l
    add r3, r0, r3
    bltlr
    li r3, 0x0
    blr
