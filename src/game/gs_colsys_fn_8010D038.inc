    nofralloc
    lis r3, -32704
    addi r4, r3, 19560
    lwz r3, 0x3704(r4)
    cmpwi r3, 0x0
    bge @L_8010D054
    li r3, 0x0
    blr
@L_8010D054:
    subi r0, r3, 0x1
    li r3, 0x1
    stw r0, 0x3704(r4)
    blr
