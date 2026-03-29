    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r7
    stw r29, 0x14(r1)
    mr r29, r6
    stw r28, 0x10(r1)
    mr r28, r5
    lwz r31, lbl_8047AC5C(r13)
    lwz r8, lbl_8047AC60(r13)
    mr r5, r31
    mtctr r8
    cmplwi r8, 0x0
    beq @L_800F9488
@L_800F9458:
    lwz r0, 0x4(r5)
    cmplwi r0, 0x0
    beq @L_800F9480
    lwz r0, 0x8(r5)
    cmplw r0, r28
    bne @L_800F9480
    lwz r0, 0xc(r5)
    cmplw r0, r29
    bne @L_800F9480
    b @L_800F948C
@L_800F9480:
    addi r5, r5, 0x14
    bdnz @L_800F9458
@L_800F9488:
    li r5, 0x0
@L_800F948C:
    cmplwi r5, 0x0
    beq @L_800F949C
    li r3, 0x0
    b @L_800F9524
@L_800F949C:
    mtctr r8
    cmplwi r8, 0x0
    ble @L_800F94C0
@L_800F94A8:
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    bne @L_800F94B8
    b @L_800F94C4
@L_800F94B8:
    addi r31, r31, 0x14
    bdnz @L_800F94A8
@L_800F94C0:
    li r31, 0x0
@L_800F94C4:
    cmplwi r31, 0x0
    bne @L_800F94D4
    li r3, 0x0
    b @L_800F9524
@L_800F94D4:
    bl fn_800E2C04
    sth r3, 0x0(r31)
    lhz r3, 0x0(r31)
    cmplwi r3, 0x0
    bne @L_800F94F0
    li r3, 0x0
    b @L_800F9524
@L_800F94F0:
    bl fn_800E27B0
    stw r3, 0x4(r31)
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    bne @L_800F9514
    lhz r3, 0x0(r31)
    bl fn_800E209C
    li r3, 0x0
    b @L_800F9524
@L_800F9514:
    stw r28, 0x8(r31)
    stw r29, 0xc(r31)
    stw r30, 0x10(r31)
    lwz r3, 0x4(r31)
@L_800F9524:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
