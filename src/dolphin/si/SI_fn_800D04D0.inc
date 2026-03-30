    nofralloc
    mflr r0
    lis r4, -32704
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    addi r31, r4, -80
    stw r30, 0x20(r1)
    addi r30, r3, 0x0
    stw r29, 0x1c(r1)
    bl OSDisableInterrupts
    lis r4, 0xcc00
    subfic r0, r30, 0x3
    lwz r29, 0x6438(r4)
    rlwinm r0, r0, 3, 0, 28
    srw r29, r29, r0
    rlwinm. r0, r29, 0, 28, 28
    beq @L_800D0538
    lis r4, -32719
    rlwinm r5, r30, 2, 0, 29
    addi r0, r4, 16288
    add r4, r0, r5
    lwz r0, 0x0(r4)
    rlwinm. r0, r0, 0, 24, 24
    bne @L_800D0538
    li r0, 0x8
    stw r0, 0x0(r4)
@L_800D0538:
    bl OSRestoreInterrupts
    rlwinm. r0, r29, 0, 26, 26
    beq @L_800D0584
    mulli r4, r30, 0xc
    lis r3, 0xcc00
    addi r0, r3, 0x6400
    add r3, r0, r4
    rlwinm r4, r30, 3, 0, 28
    lwz r0, 0x4(r3)
    add r6, r31, r4
    stw r0, 0x1c0(r6)
    rlwinm r0, r30, 2, 0, 29
    add r4, r31, r0
    lwz r5, 0x8(r3)
    li r0, 0x1
    li r3, 0x1
    stw r5, 0x1c4(r6)
    stw r0, 0x1b0(r4)
    b @L_800D0588
@L_800D0584:
    li r3, 0x0
@L_800D0588:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
