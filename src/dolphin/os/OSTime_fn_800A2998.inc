    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x38(r1)
    stmw r25, 0x1c(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    lis r27, 0x8000
    lwz r0, 0xf8(r27)
    mr r3, r29
    mr r4, r30
    rlwinm r6, r0, 30, 2, 31
    li r5, 0x0
    bl fn_800C4B44
    mr r26, r3
    li r5, 0x0
    mr r25, r4
    xoris r4, r26, 0x8000
    xoris r3, r5, 0x8000
    subfc r0, r5, r25
    subfe r3, r3, r4
    subfe r3, r4, r4
    neg r3, r3
    cmpwi r3, 0x0
    beq @L_800A2A0C
    lwz r0, 0xf8(r27)
    rlwinm r0, r0, 30, 2, 31
    addc r25, r25, r0
    adde r26, r26, r5
@L_800A2A0C:
    li r4, 0x8
    mullw r3, r26, r4
    mulhwu r0, r25, r4
    lis r27, 0x8000
    lwz r6, 0xf8(r27)
    lis r5, 0x431c
    subi r5, r5, 0x217d
    rlwinm r6, r6, 30, 2, 31
    mulhwu r5, r5, r6
    rlwinm r6, r5, 17, 15, 31
    li r28, 0x0
    add r3, r3, r0
    mullw r0, r25, r28
    mullw r4, r25, r4
    add r3, r3, r0
    li r5, 0x0
    bl fn_800C4928
    li r5, 0x0
    li r6, 0x3e8
    bl fn_800C4B44
    stw r4, 0x24(r31)
    lis r3, 0x1062
    addi r5, r3, 0x4dd3
    lwz r0, 0xf8(r27)
    mr r3, r26
    mr r4, r25
    rlwinm r0, r0, 30, 2, 31
    mulhwu r0, r5, r0
    rlwinm r6, r0, 26, 6, 31
    li r5, 0x0
    bl fn_800C4928
    li r5, 0x0
    li r6, 0x3e8
    bl fn_800C4B44
    stw r4, 0x20(r31)
    subfc r30, r25, r30
    subfe r29, r26, r29
    lwz r0, 0xf8(r27)
    lis r5, 0x1
    addi r25, r5, 0x5180
    mr r3, r29
    rlwinm r6, r0, 30, 2, 31
    mr r4, r30
    li r5, 0x0
    bl fn_800C4928
    mr r6, r25
    li r5, 0x0
    bl fn_800C4928
    lis r5, 0xb
    lwz r0, 0xf8(r27)
    addi r5, r5, 0x2575
    addc r26, r4, r5
    rlwinm r6, r0, 30, 2, 31
    adde r0, r3, r28
    mr r3, r29
    mr r4, r30
    li r5, 0x0
    bl fn_800C4928
    mr r6, r25
    li r5, 0x0
    bl fn_800C4B44
    mr r27, r4
    cmpwi r27, 0x0
    bge @L_800A2B18
    addis r27, r27, 0x1
    subi r26, r26, 0x1
    addi r27, r27, 0x5180
@L_800A2B18:
    mr r3, r26
    mr r4, r31
    bl fn_800A27FC
    lis r3, 0x8889
    subi r5, r3, 0x7777
    mulhw r0, r5, r27
    add r4, r0, r27
    srawi r0, r4, 5
    rlwinm r3, r0, 1, 31, 31
    add r7, r0, r3
    mulhw r0, r5, r7
    add r0, r0, r7
    srawi r5, r0, 5
    srawi r0, r0, 5
    rlwinm r3, r0, 1, 31, 31
    add r3, r0, r3
    srawi r0, r4, 5
    rlwinm r6, r5, 1, 31, 31
    mulli r4, r3, 0x3c
    rlwinm r3, r0, 1, 31, 31
    add r5, r5, r6
    add r0, r0, r3
    stw r5, 0x8(r31)
    mulli r0, r0, 0x3c
    subf r3, r4, r7
    stw r3, 0x4(r31)
    subf r0, r0, r27
    stw r0, 0x0(r31)
    lmw r25, 0x1c(r1)
    lwz r0, 0x3c(r1)
    addi r1, r1, 0x38
    mtlr r0
    blr
