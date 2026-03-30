    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    addi r31, r4, 0x0
    stw r30, 0x10(r1)
    addi r30, r3, 0x0
    bl OSDisableInterrupts
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    beq @L_800B0328
    stw r31, 0x4(r30)
    b @L_800B0338
    @L_800B0328:
    lwz r0, 0x4(r30)
    cmpwi r0, -0x1
    bne @L_800B0338
    stw r31, 0x4(r30)
    @L_800B0338:
    bl OSRestoreInterrupts
    mr r3, r31
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
