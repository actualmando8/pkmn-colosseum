    nofralloc
    lwz r3, 0x8(r3)
    lwz r0, 0x14(r3)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    bne @L_800EE800
    li r0, 0x0
    b @L_800EE818
    @L_800EE800:
    lwz r3, 0x18(r3)
    cmplwi r3, 0x0
    bne @L_800EE814
    li r0, 0x0
    b @L_800EE818
    @L_800EE814:
    lwz r0, 0xc(r3)
    @L_800EE818:
    neg r0, r0
    cntlzw r0, r0
    extrwi r3, r0, 8, 19
    blr
