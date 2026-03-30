    nofralloc
    lwz r0, -31480(r13)
    rlwinm r3, r3, 0, 16, 31
    cmplw r3, r0
    blt @L_8020A8B8
    li r3, 0x0
    b @L_8020A8C8
@L_8020A8B8:
    mulli r4, r3, 0x18
    lis r3, -32713
    addi r0, r3, 23048
    add r3, r0, r4
@L_8020A8C8:
    cmplwi r3, 0x0
    bne @L_8020A8D8
    li r3, 0x0
    blr
@L_8020A8D8:
    lbz r3, 0x0(r3)
    blr
