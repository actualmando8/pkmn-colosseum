    nofralloc
    lwz r0, lbl_80478B38(r13)
    cmplw r3, r0
    blt @L_8010C51C
    li r3, 0x0
    blr
@L_8010C51C:
    cmplwi r4, 0x12
    blt @L_8010C52C
    li r3, 0x0
    blr
@L_8010C52C:
    mulli r6, r3, 0x2c
    lis r5, lbl_8035B500@ha
    rlwinm r0, r4, 1, 0, 30
    addi r5, r5, lbl_8035B500@l
    add r5, r5, r6
    add r5, r5, r0
    lhz r3, 0x8(r5)
    blr

