    nofralloc
    stwu r1, -0x10(r1)
    rlwinm r0, r3, 0, 16, 31
    lhz r3, lbl_8047CDE4(r13)
    cmplwi r0, 0xc
    sth r3, 0x8(r1)
    blt @L_80103E88
    sth r3, 0xc(r1)
    b @L_80103E9C
@L_80103E88:
    lis r3, lbl_80404A98@ha
    rlwinm r0, r0, 1, 0, 30
    addi r3, r3, lbl_80404A98@l
    lhzx r0, r3, r0
    sth r0, 0xc(r1)
@L_80103E9C:
    lhz r0, 0xc(r1)
    rlwinm r3, r0, 16, 0, 15
    addi r1, r1, 0x10
    blr
