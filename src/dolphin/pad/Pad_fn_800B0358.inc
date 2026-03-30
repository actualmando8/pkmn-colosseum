    nofralloc
    cmpwi r3, 0x0
    blt @L_800B0368
    cmpwi r3, 0x2
    blt @L_800B0370
    @L_800B0368:
    li r3, -0x80
    blr
    @L_800B0370:
    mulli r4, r3, 0x110
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    add r3, r0, r4
    lwz r3, 0x4(r3)
    blr
