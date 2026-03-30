    nofralloc
    lwz r0, lbl_80478B38(r13)
    rlwinm r4, r3, 0, 16, 31
    cmplw r4, r0
    blt @L_8010C484
    li r3, 0x0
    blr
@L_8010C484:
    rlwinm r0, r3, 0, 16, 31
    lis r4, lbl_8035B500@ha
    mulli r5, r0, 0x2c
    addi r0, r4, lbl_8035B500@l
    add r4, r0, r5
    lhz r3, 0x2(r4)
    blr

