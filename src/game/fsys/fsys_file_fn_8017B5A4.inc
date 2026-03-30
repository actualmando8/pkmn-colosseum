    nofralloc
    stwu r1, -0x10(r1)
    stw r31, 0xc(r1)
    extrwi r31, r3, 6, 17
    mr r3, r31
    lwz r31, 0xc(r1)
    addi r1, r1, 0x10
    blr
