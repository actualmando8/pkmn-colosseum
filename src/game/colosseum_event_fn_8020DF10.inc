    nofralloc
    cmplwi r3, 0x0
    bne @L_8020DF20
    li r4, 0x0
    b @L_8020DF40
@L_8020DF20:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8020DF34
    li r4, 0x0
    b @L_8020DF40
@L_8020DF34:
    clrlslwi r4, r4, 24, 3
    addi r4, r4, 0x18
    add r4, r3, r4
@L_8020DF40:
    cmplwi r4, 0x0
    beqlr
    stw r5, 0x4(r4)
    blr
