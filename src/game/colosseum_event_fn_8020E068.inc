    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E078
    li r4, 0x0
    b @L_8020E098
@L_8020E078:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8020E08C
    li r4, 0x0
    b @L_8020E098
@L_8020E08C:
    clrlslwi r4, r4, 24, 3
    addi r4, r4, 0x18
    add r4, r3, r4
@L_8020E098:
    cmplwi r4, 0x0
    bne @L_8020E0A8
    li r3, 0x0
    blr
@L_8020E0A8:
    lhz r3, 0x0(r4)
    blr
