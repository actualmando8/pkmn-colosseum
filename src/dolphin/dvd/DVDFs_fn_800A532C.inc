    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x38(r1)
    stmw r26, 0x20(r1)
    mr. r29, r6
    addi r26, r3, 0x0
    addi r27, r4, 0x0
    addi r28, r5, 0x0
    addi r30, r7, 0x0
    addi r31, r8, 0x0
    blt @L_800A5364
    lwz r0, 0x34(r26)
    cmplw r29, r0
    blt @L_800A537C
@L_800A5364:
    lis r3, -32719
    crxor 6, 6, 6
    addi r5, r3, 6640
    addi r3, r13, -32352
    li r4, 0x2e6
    bl fn_800060F0
@L_800A537C:
    add. r4, r29, r28
    blt @L_800A5394
    lwz r3, 0x34(r26)
    addi r0, r3, 0x20
    cmplw r4, r0
    blt @L_800A53AC
@L_800A5394:
    lis r3, -32719
    crxor 6, 6, 6
    addi r5, r3, 6640
    addi r3, r13, -32352
    li r4, 0x2ec
    bl fn_800060F0
@L_800A53AC:
    stw r30, 0x38(r26)
    lis r3, -32758
    addi r7, r3, 21484
    lwz r0, 0x30(r26)
    addi r3, r26, 0x0
    addi r4, r27, 0x0
    addi r5, r28, 0x0
    addi r8, r31, 0x0
    add r6, r0, r29
    bl fn_800A720C
    lmw r26, 0x20(r1)
    li r3, 0x1
    lwz r0, 0x3c(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr
