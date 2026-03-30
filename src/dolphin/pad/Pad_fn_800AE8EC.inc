    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x8(r1)
    bl OSDisableInterrupts
    lis r4, 0xcc00
    addi r4, r4, 0x5000
    lhz r5, 0xa(r4)
    li r0, -0xa9
    and r0, r5, r0
    ori r0, r0, 0x4
    sth r0, 0xa(r4)
    bl OSRestoreInterrupts
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
