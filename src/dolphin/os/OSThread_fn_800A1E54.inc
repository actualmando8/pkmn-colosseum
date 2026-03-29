    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    addi r31, r3, 0x0
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    addi r29, r4, 0x0
    bl OSDisableInterrupts
    lhz r0, 0x2ca(r31)
    addi r30, r3, 0x0
    clrlwi. r0, r0, 31
    bne @L_800A1F00
    lhz r0, 0x2c8(r31)
    cmplwi r0, 0x8
    beq @L_800A1F00
    lwz r0, 0x2e8(r31)
    cmplwi r0, 0x0
    bne @L_800A1F00
    addi r3, r31, 0x2e8
    bl fn_800A238C
    lhz r0, 0x2c8(r31)
    cmplwi r0, 0x0
    bne @L_800A1EBC
    li r0, 0x0
    b @L_800A1EE8
    @L_800A1EBC:
    lis r3, 0x8000
    lwz r3, 0xdc(r3)
    b @L_800A1EDC
    @L_800A1EC8:
    cmplw r31, r3
    bne @L_800A1ED8
    li r0, 0x1
    b @L_800A1EE8
    @L_800A1ED8:
    lwz r3, 0x2fc(r3)
    @L_800A1EDC:
    cmplwi r3, 0x0
    bne @L_800A1EC8
    li r0, 0x0
    @L_800A1EE8:
    cmpwi r0, 0x0
    bne @L_800A1F00
    mr r3, r30
    bl OSRestoreInterrupts
    li r3, 0x0
    b @L_800A1F78
    @L_800A1F00:
    lhz r0, 0x2c8(r31)
    cmplwi r0, 0x8
    bne @L_800A1F6C
    cmplwi r29, 0x0
    beq @L_800A1F1C
    lwz r0, 0x2d8(r31)
    stw r0, 0x0(r29)
    @L_800A1F1C:
    lwz r4, 0x2fc(r31)
    lwz r5, 0x300(r31)
    cmplwi r4, 0x0
    bne @L_800A1F38
    lis r3, 0x8000
    stw r5, 0xe0(r3)
    b @L_800A1F3C
    @L_800A1F38:
    stw r5, 0x300(r4)
    @L_800A1F3C:
    cmplwi r5, 0x0
    bne @L_800A1F50
    lis r3, 0x8000
    stw r4, 0xdc(r3)
    b @L_800A1F54
    @L_800A1F50:
    stw r4, 0x2fc(r5)
    @L_800A1F54:
    li r0, 0x0
    sth r0, 0x2c8(r31)
    mr r3, r30
    bl OSRestoreInterrupts
    li r3, 0x1
    b @L_800A1F78
    @L_800A1F6C:
    mr r3, r30
    bl OSRestoreInterrupts
    li r3, 0x0
    @L_800A1F78:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
