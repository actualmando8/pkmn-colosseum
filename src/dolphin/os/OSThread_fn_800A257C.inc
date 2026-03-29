    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    mr. r31, r4
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    addi r29, r3, 0x0
    blt @L_800A25A8
    cmpwi r31, 0x1f
    ble @L_800A25B0
    @L_800A25A8:
    li r3, 0x0
    b @L_800A2620
    @L_800A25B0:
    bl OSDisableInterrupts
    lwz r0, 0x2d4(r29)
    addi r30, r3, 0x0
    cmpw r0, r31
    beq @L_800A2614
    stw r31, 0x2d4(r29)
    mr r31, r29
    @L_800A25CC:
    lwz r0, 0x2cc(r31)
    cmpwi r0, 0x0
    bgt @L_800A2600
    mr r3, r31
    bl fn_800A14EC
    lwz r0, 0x2d0(r31)
    addi r4, r3, 0x0
    cmpw r0, r4
    beq @L_800A2600
    mr r3, r31
    bl fn_800A1528
    mr. r31, r3
    bne @L_800A25CC
    @L_800A2600:
    lwz r0, RunQueueHint_8047A764(r13)
    cmpwi r0, 0x0
    beq @L_800A2614
    li r3, 0x0
    bl SelectThread
    @L_800A2614:
    mr r3, r30
    bl OSRestoreInterrupts
    li r3, 0x1
    @L_800A2620:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
