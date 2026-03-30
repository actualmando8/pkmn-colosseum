    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r3, lbl_80466DE8@ha
    stw r0, 0x34(r1)
    addi r3, r3, lbl_80466DE8@l
    stmw r23, 0xc(r1)
    lbz r0, lbl_8047B398(r13)
    lhz r31, 0x40(r3)
    cmplwi r0, 0x0
    bne @L_801C2DB4
    lbz r0, lbl_8047B399(r13)
    cmplwi r0, 0x0
    beq @L_801C2DC0
    @L_801C2DB4:
    li r0, 0xc8
    stw r0, lbl_80478CA8(r13)
    b @L_801C2EEC
    @L_801C2DC0:
    bl fn_801D2C6C
    cmplwi r3, 0x0
    beq @L_801C2DD8
    li r0, 0xc8
    stw r0, lbl_80478CA8(r13)
    b @L_801C2EEC
    @L_801C2DD8:
    bl fn_800D3088
    lwz r0, lbl_80478CA8(r13)
    subf. r0, r3, r0
    stw r0, lbl_80478CA8(r13)
    bgt @L_801C2EEC
    lis r3, lbl_80466DE8@ha
    addi r30, r3, lbl_80466DE8@l
    lhz r0, 0x42(r30)
    add. r0, r31, r0
    beq @L_801C2EE4
    bl fn_800E0BE4
    lfs f0, lbl_8047DF58(r2)
    fcmpo cr0, f1, f0
    ble @L_801C2E1C
    li r0, 0xc8
    stw r0, lbl_80478CA8(r13)
    b @L_801C2EEC
    @L_801C2E1C:
    lhz r0, 0x42(r30)
    add r3, r31, r0
    bl fn_800E0C04
    mr r24, r3
    b @L_801C2E40
    @L_801C2E30:
    lhz r0, 0x42(r30)
    add r3, r31, r0
    bl fn_800E0C04
    mr r24, r3
    @L_801C2E40:
    lwz r0, lbl_80478CAC(r13)
    cmplw r24, r0
    beq @L_801C2E30
    lis r3, lbl_80466DE8@ha
    stw r24, lbl_80478CAC(r13)
    addi r0, r3, lbl_80466DE8@l
    li r23, 0x0
    mr r25, r0
    li r28, 0x0
    b @L_801C2ED8
    @L_801C2E68:
    lwz r3, 0x0(r25)
    cmplwi r3, 0x0
    beq @L_801C2ED0
    cmplw r23, r24
    bne @L_801C2E88
    bl fn_801D2C74
    li r0, 0xc8
    stw r0, lbl_80478CA8(r13)
    @L_801C2E88:
    lhz r26, 0xc(r25)
    mr r29, r25
    li r27, 0x0
    li r30, 0xc8
    addi r23, r23, 0x1
    b @L_801C2EC8
    @L_801C2EA0:
    lwz r3, 0x4(r29)
    cmplwi r3, 0x0
    beq @L_801C2EC0
    cmplw r23, r24
    bne @L_801C2EBC
    bl fn_801D2C74
    stw r30, lbl_80478CA8(r13)
    @L_801C2EBC:
    addi r23, r23, 0x1
    @L_801C2EC0:
    addi r29, r29, 0x4
    addi r27, r27, 0x1
    @L_801C2EC8:
    cmpw r27, r26
    blt @L_801C2EA0
    @L_801C2ED0:
    addi r28, r28, 0x1
    addi r25, r25, 0x10
    @L_801C2ED8:
    cmpw r28, r31
    blt @L_801C2E68
    b @L_801C2EEC
    @L_801C2EE4:
    li r0, 0xc8
    stw r0, lbl_80478CA8(r13)
    @L_801C2EEC:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
