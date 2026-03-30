    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    lwz r3, -24000(r13)
    bl SISetSamplingRate
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
