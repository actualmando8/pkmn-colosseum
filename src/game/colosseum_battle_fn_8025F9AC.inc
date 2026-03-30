    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stmw r27, 0x1c(r1)
    addi r27, r3, 0x0
    lis r3, -32696
    rlwinm r7, r27, 8, 0, 23
    addi r0, r3, -31776
    addi r28, r4, 0x0
    addi r29, r5, 0x0
    addi r31, r6, 0x0
    add r30, r0, r7
    bl OSDisableInterrupts
    stw r31, 0x38(r30)
    lis r4, -32730
    addi r31, r3, 0x0
    stw r28, 0xc(r30)
    addi r4, r4, -1912
    mr r3, r27
    stw r29, 0x10(r30)
    bl fn_800D0CBC
    mr r3, r31
    bl OSRestoreInterrupts
    lmw r27, 0x1c(r1)
    li r3, 0x0
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
