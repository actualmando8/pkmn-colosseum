    nofralloc
    stwu r1, -0x10(r1)
    rlwinm r0, r3, 0, 16, 31
    lhz r3, lbl_8047CDE0(r13)
    cmplwi r0, 0xc
    sth r3, 0xc(r1)
    blt @L_80103ECC
    rlwinm r3, r3, 16, 0, 15
    b @L_80103EEC
@L_80103ECC:
    lis r5, lbl_80404A98@ha
    rlwinm r6, r0, 1, 0, 30
    addi r5, r5, lbl_80404A98@l
    lhz r0, 0x0(r4)
    lhzx r3, r5, r6
    sth r3, 0x8(r1)
    rlwinm r3, r3, 16, 0, 15
    sthx r0, r5, r6
@L_80103EEC:
    addi r1, r1, 0x10
    blr
