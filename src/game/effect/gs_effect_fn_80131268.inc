    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    beq @L_801312BC
    lis r4, -32714
    addi r4, r4, 13760
    lwz r0, 0x0(r4)
    cmplw r3, r0
    bgt @L_801312BC
    subi r0, r3, 0x1
    lwz r3, 0xc(r4)
    mulli r0, r0, 0x34
    add r31, r3, r0
    lwz r0, 0x4(r31)
    cmpwi r0, -0x1
    beq @L_801312BC
    b @L_801312C0
@L_801312BC:
    li r31, 0x0
@L_801312C0:
    cmplwi r31, 0x0
    beq @L_80131380
    lwz r0, 0x4(r31)
    lwz r30, 0x30(r31)
    cmpwi r0, 0x0
    lwz r29, 0x2c(r31)
    beq @L_8013131C
    cmpwi r0, 0x1
    beq @L_801312FC
    lwz r12, 0x14(r31)
    cmplwi r12, 0x0
    beq @L_801312FC
    lwz r3, 0x24(r31)
    mtctr r12
    bctrl
@L_801312FC:
    lwz r12, 0xc(r31)
    cmplwi r12, 0x0
    beq @L_80131314
    lwz r3, 0x24(r31)
    mtctr r12
    bctrl
@L_80131314:
    li r0, -0x1
    stw r0, 0x4(r31)
@L_8013131C:
    cmplwi r30, 0x0
    beq @L_8013132C
    stw r29, 0x2c(r30)
    b @L_80131338
@L_8013132C:
    lis r3, -32714
    addi r3, r3, 13760
    stw r29, 0x10(r3)
@L_80131338:
    cmplwi r29, 0x0
    beq @L_80131344
    stw r30, 0x30(r29)
@L_80131344:
    li r0, 0x0
    lis r3, -32714
    stw r0, 0x30(r31)
    addi r3, r3, 13760
    lwz r0, 0x8(r3)
    stw r0, 0x2c(r31)
    lwz r4, 0x8(r3)
    cmplwi r4, 0x0
    beq @L_8013136C
    stw r31, 0x30(r4)
@L_8013136C:
    stw r31, 0x8(r3)
    lhz r3, 0x28(r31)
    bl fn_800E24B0
    lhz r3, 0x28(r31)
    bl fn_800E209C
@L_80131380:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
