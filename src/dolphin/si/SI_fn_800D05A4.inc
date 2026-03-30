    nofralloc
    mflr r0
    lis r5, -32704
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stmw r27, 0x14(r1)
    addi r29, r3, 0x0
    addi r30, r4, 0x0
    addi r28, r5, -80
    bl OSDisableInterrupts
    addi r31, r3, 0x0
    addi r3, r29, 0x0
    bl fn_800D02BC
    rlwinm. r0, r3, 0, 26, 26
    beq @L_800D0614
    mulli r4, r29, 0xc
    lis r3, 0xcc00
    addi r0, r3, 0x6400
    add r4, r0, r4
    rlwinm r3, r29, 3, 0, 28
    lwz r0, 0x4(r4)
    add r5, r28, r3
    stw r0, 0x1c0(r5)
    rlwinm r0, r29, 2, 0, 29
    add r3, r28, r0
    lwz r4, 0x8(r4)
    li r0, 0x1
    stw r4, 0x1c4(r5)
    stw r0, 0x1b0(r3)
@L_800D0614:
    rlwinm r0, r29, 2, 0, 29
    add r3, r28, r0
    lwz r27, 0x1b0(r3)
    li r0, 0x0
    cmpwi r27, 0x0
    stw r0, 0x1b0(r3)
    beq @L_800D0648
    rlwinm r0, r29, 3, 0, 28
    add r3, r28, r0
    lwz r0, 0x1c0(r3)
    stw r0, 0x0(r30)
    lwz r0, 0x1c4(r3)
    stw r0, 0x4(r30)
@L_800D0648:
    mr r3, r31
    bl OSRestoreInterrupts
    mr r3, r27
    lmw r27, 0x14(r1)
    lwz r0, 0x2c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
