    nofralloc
    lwz r0, -31896(r13)
    cmplw r3, r0
    ble @L_80136064
    li r3, 0x0
    blr
@L_80136064:
    mulli r0, r3, 0xa
    lis r3, -32714
    addi r3, r3, 15128
    lbzx r3, r3, r0
    blr
