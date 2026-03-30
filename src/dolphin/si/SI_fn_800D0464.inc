    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr. r31, r3
    bne @L_800D048C
    lis r3, -32719
    addi r3, r3, 16268
    lwz r3, 0x4(r3)
    b @L_800D04BC
@L_800D048C:
    bl OSDisableInterrupts
    lis r4, -32719
    addi r4, r4, 16268
    lwz r0, 0x4(r4)
    rlwinm r31, r31, 8, 24, 27
    addi r5, r4, 0x4
    lis r4, 0xcc00
    andc r31, r0, r31
    stw r31, 0x6430(r4)
    stw r31, 0x0(r5)
    bl OSRestoreInterrupts
    mr r3, r31
@L_800D04BC:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
