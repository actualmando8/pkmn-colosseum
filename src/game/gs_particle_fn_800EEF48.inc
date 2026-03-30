    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r26, r3
    lwz r0, 0x8(r3)
    cmpwi r0, 0x44
    bne @L_800EF084
    lhz r3, 0x50(r26)
    addi r0, r3, 0x1
    sth r0, 0x50(r26)
    lwz r29, 0x28(r26)
    cmplwi r29, 0x0
    beq @L_800EF084
    lhz r31, 0x0(r26)
    lhz r0, 0x2(r26)
    mullw r0, r31, r0
    clrlslwi r30, r0, 16, 1
    mr r3, r30
    rlwinm r28, r0, 0, 16, 31
    bl fn_800E3534
    rlwinm. r0, r3, 0, 16, 31
    mr r27, r3
    beq @L_800EF084
    bl fn_800E27B0
    srawi r0, r31, 2
    mr r4, r3
    li r3, 0x0
    rlwinm r5, r0, 0, 16, 31
    b @L_800EF040
@L_800EEFC0:
    rlwinm r6, r3, 0, 16, 31
    rlwinm r7, r3, 28, 0, 3
    srawi r0, r6, 4
    rlwinm r8, r3, 30, 0, 1
    rlwinm r0, r0, 0, 16, 31
    rlwinm r9, r6, 1, 31, 31
    divw r10, r0, r5
    clrlslwi r6, r3, 16, 1
    subf r7, r9, r7
    subf r8, r9, r8
    rlwinm r7, r7, 4, 0, 31
    rlwinm r8, r8, 2, 0, 31
    add r7, r7, r9
    rlwinm r11, r10, 0, 16, 31
    mullw r10, r10, r5
    add r8, r8, r9
    srawi r7, r7, 2
    rlwinm r9, r8, 0, 16, 31
    addze r7, r7
    addi r3, r3, 0x1
    rlwinm r7, r7, 0, 16, 31
    subf r0, r10, r0
    mullw r7, r7, r31
    rlwinm r0, r0, 0, 16, 31
    mullw r8, r11, r31
    add r7, r7, r9
    add r0, r0, r8
    rlwinm r0, r0, 2, 0, 29
    add r0, r0, r7
    clrlslwi r0, r0, 16, 1
    lhzx r0, r29, r0
    sthx r0, r4, r6
@L_800EF040:
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r28
    blt @L_800EEFC0
    mr r3, r29
    mr r5, r30
    bl memcpy
    lwz r3, 0x28(r26)
    lwz r4, 0x4c(r26)
    bl DCFlushRange
    bl fn_800BB29C
    lhz r4, 0x50(r26)
    mr r3, r27
    subi r0, r4, 0x1
    sth r0, 0x50(r26)
    bl fn_800E24B0
    mr r3, r27
    bl fn_800E209C
@L_800EF084:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
