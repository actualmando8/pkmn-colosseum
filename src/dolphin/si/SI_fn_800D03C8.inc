    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr. r31, r3
    bne @L_800D03F0
    lis r3, -32719
    addi r3, r3, 16268
    lwz r3, 0x4(r3)
    b @L_800D0450
@L_800D03F0:
    bl OSDisableInterrupts
    lis r4, -32719
    addi r4, r4, 16268
    rlwinm r31, r31, 8, 24, 31
    lwz r0, 0x4(r4)
    addi r5, r4, 0x4
    rlwinm r4, r31, 28, 28, 31
    andc r0, r0, r4
    stw r0, 0x0(r5)
    oris r0, r4, 0x3ff
    ori r0, r0, 0xfff0
    and r31, r31, r0
    lwz r0, 0x0(r5)
    rlwinm r31, r31, 0, 24, 5
    or r0, r0, r31
    stw r0, 0x0(r5)
    lis r4, 0xcc00
    addi r4, r4, 0x6400
    lwz r31, 0x0(r5)
    lis r0, 0x8000
    stw r0, 0x38(r4)
    stw r31, 0x30(r4)
    bl OSRestoreInterrupts
    mr r3, r31
@L_800D0450:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
