    nofralloc
    lwz r0, -31480(r13)
    rlwinm r3, r3, 0, 16, 31
    cmplw r3, r0
    blt @L_8020A878
    li r3, 0x0
    b @L_8020A888
@L_8020A878:
    mulli r4, r3, 0x18
    lis r3, -32713
    addi r0, r3, 23048
    add r3, r0, r4
@L_8020A888:
    cmplwi r3, 0x0
    bne @L_8020A898
    li r3, 0x0
    blr
@L_8020A898:
    lhz r3, 0x2(r3)
    blr
