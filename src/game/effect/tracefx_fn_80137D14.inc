    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stfd f30, 0x50(r1)
    psq_st f30, 0x58(r1), 0, 0
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    stw r29, 0x44(r1)
    stw r28, 0x40(r1)
    mr r30, r3
    mr r31, r4
    lhz r3, 0x1c(r3)
    lhz r0, 0x20(r30)
    cmplw r3, r0
    bge @L_80137F24
    lhz r3, 0x24(r30)
    lhz r4, 0x26(r30)
    bl fn_800F9318
    mr. r29, r3
    bne @L_80137D74
    li r3, 0x0
    b @L_80137F28
@L_80137D74:
    lwz r0, 0x14(r30)
    cmplwi r0, 0x0
    bne @L_80137D88
    li r3, 0x0
    b @L_80137F28
@L_80137D88:
    lhz r4, 0x28(r30)
    bl fn_800EE150
    mr. r28, r3
    bne @L_80137DA0
    li r3, 0x0
    b @L_80137F28
@L_80137DA0:
    addi r4, r1, 0x2c
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r28
    bl fn_800EE828
    lhz r4, 0x2a(r30)
    mr r3, r29
    bl fn_800EE150
    mr. r29, r3
    bne @L_80137DD4
    li r3, 0x0
    b @L_80137F28
@L_80137DD4:
    addi r4, r1, 0x20
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r29
    bl fn_800EE828
    lhz r0, 0x1c(r30)
    lwz r29, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_80137E34
    mr r4, r29
    addi r3, r1, 0x14
    bl fn_800E01D0
    addi r3, r1, 0x8
    addi r4, r29, 0xc
    bl fn_800E01D0
    lis r0, 0x4330
    stw r31, 0x3c(r1)
    lfd f1, -25952(r2)
    stw r0, 0x38(r1)
    lfs f2, -25968(r2)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f1
    fdivs f31, f2, f0
@L_80137E34:
    li r28, 0x0
    b @L_80137EB4
@L_80137E3C:
    lhz r0, 0x1c(r30)
    lwz r29, 0x18(r29)
    cmplwi r0, 0x0
    beq @L_80137E98
    addi r3, r28, 0x1
    lis r0, 0x4330
    stw r3, 0x3c(r1)
    mr r3, r29
    lfd f1, -25952(r2)
    addi r4, r1, 0x14
    stw r0, 0x38(r1)
    addi r5, r1, 0x2c
    lfd f0, 0x38(r1)
    fsubs f0, f0, f1
    fmuls f30, f31, f0
    fmr f1, f30
    bl fn_800E090C
    fmr f1, f30
    addi r3, r29, 0xc
    addi r4, r1, 0x8
    addi r5, r1, 0x20
    bl fn_800E090C
    b @L_80137EB0
@L_80137E98:
    mr r3, r29
    addi r4, r1, 0x2c
    bl fn_800E01D0
    addi r3, r29, 0xc
    addi r4, r1, 0x20
    bl fn_800E01D0
@L_80137EB0:
    addi r28, r28, 0x1
@L_80137EB4:
    cmplw r28, r31
    blt @L_80137E3C
    stw r29, 0x0(r30)
    lhz r0, 0x1c(r30)
    add r0, r0, r31
    sth r0, 0x1c(r30)
    lhz r0, 0x1e(r30)
    add r0, r0, r31
    sth r0, 0x1e(r30)
    lhz r0, 0x1c(r30)
    lhz r3, 0x20(r30)
    cmplw r0, r3
    ble @L_80137EEC
    sth r3, 0x1c(r30)
@L_80137EEC:
    lhz r0, 0x1e(r30)
    lhz r3, 0x22(r30)
    cmplw r0, r3
    ble @L_80137F00
    sth r3, 0x1e(r30)
@L_80137F00:
    lhz r3, 0x1c(r30)
    lhz r0, 0x20(r30)
    lhz r4, 0x1e(r30)
    subf r0, r3, r0
    cmpw r4, r0
    ble @L_80137F1C
    sth r0, 0x1e(r30)
@L_80137F1C:
    li r3, 0x1
    b @L_80137F28
@L_80137F24:
    li r3, 0x0
@L_80137F28:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    psq_l f30, 0x58(r1), 0, 0
    lfd f30, 0x50(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    lwz r0, 0x74(r1)
    lwz r28, 0x40(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
