    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x4
    stw r0, 0x14(r1)
    oris r3, r3, 0x4
    mtspr GQR2, r3
    li r3, 0x5
    oris r3, r3, 0x5
    mtspr GQR3, r3
    li r3, 0x6
    oris r3, r3, 0x6
    mtspr GQR4, r3
    li r3, 0x7
    oris r3, r3, 0x7
    mtspr GQR5, r3
    bl fn_800E0C78
    bl fn_800E0D24
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
