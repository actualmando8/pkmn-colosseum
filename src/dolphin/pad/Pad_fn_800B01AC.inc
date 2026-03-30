    nofralloc
    mulli r0, r3, 0x110
    lis r3, lbl_803FC620@ha
    addi r3, r3, lbl_803FC620@l
    add r3, r3, r0
    lwz r3, 0x10c(r3)
    blr
