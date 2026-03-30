    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r28, r3
    bl OSDisableInterrupts
    lis r31, 0x8000
    lwz r30, 0xe4(r31)
    addi r29, r3, 0x0
    addi r3, r30, 0x0
    bl OSClearContext
    lhz r0, 0x2ca(r30)
    clrlwi. r0, r0, 31
    beq @L_800A1C38
    lwz r4, 0x2fc(r30)
    lwz r5, 0x300(r30)
    cmplwi r4, 0x0
    bne @L_800A1C10
    stw r5, 0xe0(r31)
    b @L_800A1C14
    @L_800A1C10:
    stw r5, 0x300(r4)
    @L_800A1C14:
    cmplwi r5, 0x0
    bne @L_800A1C28
    lis r3, 0x8000
    stw r4, 0xdc(r3)
    b @L_800A1C2C
    @L_800A1C28:
    stw r4, 0x2fc(r5)
    @L_800A1C2C:
    li r0, 0x0
    sth r0, 0x2c8(r30)
    b @L_800A1C44
    @L_800A1C38:
    li r0, 0x8
    sth r0, 0x2c8(r30)
    stw r28, 0x2d8(r30)
    @L_800A1C44:
    mr r3, r30
    bl fn_8009F958
    addi r3, r30, 0x2e8
    bl fn_800A2478
    li r0, 0x1
    stw r0, RunQueueHint_8047A764(r13)
    lwz r0, RunQueueHint_8047A764(r13)
    cmpwi r0, 0x0
    beq @L_800A1C70
    li r3, 0x0
    bl SelectThread
    @L_800A1C70:
    mr r3, r29
    bl OSRestoreInterrupts
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
