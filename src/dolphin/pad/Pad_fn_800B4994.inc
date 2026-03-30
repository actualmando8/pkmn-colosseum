    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x30(r1)
    stw r31, 0x2c(r1)
    addi r31, r6, 0x0
    stw r30, 0x28(r1)
    addi r30, r5, 0x0
    stw r29, 0x24(r1)
    addi r29, r4, 0x0
    addi r4, r1, 0x18
    stw r28, 0x20(r1)
    mr r28, r3
    lwz r3, 0x0(r3)
    bl fn_800B023C
    cmpwi r3, 0x0
    bge @L_800B49D8
    b @L_800B4B2C
    @L_800B49D8:
    lhz r0, 0x10(r28)
    cmplwi r0, 0x5
    blt @L_800B4A08
    lwz r3, 0x18(r1)
    lhz r5, 0x10(r3)
    cmplw r0, r5
    bge @L_800B4A08
    lwz r4, 0xc(r3)
    lwz r0, 0x8(r28)
    mullw r4, r5, r4
    cmpw r4, r0
    bgt @L_800B4A18
    @L_800B4A08:
    lwz r3, 0x18(r1)
    li r4, -0x80
    bl fn_800B02F4
    b @L_800B4B2C
    @L_800B4A18:
    bl fn_800B1E0C
    lwz r0, 0x4(r28)
    lwz r5, 0x18(r1)
    slwi r0, r0, 6
    add r4, r3, r0
    lwz r3, 0xc(r5)
    lhz r0, 0x38(r4)
    mullw r3, r0, r3
    cmpw r3, r30
    ble @L_800B4A4C
    add r0, r30, r29
    cmpw r3, r0
    bge @L_800B4A5C
    @L_800B4A4C:
    addi r3, r5, 0x0
    li r4, -0xb
    bl fn_800B02F4
    b @L_800B4B2C
    @L_800B4A5C:
    stw r28, 0xc0(r5)
    stw r29, 0xc(r28)
    lwz r0, 0x8(r28)
    cmpw r30, r0
    bge @L_800B4AAC
    li r0, 0x0
    stw r0, 0x8(r28)
    lhz r0, 0x36(r4)
    sth r0, 0x10(r28)
    lhz r4, 0x10(r28)
    cmplwi r4, 0x5
    blt @L_800B4A9C
    lwz r3, 0x18(r1)
    lhz r0, 0x10(r3)
    cmplw r4, r0
    blt @L_800B4AAC
    @L_800B4A9C:
    lwz r3, 0x18(r1)
    li r4, -0x6
    bl fn_800B02F4
    b @L_800B4B2C
    @L_800B4AAC:
    lwz r3, 0x18(r1)
    bl fn_800B1A08
    b @L_800B4B00
    @L_800B4AB8:
    lwz r0, 0x8(r28)
    add r0, r0, r4
    stw r0, 0x8(r28)
    lhz r0, 0x10(r28)
    slwi r0, r0, 1
    lhzx r0, r3, r0
    sth r0, 0x10(r28)
    lhz r5, 0x10(r28)
    cmplwi r5, 0x5
    blt @L_800B4AF0
    lwz r4, 0x18(r1)
    lhz r0, 0x10(r4)
    cmplw r5, r0
    blt @L_800B4B00
    @L_800B4AF0:
    lwz r3, 0x18(r1)
    li r4, -0x6
    bl fn_800B02F4
    b @L_800B4B2C
    @L_800B4B00:
    lwz r4, 0x18(r1)
    lwz r5, 0x8(r28)
    lwz r4, 0xc(r4)
    subi r0, r4, 0x1
    andc r0, r30, r0
    cmplw r5, r0
    blt @L_800B4AB8
    stw r30, 0x8(r28)
    li r3, 0x0
    lwz r0, 0x18(r1)
    stw r0, 0x0(r31)
    @L_800B4B2C:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    addi r1, r1, 0x30
    mtlr r0
    blr
