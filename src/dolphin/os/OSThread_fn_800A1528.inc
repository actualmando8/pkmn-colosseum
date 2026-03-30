    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    mr r31, r3
    stw r30, 0x10(r1)
    addi r30, r4, 0x0
    lhz r0, 0x2c8(r3)
    cmpwi r0, 0x3
    beq @L_800A16CC
    bge @L_800A1564
    cmpwi r0, 0x1
    beq @L_800A1570
    bge @L_800A16C0
    b @L_800A16CC
    @L_800A1564:
    cmpwi r0, 0x5
    bge @L_800A16CC
    b @L_800A15E8
    @L_800A1570:
    mr r3, r31
    bl fn_800A1484
    stw r30, 0x2d0(r31)
    lis r3, RunQueue_803FB898@ha
    addi r0, r3, RunQueue_803FB898@l
    lwz r3, 0x2d0(r31)
    slwi r3, r3, 3
    add r0, r0, r3
    stw r0, 0x2dc(r31)
    lwz r4, 0x2dc(r31)
    lwz r3, 0x4(r4)
    cmplwi r3, 0x0
    bne @L_800A15AC
    stw r31, 0x0(r4)
    b @L_800A15B0
    @L_800A15AC:
    stw r31, 0x2e0(r3)
    @L_800A15B0:
    stw r3, 0x2e4(r31)
    li r0, 0x0
    li r3, 0x1
    stw r0, 0x2e0(r31)
    lwz r4, 0x2dc(r31)
    stw r31, 0x4(r4)
    lwz r0, 0x2d0(r31)
    lwz r4, RunQueueBits_8047A760(r13)
    subfic r0, r0, 0x1f
    slw r0, r3, r0
    or r0, r4, r0
    stw r0, RunQueueBits_8047A760(r13)
    stw r3, RunQueueHint_8047A764(r13)
    b @L_800A16CC
    @L_800A15E8:
    lwz r4, 0x2e0(r31)
    lwz r5, 0x2e4(r31)
    cmplwi r4, 0x0
    bne @L_800A1604
    lwz r3, 0x2dc(r31)
    stw r5, 0x4(r3)
    b @L_800A1608
    @L_800A1604:
    stw r5, 0x2e4(r4)
    @L_800A1608:
    cmplwi r5, 0x0
    bne @L_800A161C
    lwz r3, 0x2dc(r31)
    stw r4, 0x0(r3)
    b @L_800A1620
    @L_800A161C:
    stw r4, 0x2e0(r5)
    @L_800A1620:
    stw r30, 0x2d0(r31)
    lwz r4, 0x2dc(r31)
    lwz r5, 0x0(r4)
    b @L_800A1634
    @L_800A1630:
    lwz r5, 0x2e0(r5)
    @L_800A1634:
    cmplwi r5, 0x0
    beq @L_800A164C
    lwz r3, 0x2d0(r5)
    lwz r0, 0x2d0(r31)
    cmpw r3, r0
    ble @L_800A1630
    @L_800A164C:
    cmplwi r5, 0x0
    bne @L_800A1684
    lwz r3, 0x4(r4)
    cmplwi r3, 0x0
    bne @L_800A1668
    stw r31, 0x0(r4)
    b @L_800A166C
    @L_800A1668:
    stw r31, 0x2e0(r3)
    @L_800A166C:
    stw r3, 0x2e4(r31)
    li r0, 0x0
    stw r0, 0x2e0(r31)
    lwz r3, 0x2dc(r31)
    stw r31, 0x4(r3)
    b @L_800A16AC
    @L_800A1684:
    stw r5, 0x2e0(r31)
    lwz r3, 0x2e4(r5)
    stw r31, 0x2e4(r5)
    cmplwi r3, 0x0
    stw r3, 0x2e4(r31)
    bne @L_800A16A8
    lwz r3, 0x2dc(r31)
    stw r31, 0x0(r3)
    b @L_800A16AC
    @L_800A16A8:
    stw r31, 0x2e0(r3)
    @L_800A16AC:
    lwz r3, 0x2f0(r31)
    cmplwi r3, 0x0
    beq @L_800A16CC
    lwz r3, 0x8(r3)
    b @L_800A16D0
    @L_800A16C0:
    li r0, 0x1
    stw r0, RunQueueHint_8047A764(r13)
    stw r30, 0x2d0(r31)
    @L_800A16CC:
    li r3, 0x0
    @L_800A16D0:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
