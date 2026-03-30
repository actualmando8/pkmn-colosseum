    nofralloc
    lwz r0, -31896(r13)
    cmplw r3, r0
    bgt @L_80135FD0
    cmplwi r4, 0x2
    ble @L_80135FD8
@L_80135FD0:
    li r3, 0x0
    blr
@L_80135FD8:
    mulli r3, r3, 0xa
    lis r5, -32714
    rlwinm r0, r4, 1, 0, 30
    addi r4, r5, 15128
    add r3, r4, r3
    add r3, r3, r0
    lha r3, 0x6(r3)
    blr
