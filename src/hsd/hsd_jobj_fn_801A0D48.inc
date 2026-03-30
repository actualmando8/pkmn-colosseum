    nofralloc
    lis r4, 0x1
    lhz r5, 0x4(r3)
    subi r4, r4, 0x1
    clrlwi r0, r4, 16
    clrlwi r4, r5, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r4, r0, 5
    beq @L_801A0D74
    mr r3, r4
    blr
    @L_801A0D74:
    lhz r0, 0x4(r3)
    lhz r4, 0x4(r3)
    cntlzw r5, r0
    subi r0, r4, 0x1
    srwi r4, r5, 5
    sth r0, 0x4(r3)
    mr r3, r4
    blr
