    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    mr r30, r3
    bl OSDisableInterrupts
    lis r4, 0xcc00
    subfic r0, r30, 0x3
    lwz r31, 0x6438(r4)
    rlwinm r0, r0, 3, 0, 28
    srw r31, r31, r0
    rlwinm. r0, r31, 0, 28, 28
    beq @L_800D0318
    lis r4, -32719
    rlwinm r5, r30, 2, 0, 29
    addi r0, r4, 16288
    add r4, r0, r5
    lwz r0, 0x0(r4)
    rlwinm. r0, r0, 0, 24, 24
    bne @L_800D0318
    li r0, 0x8
    stw r0, 0x0(r4)
@L_800D0318:
    bl OSRestoreInterrupts
    mr r3, r31
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
