    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x28(r1)
    stw r31, 0x24(r1)
    addi r31, r5, 0x0
    stw r30, 0x20(r1)
    stw r29, 0x1c(r1)
    mr. r29, r4
    blt @L_800B555C
    cmpwi r29, 0x7f
    blt @L_800B5564
    @L_800B555C:
    li r3, -0x80
    b @L_800B5640
    @L_800B5564:
    addi r4, r1, 0x14
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B5578
    b @L_800B5640
    @L_800B5578:
    lwz r3, 0x14(r1)
    bl fn_800B1E0C
    slwi r0, r29, 6
    add r29, r3, r0
    lwz r3, 0x14(r1)
    mr r4, r29
    bl fn_800B4270
    addi r30, r3, 0x0
    cmpwi r30, -0xa
    bne @L_800B55AC
    mr r3, r29
    bl fn_800B4308
    mr r30, r3
    @L_800B55AC:
    cmpwi r30, 0x0
    blt @L_800B5634
    addi r4, r29, 0x0
    addi r3, r31, 0x28
    li r5, 0x4
    bl memcpy
    addi r3, r31, 0x2c
    addi r4, r29, 0x4
    li r5, 0x2
    bl memcpy
    lwz r5, 0x14(r1)
    mr r3, r31
    lhz r6, 0x38(r29)
    addi r4, r29, 0x8
    lwz r0, 0xc(r5)
    li r5, 0x20
    mullw r0, r6, r0
    stw r0, 0x20(r31)
    bl memcpy
    lwz r0, 0x28(r29)
    addi r3, r29, 0x0
    addi r4, r31, 0x0
    stw r0, 0x24(r31)
    lbz r0, 0x7(r29)
    stb r0, 0x2e(r31)
    lwz r0, 0x2c(r29)
    stw r0, 0x30(r31)
    lhz r0, 0x30(r29)
    sth r0, 0x34(r31)
    lhz r0, 0x32(r29)
    sth r0, 0x36(r31)
    lwz r0, 0x3c(r29)
    stw r0, 0x38(r31)
    bl fn_800B5338
    @L_800B5634:
    lwz r3, 0x14(r1)
    mr r4, r30
    bl fn_800B02F4
    @L_800B5640:
    lwz r0, 0x2c(r1)
    lwz r31, 0x24(r1)
    lwz r30, 0x20(r1)
    lwz r29, 0x1c(r1)
    addi r1, r1, 0x28
    mtlr r0
    blr
