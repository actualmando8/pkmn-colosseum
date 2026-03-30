    nofralloc
    lis r5, -32704
    addi r6, r5, 19560
    lwz r5, 0x0(r6)
    cmplwi r5, 0x0
    bne @L_8010C860
    li r3, 0x1
    b @L_8010C89C
@L_8010C860:
    cmpwi r3, 0x0
    blt @L_8010C874
    lwz r0, 0x4(r5)
    cmplw r3, r0
    blt @L_8010C87C
@L_8010C874:
    li r3, 0x2
    b @L_8010C89C
@L_8010C87C:
    lwz r5, 0x3704(r6)
    mulli r0, r3, 0x28
    li r3, 0x0
    mulli r5, r5, 0xdc0
    add r5, r6, r5
    addi r5, r5, 0x4
    add r5, r5, r0
    mr r7, r5
@L_8010C89C:
    cmpwi r3, 0x0
    bnelr
    cmpwi r4, 0x0
    beq @L_8010C8BC
    lhz r0, 0x24(r7)
    rlwinm r0, r0, 0, 16, 30
    sth r0, 0x24(r7)
    b @L_8010C8C8
@L_8010C8BC:
    lhz r0, 0x24(r7)
    ori r0, r0, 0x1
    sth r0, 0x24(r7)
@L_8010C8C8:
    li r3, 0x0
    blr
