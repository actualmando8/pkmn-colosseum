    nofralloc
    lis r3, -32704
    addi r5, r3, 19560
    lwz r3, 0x3704(r5)
    addi r6, r3, 0x1
    cmpwi r6, 0x4
    blt @L_8010D084
    li r3, 0x0
    blr
@L_8010D084:
    mulli r3, r6, 0xdc0
    li r4, 0x0
    li r0, 0x3
    stw r4, 0x0(r5)
    add r3, r5, r3
    mtctr r0
@L_8010D09C:
    lhz r0, 0xa14(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa14(r3)
    lhz r0, 0xa28(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa28(r3)
    lhz r0, 0xa3c(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa3c(r3)
    lhz r0, 0xa50(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa50(r3)
    lhz r0, 0xa64(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa64(r3)
    lhz r0, 0xa78(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa78(r3)
    lhz r0, 0xa8c(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa8c(r3)
    lhz r0, 0xaa0(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xaa0(r3)
    lhz r0, 0xab4(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xab4(r3)
    lhz r0, 0xac8(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xac8(r3)
    lhz r0, 0xadc(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xadc(r3)
    lhz r0, 0xaf0(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xaf0(r3)
    lhz r0, 0xb04(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xb04(r3)
    lhz r0, 0xb18(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xb18(r3)
    lhz r0, 0xb2c(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xb2c(r3)
    lhz r0, 0xb40(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xb40(r3)
    addi r3, r3, 0x140
    bdnz @L_8010D09C
    stw r6, 0x3704(r5)
    li r3, 0x1
    blr
