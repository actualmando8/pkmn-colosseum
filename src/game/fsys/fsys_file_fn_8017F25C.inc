    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r4, 0xc(r1)
    stw r30, 0x18(r1)
    lwz r31, 0xc(r1)
    stw r3, 0x8(r1)
    bl OSDisableInterrupts
    lwz r0, 0x8(r1)
    mr r30, r3
    cmpwi r0, 0x1
    bne @L_8017F29C
    li r0, 0xa2
    stw r0, 0x48(r31)
    b @L_8017F2A4
    @L_8017F29C:
    li r0, 0x98
    stw r0, 0x48(r31)
    @L_8017F2A4:
    mr r3, r30
    bl OSRestoreInterrupts
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
