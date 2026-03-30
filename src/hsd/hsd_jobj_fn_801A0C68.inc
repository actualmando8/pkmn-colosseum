    nofralloc
    lhz r0, 0x6(r3)
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A0C80
    mr r3, r0
    blr
    @L_801A0C80:
    lhz r4, 0x6(r3)
    subi r0, r4, 0x1
    sth r0, 0x6(r3)
    lhz r0, 0x6(r3)
    cntlzw r0, r0
    srwi r3, r0, 5
    blr
