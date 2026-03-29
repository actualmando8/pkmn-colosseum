    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stmw r26, 0x18(r1)
    addi r26, r3, 0x0
    cmplwi r26, 0x4
    bge @L_8009F538
    add r3, r4, r5
    addi r0, r3, 0x3ff
    clrrwi r27, r4, 10
    clrrwi r29, r0, 10
    addi r3, r27, 0x0
    clrlwi r31, r6, 30
    subf r4, r27, r29
    bl DCFlushRange
    bl OSDisableInterrupts
    lis r0, 0x8000
    srw r30, r0, r26
    addi r28, r3, 0x0
    addi r3, r30, 0x0
    bl __OSMaskInterrupts
    lis r3, 0xcc00
    addi r5, r3, 0x4000
    slwi r3, r26, 2
    extrwi r0, r27, 16, 6
    sthx r0, r5, r3
    extrwi r4, r29, 16, 6
    add r3, r5, r3
    sth r4, 0x2(r3)
    addi r4, r5, 0x10
    slwi r6, r26, 1
    li r0, 0x3
    lhz r5, 0x10(r5)
    slw r3, r0, r6
    slw r0, r31, r6
    andc r5, r5, r3
    or r5, r5, r0
    cmplwi r31, 0x3
    sth r5, 0x0(r4)
    beq @L_8009F530
    mr r3, r30
    bl __OSUnmaskInterrupts
    @L_8009F530:
    mr r3, r28
    bl OSRestoreInterrupts
    @L_8009F538:
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
