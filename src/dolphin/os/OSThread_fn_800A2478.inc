    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    mr r30, r3
    bl OSDisableInterrupts
    lis r4, RunQueue_803FB898@ha
    addi r31, r3, 0x0
    addi r5, r4, RunQueue_803FB898@l
    b @L_800A253C
    @L_800A24A4:
    lwz r3, 0x2e0(r6)
    cmplwi r3, 0x0
    bne @L_800A24BC
    li r0, 0x0
    stw r0, 0x4(r30)
    b @L_800A24C4
    @L_800A24BC:
    li r0, 0x0
    stw r0, 0x2e4(r3)
    @L_800A24C4:
    stw r3, 0x0(r30)
    li r0, 0x1
    sth r0, 0x2c8(r6)
    lwz r0, 0x2cc(r6)
    cmpwi r0, 0x0
    bgt @L_800A253C
    lwz r0, 0x2d0(r6)
    slwi r0, r0, 3
    add r0, r5, r0
    stw r0, 0x2dc(r6)
    lwz r4, 0x2dc(r6)
    lwz r3, 0x4(r4)
    cmplwi r3, 0x0
    bne @L_800A2504
    stw r6, 0x0(r4)
    b @L_800A2508
    @L_800A2504:
    stw r6, 0x2e0(r3)
    @L_800A2508:
    stw r3, 0x2e4(r6)
    li r0, 0x0
    li r3, 0x1
    stw r0, 0x2e0(r6)
    lwz r4, 0x2dc(r6)
    stw r6, 0x4(r4)
    lwz r0, 0x2d0(r6)
    lwz r4, RunQueueBits_8047A760(r13)
    subfic r0, r0, 0x1f
    slw r0, r3, r0
    or r0, r4, r0
    stw r0, RunQueueBits_8047A760(r13)
    stw r3, RunQueueHint_8047A764(r13)
    @L_800A253C:
    lwz r6, 0x0(r30)
    cmplwi r6, 0x0
    bne @L_800A24A4
    lwz r0, RunQueueHint_8047A764(r13)
    cmpwi r0, 0x0
    beq @L_800A255C
    li r3, 0x0
    bl SelectThread
    @L_800A255C:
    mr r3, r31
    bl OSRestoreInterrupts
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
