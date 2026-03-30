    nofralloc
    mflr r0
    lis r3, 0x2
    stw r0, 0x4(r1)
    addi r3, r3, -0x8000
    stwu r1, -0x8(r1)
    bl __OSMaskInterrupts
    li r3, 0x40
    bl __OSMaskInterrupts
    lis r3, 0x800D
    addi r0, r3, -0x4E90
    lis r3, 0x800D
    stw r0, -24052(r13)
    addi r4, r3, -0x4ED0
    li r3, 0x19
    bl __OSSetInterruptHandler
    li r3, 0x40
    bl __OSUnmaskInterrupts
    lwz r0, 0xc(r1)
    addi r1, r1, 0x8
    mtlr r0
    blr
