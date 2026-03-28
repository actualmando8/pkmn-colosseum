    nofralloc
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x30
    ble @L_801EE454
    li r3, 0x0
    blr
@L_801EE454:
    lis r4, -32713
    clrlslwi r0, r3, 16, 1
    addi r3, r4, 21056
    lhzx r3, r3, r0
    blr
