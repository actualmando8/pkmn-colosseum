    nofralloc
    lis r3, -32704
    addi r3, r3, 19560
    lwz r0, 0x3704(r3)
    cmpwi r0, 0x0
    blt @L_8010CBEC
    cmpwi r0, 0x4
    blt @L_8010CBF4
@L_8010CBEC:
    li r3, 0x0
    blr
@L_8010CBF4:
    mulli r0, r0, 0xdc0
    add r3, r3, r0
    addi r3, r3, 0x4
    blr
