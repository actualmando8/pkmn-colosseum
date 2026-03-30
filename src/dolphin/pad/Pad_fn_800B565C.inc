    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    mr. r31, r4
    stw r30, 0x28(r1)
    addi r30, r6, 0x0
    stw r29, 0x24(r1)
    addi r29, r5, 0x0
    stw r28, 0x20(r1)
    addi r28, r3, 0x0
    blt @L_800B56C8
    cmpwi r31, 0x7f
    bge @L_800B56C8
    lwz r3, 0x30(r29)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_800B56AC
    cmplwi r3, 0x200
    bge @L_800B56C8
    @L_800B56AC:
    lwz r3, 0x38(r29)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    beq @L_800B56D0
    clrlwi r0, r3, 19
    cmplwi r0, 0x1fc0
    ble @L_800B56D0
    @L_800B56C8:
    li r3, -0x80
    b @L_800B57B0
    @L_800B56D0:
    addi r3, r28, 0x0
    addi r4, r1, 0x18
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B56E8
    b @L_800B57B0
    @L_800B56E8:
    lwz r3, 0x18(r1)
    bl fn_800B1E0C
    slwi r0, r31, 6
    add r31, r3, r0
    lwz r3, 0x18(r1)
    mr r4, r31
    bl fn_800B4270
    mr. r4, r3
    bge @L_800B5718
    lwz r3, 0x18(r1)
    bl fn_800B02F4
    b @L_800B57B0
    @L_800B5718:
    lbz r0, 0x2e(r29)
    addi r3, r31, 0x0
    addi r4, r29, 0x0
    stb r0, 0x7(r31)
    lwz r0, 0x30(r29)
    stw r0, 0x2c(r31)
    lhz r0, 0x34(r29)
    sth r0, 0x30(r31)
    lhz r0, 0x36(r29)
    sth r0, 0x32(r31)
    lwz r0, 0x38(r29)
    stw r0, 0x3c(r31)
    bl fn_800B5338
    lwz r3, 0x2c(r31)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_800B576C
    lhz r0, 0x32(r31)
    clrrwi r0, r0, 2
    ori r0, r0, 0x1
    sth r0, 0x32(r31)
    @L_800B576C:
    lis r3, 0x8000
    lwz r0, 0xf8(r3)
    srwi r29, r0, 2
    bl OSGetTime
    addi r6, r29, 0x0
    li r5, 0x0
    bl fn_800C4928
    stw r4, 0x28(r31)
    addi r3, r28, 0x0
    addi r4, r30, 0x0
    bl fn_800B1FAC
    mr. r28, r3
    bge @L_800B57AC
    lwz r3, 0x18(r1)
    mr r4, r28
    bl fn_800B02F4
    @L_800B57AC:
    mr r3, r28
    @L_800B57B0:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
