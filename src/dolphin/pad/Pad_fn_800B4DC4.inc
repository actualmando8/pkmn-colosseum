    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    stw r30, 0x10(r1)
    mr r30, r3
    bl OSDisableInterrupts
    lwz r0, 0x0(r30)
    lis r4, lbl_803FC620@ha
    li r31, 0x0
    mulli r5, r0, 0x110
    addi r0, r4, lbl_803FC620@l
    add r4, r0, r5
    lwz r0, 0x0(r4)
    cmpwi r0, 0x0
    bne @L_800B4E0C
    li r31, -0x3
    b @L_800B4E30
    @L_800B4E0C:
    lwz r0, 0x4(r4)
    cmpwi r0, -0x1
    bne @L_800B4E30
    lwz r0, 0xc0(r4)
    cmplw r0, r30
    bne @L_800B4E30
    li r0, -0x1
    stw r0, 0xc(r30)
    li r31, -0xe
    @L_800B4E30:
    bl OSRestoreInterrupts
    mr r3, r31
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
