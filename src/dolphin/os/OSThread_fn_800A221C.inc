    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    bl OSDisableInterrupts
    lwz r4, 0x2cc(r29)
    addi r31, r3, 0x0
    addi r0, r4, 0x1
    mr. r30, r4
    stw r0, 0x2cc(r29)
    bne @L_800A2364
    lhz r0, 0x2c8(r29)
    cmpwi r0, 0x3
    beq @L_800A2350
    bge @L_800A2274
    cmpwi r0, 0x1
    beq @L_800A2290
    bge @L_800A2280
    b @L_800A2350
    @L_800A2274:
    cmpwi r0, 0x5
    bge @L_800A2350
    b @L_800A229C
    @L_800A2280:
    li r0, 0x1
    stw r0, RunQueueHint_8047A764(r13)
    sth r0, 0x2c8(r29)
    b @L_800A2350
    @L_800A2290:
    mr r3, r29
    bl fn_800A1484
    b @L_800A2350
    @L_800A229C:
    lwz r4, 0x2e0(r29)
    lwz r5, 0x2e4(r29)
    cmplwi r4, 0x0
    bne @L_800A22B8
    lwz r3, 0x2dc(r29)
    stw r5, 0x4(r3)
    b @L_800A22BC
    @L_800A22B8:
    stw r5, 0x2e4(r4)
    @L_800A22BC:
    cmplwi r5, 0x0
    bne @L_800A22D0
    lwz r3, 0x2dc(r29)
    stw r4, 0x0(r3)
    b @L_800A22D4
    @L_800A22D0:
    stw r4, 0x2e0(r5)
    @L_800A22D4:
    li r0, 0x20
    stw r0, 0x2d0(r29)
    lwz r4, 0x2dc(r29)
    lwz r3, 0x4(r4)
    cmplwi r3, 0x0
    bne @L_800A22F4
    stw r29, 0x0(r4)
    b @L_800A22F8
    @L_800A22F4:
    stw r29, 0x2e0(r3)
    @L_800A22F8:
    stw r3, 0x2e4(r29)
    li r0, 0x0
    stw r0, 0x2e0(r29)
    lwz r3, 0x2dc(r29)
    stw r29, 0x4(r3)
    lwz r3, 0x2f0(r29)
    cmplwi r3, 0x0
    beq @L_800A2350
    lwz r29, 0x8(r3)
    @L_800A231C:
    lwz r0, 0x2cc(r29)
    cmpwi r0, 0x0
    bgt @L_800A2350
    mr r3, r29
    bl fn_800A14EC
    lwz r0, 0x2d0(r29)
    addi r4, r3, 0x0
    cmpw r0, r4
    beq @L_800A2350
    mr r3, r29
    bl fn_800A1528
    mr. r29, r3
    bne @L_800A231C
    @L_800A2350:
    lwz r0, RunQueueHint_8047A764(r13)
    cmpwi r0, 0x0
    beq @L_800A2364
    li r3, 0x0
    bl SelectThread
    @L_800A2364:
    mr r3, r31
    bl OSRestoreInterrupts
    mr r3, r30
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
