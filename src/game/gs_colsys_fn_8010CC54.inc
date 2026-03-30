    nofralloc
    lis r3, -32704
    addi r3, r3, 19560
    lwz r0, 0x3704(r3)
    cmpwi r0, 0x0
    blt @L_8010CC70
    cmpwi r0, 0x4
    blt @L_8010CC78
@L_8010CC70:
    li r3, 0x0
    b @L_8010CC84
@L_8010CC78:
    mulli r0, r0, 0xdc0
    add r3, r3, r0
    addi r3, r3, 0x4
@L_8010CC84:
    cmplwi r3, 0x0
    beq @L_8010CD5C
    li r0, 0x3
    mtctr r0
@L_8010CC94:
    lhz r0, 0xa10(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa10(r3)
    lhz r0, 0xa24(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa24(r3)
    lhz r0, 0xa38(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa38(r3)
    lhz r0, 0xa4c(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa4c(r3)
    lhz r0, 0xa60(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa60(r3)
    lhz r0, 0xa74(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa74(r3)
    lhz r0, 0xa88(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa88(r3)
    lhz r0, 0xa9c(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xa9c(r3)
    lhz r0, 0xab0(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xab0(r3)
    lhz r0, 0xac4(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xac4(r3)
    lhz r0, 0xad8(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xad8(r3)
    lhz r0, 0xaec(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xaec(r3)
    lhz r0, 0xb00(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xb00(r3)
    lhz r0, 0xb14(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xb14(r3)
    lhz r0, 0xb28(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xb28(r3)
    lhz r0, 0xb3c(r3)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0xb3c(r3)
    addi r3, r3, 0x140
    bdnz @L_8010CC94
@L_8010CD5C:
    lis r3, -32704
    li r0, 0x0
    stw r0, 19560(r3)
    blr
