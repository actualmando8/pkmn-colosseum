    nofralloc
    mflr r0
    lis r4, -32697
    stw r0, 0x4(r1)
    stwu r1, -0x10(r1)
    stw r31, 0xc(r1)
    addi r31, r4, -11008
    lwz r3, -31512(r13)
    bl OSRegisterVersion
    lis r4, 0xe000
    stw r4, 0x100(r31)
    addi r4, r4, 0x2000
    stw r4, 0x104(r31)
    addi r4, r4, 0x800
    stw r4, 0x108(r31)
    lis r4, 0xe000
    stw r4, 0x10c(r31)
    addi r4, r4, 0x2800
    stw r4, 0x110(r31)
    addi r4, r4, 0xa00
    stw r4, 0x114(r31)
    li r3, 0x4
    oris r3, r3, 0x4
    mtspr GQR2, r3
    li r3, 0x5
    oris r3, r3, 0x5
    mtspr GQR3, r3
    li r3, 0x6
    oris r3, r3, 0x6
    mtspr GQR4, r3
    li r3, 0x7
    oris r3, r3, 0x7
    mtspr GQR5, r3
    li r0, 0x1
    stw r0, -21100(r13)
    li r3, 0x1
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    addi r1, r1, 0x10
    mtlr r0
    blr
