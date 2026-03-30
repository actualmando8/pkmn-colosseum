    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    addi r31, r5, 0x0
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    addi r29, r4, 0x0
    addi r4, r1, 0x18
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B03BC
    b @L_800B04BC
    @L_800B03BC:
    lwz r3, 0x18(r1)
    bl fn_800B1A08
    mr r30, r3
    lwz r3, 0x18(r1)
    bl fn_800B1E0C
    cmplwi r30, 0x0
    beq @L_800B03E0
    cmplwi r3, 0x0
    bne @L_800B0420
    @L_800B03E0:
    lwz r30, 0x18(r1)
    bl OSDisableInterrupts
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    beq @L_800B0400
    li r0, -0x6
    stw r0, 0x4(r30)
    b @L_800B0414
    @L_800B0400:
    lwz r0, 0x4(r30)
    cmpwi r0, -0x1
    bne @L_800B0414
    li r0, -0x6
    stw r0, 0x4(r30)
    @L_800B0414:
    bl OSRestoreInterrupts
    li r3, -0x6
    b @L_800B04BC
    @L_800B0420:
    cmplwi r29, 0x0
    beq @L_800B043C
    lwz r4, 0x18(r1)
    lhz r0, 0x6(r30)
    lwz r4, 0xc(r4)
    mullw r0, r4, r0
    stw r0, 0x0(r29)
    @L_800B043C:
    cmplwi r31, 0x0
    beq @L_800B0480
    li r0, 0x0
    stw r0, 0x0(r31)
    li r5, 0x0
    b @L_800B0474
    @L_800B0454:
    lbz r0, 0x8(r3)
    cmplwi r0, 0xff
    bne @L_800B046C
    lwz r4, 0x0(r31)
    addi r0, r4, 0x1
    stw r0, 0x0(r31)
    @L_800B046C:
    addi r3, r3, 0x40
    addi r5, r5, 0x1
    @L_800B0474:
    clrlwi r0, r5, 16
    cmplwi r0, 0x7f
    blt @L_800B0454
    @L_800B0480:
    lwz r30, 0x18(r1)
    bl OSDisableInterrupts
    lwz r0, 0x0(r30)
    cmpwi r0, 0x0
    beq @L_800B04A0
    li r0, 0x0
    stw r0, 0x4(r30)
    b @L_800B04B4
    @L_800B04A0:
    lwz r0, 0x4(r30)
    cmpwi r0, -0x1
    bne @L_800B04B4
    li r0, 0x0
    stw r0, 0x4(r30)
    @L_800B04B4:
    bl OSRestoreInterrupts
    li r3, 0x0
    @L_800B04BC:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
