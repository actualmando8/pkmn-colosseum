    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    lwz r30, 0xfc(r3)
    lwz r29, 0x4(r30)
    lwz r3, 0x0(r29)
    subis r0, r3, 0x4c5a
    cmplwi r0, 0x5353
    bne @L_8017C04C
    mr r3, r31
    mr r4, r30
    li r5, 0x0
    bl fn_80180C78
    @L_8017C04C:
    li r0, 0x64
    li r3, 0x0
    stw r0, 0x48(r31)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
