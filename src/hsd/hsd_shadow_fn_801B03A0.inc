    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801B03CC
    lis r3, 0x8027
    li r4, 0x37d
    addi r3, r3, 21184
    addi r5, r2, -22760
    bl fn_80196E10
@L_801B03CC:
    lfs f1, 0x40(r31)
    li r3, 0x0
    lfs f0, 0x44(r31)
    fcmpo cr0, f1, f0
    ble @L_801B03F4
    lfs f1, 0x4c(r31)
    lfs f0, 0x48(r31)
    fcmpo cr0, f1, f0
    ble @L_801B03F4
    li r3, 0x1
@L_801B03F4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
