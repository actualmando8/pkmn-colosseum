    nofralloc
    cmplwi r3, 0x0
    bne @L_801FBEE0
    li r3, 0x0
    blr
    @L_801FBEE0:
    clrlwi r0, r4, 24
    cmplwi r0, 0x4
    ble @L_801FBEF4
    li r3, 0x0
    blr
    @L_801FBEF4:
    clrlslwi r0, r4, 24, 1
    add r3, r3, r0
    lhz r3, 0x2(r3)
    blr
