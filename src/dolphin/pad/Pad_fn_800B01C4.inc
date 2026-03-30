    nofralloc
    mflr r0
    mulli r5, r3, 0x110
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    lis r3, lbl_803FC620@ha
    addi r0, r3, lbl_803FC620@l
    stw r31, 0x14(r1)
    add r31, r0, r5
    stw r30, 0x10(r1)
    addi r30, r4, 0x0
    bl OSDisableInterrupts
    lwz r0, 0x4(r31)
    cmpwi r0, -0x1
    bne @L_800B0204
    li r3, -0x1
    b @L_800B0224
    @L_800B0204:
    cmplwi r30, 0x0
    beq @L_800B0214
    mr r0, r30
    b @L_800B0218
    @L_800B0214:
    lis r0, 0x8000
    @L_800B0218:
    stw r0, 0x10c(r31)
    bl OSRestoreInterrupts
    li r3, 0x0
    @L_800B0224:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
