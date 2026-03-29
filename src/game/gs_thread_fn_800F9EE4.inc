    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r3
    mr r31, r4
    bl fn_800FDFE4
    mr r29, r3
    mr r3, r31
    bl fn_800FDFE4
    cmplw r29, r3
    bne @L_800F9F80
    mr r4, r31
    mr r5, r30
    li r6, 0x0
    mtctr r3
    cmplwi r3, 0x0
    ble @L_800F9F78
@L_800F9F34:
    lhz r3, 0x0(r5)
    lhz r0, 0x0(r4)
    cmplw r3, r0
    beq @L_800F9F68
    slwi r0, r6, 1
    lhzx r3, r30, r0
    lhzx r0, r31, r0
    cmplw r3, r0
    ble @L_800F9F60
    li r3, 0x1
    b @L_800FA048
@L_800F9F60:
    li r3, -0x1
    b @L_800FA048
@L_800F9F68:
    addi r4, r4, 0x2
    addi r5, r5, 0x2
    addi r6, r6, 0x1
    bdnz @L_800F9F34
@L_800F9F78:
    li r3, 0x0
    b @L_800FA048
@L_800F9F80:
    ble @L_800F9FE8
    mr r4, r31
    mr r5, r30
    li r6, 0x0
    mtctr r3
    cmplwi r3, 0x0
    ble @L_800F9FE0
@L_800F9F9C:
    lhz r3, 0x0(r5)
    lhz r0, 0x0(r4)
    cmplw r3, r0
    beq @L_800F9FD0
    slwi r0, r6, 1
    lhzx r3, r30, r0
    lhzx r0, r31, r0
    cmplw r3, r0
    ble @L_800F9FC8
    li r3, 0x1
    b @L_800FA048
@L_800F9FC8:
    li r3, -0x1
    b @L_800FA048
@L_800F9FD0:
    addi r4, r4, 0x2
    addi r5, r5, 0x2
    addi r6, r6, 0x1
    bdnz @L_800F9F9C
@L_800F9FE0:
    li r3, 0x1
    b @L_800FA048
@L_800F9FE8:
    mr r4, r31
    mr r5, r30
    li r6, 0x0
    mtctr r29
    cmplwi r29, 0x0
    ble @L_800FA044
@L_800FA000:
    lhz r3, 0x0(r5)
    lhz r0, 0x0(r4)
    cmplw r3, r0
    beq @L_800FA034
    slwi r0, r6, 1
    lhzx r3, r30, r0
    lhzx r0, r31, r0
    cmplw r3, r0
    ble @L_800FA02C
    li r3, 0x1
    b @L_800FA048
@L_800FA02C:
    li r3, -0x1
    b @L_800FA048
@L_800FA034:
    addi r4, r4, 0x2
    addi r5, r5, 0x2
    addi r6, r6, 0x1
    bdnz @L_800FA000
@L_800FA044:
    li r3, -0x1
@L_800FA048:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
