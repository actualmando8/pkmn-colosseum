    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r4, lbl_80466DE8@ha
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    addi r29, r4, lbl_80466DE8@l
    mr r26, r3
    li r31, -0x2
    lhz r0, 0x40(r29)
    cmplwi r0, 0x0
    bne @L_801C3A98
    li r31, 0x0
    b @L_801C3AFC
    @L_801C3A98:
    li r27, 0x0
    b @L_801C3AF0
    @L_801C3AA0:
    lwz r0, 0x0(r29)
    cmplwi r0, 0x0
    beq @L_801C3AE8
    addi r30, r29, 0x4
    li r28, 0x0
    b @L_801C3ADC
    @L_801C3AB8:
    lwz r3, 0x0(r30)
    cmplwi r3, 0x0
    beq @L_801C3AD4
    bl fn_801DAC24
    cmpw r3, r31
    ble @L_801C3AD4
    mr r31, r3
    @L_801C3AD4:
    addi r28, r28, 0x1
    addi r30, r30, 0x4
    @L_801C3ADC:
    clrlwi r0, r28, 16
    cmplwi r0, 0x2
    blt @L_801C3AB8
    @L_801C3AE8:
    addi r27, r27, 0x1
    addi r29, r29, 0x10
    @L_801C3AF0:
    clrlwi r0, r27, 16
    cmplwi r0, 0x4
    blt @L_801C3AA0
    @L_801C3AFC:
    cmpwi r31, 0x1
    beq @L_801C3B34
    bge @L_801C3B1C
    cmpwi r31, 0x0
    bge @L_801C3B4C
    cmpwi r31, -0x2
    bge @L_801C3B2C
    b @L_801C3B4C
    @L_801C3B1C:
    cmpwi r31, 0x3
    beq @L_801C3B44
    bge @L_801C3B4C
    b @L_801C3B3C
    @L_801C3B2C:
    lfs f1, lbl_8047DF6C(r2)
    b @L_801C3B50
    @L_801C3B34:
    lfs f1, lbl_8047DF64(r2)
    b @L_801C3B50
    @L_801C3B3C:
    lfs f1, lbl_8047DF70(r2)
    b @L_801C3B50
    @L_801C3B44:
    lfs f1, lbl_8047DF74(r2)
    b @L_801C3B50
    @L_801C3B4C:
    lfs f1, lbl_8047DF68(r2)
    @L_801C3B50:
    clrlwi. r0, r26, 24
    beq @L_801C3B64
    lfs f0, lbl_8047DF7C(r2)
    fmuls f1, f0, f1
    b @L_801C3B6C
    @L_801C3B64:
    lfs f0, lbl_8047DF78(r2)
    fmuls f1, f0, f1
    @L_801C3B6C:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
