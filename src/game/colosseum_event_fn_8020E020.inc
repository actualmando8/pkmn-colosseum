    nofralloc
    cmplwi r3, 0x0
    bne @L_8020E030
    li r4, 0x0
    b @L_8020E050
@L_8020E030:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8020E044
    li r4, 0x0
    b @L_8020E050
@L_8020E044:
    clrlslwi r4, r4, 24, 3
    addi r4, r4, 0x18
    add r4, r3, r4
@L_8020E050:
    cmplwi r4, 0x0
    bne @L_8020E060
    li r3, 0x0
    blr
@L_8020E060:
    lwz r3, 0x4(r4)
    blr
