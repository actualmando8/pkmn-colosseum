    nofralloc
    lis r3, lbl_803A9768@ha
    addi r3, r3, lbl_803A9768@l
    lwz r0, 0x278(r3)
    mulli r0, r0, 0x138
    add r3, r3, r0
    addi r3, r3, 0x8
    blr
