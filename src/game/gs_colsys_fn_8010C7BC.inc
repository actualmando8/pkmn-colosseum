    nofralloc
    lis r5, -32704
    addi r6, r5, 19560
    lwz r5, 0x0(r6)
    cmplwi r5, 0x0
    bne @L_8010C7D8
    li r3, 0x1
    b @L_8010C814
@L_8010C7D8:
    cmpwi r3, 0x0
    blt @L_8010C7EC
    lwz r0, 0x4(r5)
    cmplw r3, r0
    blt @L_8010C7F4
@L_8010C7EC:
    li r3, 0x2
    b @L_8010C814
@L_8010C7F4:
    lwz r5, 0x3704(r6)
    mulli r0, r3, 0x28
    li r3, 0x0
    mulli r5, r5, 0xdc0
    add r5, r6, r5
    addi r5, r5, 0x4
    add r5, r5, r0
    mr r7, r5
@L_8010C814:
    cmpwi r3, 0x0
    bnelr
    lhz r0, 0x24(r7)
    rlwinm. r0, r0, 0, 31, 31
    beq @L_8010C834
    li r0, 0x0
    stw r0, 0x0(r4)
    b @L_8010C83C
@L_8010C834:
    li r0, 0x1
    stw r0, 0x0(r4)
@L_8010C83C:
    li r3, 0x0
    blr
