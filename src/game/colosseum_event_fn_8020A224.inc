    nofralloc
    mr. r5, r3
    bne @L_8020A234
    li r3, 0x0
    blr
@L_8020A234:
    rlwinm r0, r4, 0, 16, 31
    cmplwi r0, 0x9
    blt @L_8020A248
    li r3, 0x0
    blr
@L_8020A248:
    clrlslwi r3, r4, 16, 4
    addi r3, r3, 0x8
    add r3, r5, r3
    blr
