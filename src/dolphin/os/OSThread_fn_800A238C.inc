    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    mr r30, r3
    bl OSDisableInterrupts
    lis r4, 0x8000
    lwz r4, 0xe4(r4)
    li r0, 0x4
    mr r31, r3
    sth r0, 0x2c8(r4)
    stw r30, 0x2dc(r4)
    lwz r5, 0x0(r30)
    b @L_800A23CC
    @L_800A23C8:
    lwz r5, 0x2e0(r5)
    @L_800A23CC:
    cmplwi r5, 0x0
    beq @L_800A23E4
    lwz r3, 0x2d0(r5)
    lwz r0, 0x2d0(r4)
    cmpw r3, r0
    ble @L_800A23C8
    @L_800A23E4:
    cmplwi r5, 0x0
    bne @L_800A2418
    lwz r3, 0x4(r30)
    cmplwi r3, 0x0
    bne @L_800A2400
    stw r4, 0x0(r30)
    b @L_800A2404
    @L_800A2400:
    stw r4, 0x2e0(r3)
    @L_800A2404:
    stw r3, 0x2e4(r4)
    li r0, 0x0
    stw r0, 0x2e0(r4)
    stw r4, 0x4(r30)
    b @L_800A243C
    @L_800A2418:
    stw r5, 0x2e0(r4)
    lwz r3, 0x2e4(r5)
    stw r4, 0x2e4(r5)
    cmplwi r3, 0x0
    stw r3, 0x2e4(r4)
    bne @L_800A2438
    stw r4, 0x0(r30)
    b @L_800A243C
    @L_800A2438:
    stw r4, 0x2e0(r3)
    @L_800A243C:
    li r0, 0x1
    stw r0, RunQueueHint_8047A764(r13)
    lwz r0, RunQueueHint_8047A764(r13)
    cmpwi r0, 0x0
    beq @L_800A2458
    li r3, 0x0
    bl SelectThread
    @L_800A2458:
    mr r3, r31
    bl OSRestoreInterrupts
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
