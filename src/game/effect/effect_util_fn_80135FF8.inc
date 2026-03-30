    nofralloc
    lwz r0, -31896(r13)
    cmplw r3, r0
    ble @L_8013600C
    li r3, 0x0
    blr
@L_8013600C:
    mulli r0, r3, 0xa
    lis r3, -32714
    addi r3, r3, 15128
    add r3, r3, r0
    lbz r3, 0x1(r3)
    blr
