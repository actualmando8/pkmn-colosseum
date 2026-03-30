    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    li r31, 0x0
    stw r30, 0x18(r1)
    mr r30, r3
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    li r28, 0x0
    b @L_800F7240
    @L_800F7198:
    lwz r0, 0xc(r3)
    add r29, r0, r28
    lbz r0, 0x4(r29)
    cmplwi r0, 0x0
    beq @L_800F7238
    lwz r3, 0xc(r29)
    cmplwi r3, 0x0
    beq @L_800F7238
    bl fn_800F0374
    cmplw r3, r30
    bne @L_800F7238
    lwz r5, lbl_80478B00(r13)
    li r4, 0x0
    lhz r3, 0x6(r29)
    lhz r0, 0x0(r5)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_800F720C
    @L_800F71E0:
    lwz r0, 0xc(r5)
    add r29, r0, r4
    lbz r0, 0x4(r29)
    cmplwi r0, 0x0
    beq @L_800F7204
    lhz r0, 0x6(r29)
    cmplw r0, r3
    bne @L_800F7204
    b @L_800F7210
    @L_800F7204:
    addi r4, r4, 0x16c
    bdnz @L_800F71E0
    @L_800F720C:
    li r29, 0x0
    @L_800F7210:
    cmplwi r29, 0x0
    beq @L_800F7238
    li r0, 0x4
    stb r0, 0x4(r29)
    lwz r3, 0xc(r29)
    cmplwi r3, 0x0
    beq @L_800F7238
    bl fn_800F05A0
    li r0, 0x0
    stw r0, 0xc(r29)
    @L_800F7238:
    addi r28, r28, 0x16c
    addi r31, r31, 0x1
    @L_800F7240:
    lwz r3, lbl_80478B00(r13)
    lhz r0, 0x0(r3)
    cmpw r31, r0
    blt @L_800F7198
    lwz r0, 0x24(r1)
    li r3, 0x0
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
