    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801B16EC
    lis r3, 0x8027
    li r4, 0x10c
    addi r3, r3, 21184
    addi r5, r2, -22740
    bl fn_80196E10
@L_801B16EC:
    lwz r5, 0x8(r31)
    li r3, 0x0
    li r4, 0x0
    lwz r31, 0x58(r5)
    lhz r5, 0x4(r31)
    lhz r6, 0x6(r31)
    bl fn_800B962C
    lhz r3, 0x4(r31)
    li r5, 0x20
    lhz r4, 0x6(r31)
    li r6, 0x0
    bl fn_800B96F8
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
