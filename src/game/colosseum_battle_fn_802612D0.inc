    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, 0x8048
    stw r0, 0x14(r1)
    addi r8, r3, -30752
    lwz r3, 0x0(r8)
    lwz r4, 0x4(r8)
    lwz r5, 0x8(r8)
    lwz r6, 0x10(r8)
    lwz r7, 0xc(r8)
    lwz r8, 0x14(r8)
    bl fn_80260EBC
    lis r4, 0x8048
    addi r4, r4, -30752
    stw r3, 0x18(r4)
    bl fn_800FF660
    li r3, 0x0
    li r4, 0x0
    bl fn_8011288C
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
