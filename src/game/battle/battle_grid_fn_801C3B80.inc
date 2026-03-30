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
    bne @L_801C3BB4
    li r31, 0x0
    b @L_801C3C18
    @L_801C3BB4:
    li r27, 0x0
    b @L_801C3C0C
    @L_801C3BBC:
    lwz r0, 0x0(r29)
    cmplwi r0, 0x0
    beq @L_801C3C04
    addi r30, r29, 0x4
    li r28, 0x0
    b @L_801C3BF8
    @L_801C3BD4:
    lwz r3, 0x0(r30)
    cmplwi r3, 0x0
    beq @L_801C3BF0
    bl fn_801DAC24
    cmpw r3, r31
    ble @L_801C3BF0
    mr r31, r3
    @L_801C3BF0:
    addi r28, r28, 0x1
    addi r30, r30, 0x4
    @L_801C3BF8:
    clrlwi r0, r28, 16
    cmplwi r0, 0x2
    blt @L_801C3BD4
    @L_801C3C04:
    addi r27, r27, 0x1
    addi r29, r29, 0x10
    @L_801C3C0C:
    clrlwi r0, r27, 16
    cmplwi r0, 0x4
    blt @L_801C3BBC
    @L_801C3C18:
    cmpwi r31, 0x1
    beq @L_801C3C50
    bge @L_801C3C38
    cmpwi r31, 0x0
    bge @L_801C3C68
    cmpwi r31, -0x2
    bge @L_801C3C48
    b @L_801C3C68
    @L_801C3C38:
    cmpwi r31, 0x3
    beq @L_801C3C60
    bge @L_801C3C68
    b @L_801C3C58
    @L_801C3C48:
    lfs f1, lbl_8047DF6C(r2)
    b @L_801C3C6C
    @L_801C3C50:
    lfs f1, lbl_8047DF64(r2)
    b @L_801C3C6C
    @L_801C3C58:
    lfs f1, lbl_8047DF70(r2)
    b @L_801C3C6C
    @L_801C3C60:
    lfs f1, lbl_8047DF74(r2)
    b @L_801C3C6C
    @L_801C3C68:
    lfs f1, lbl_8047DF68(r2)
    @L_801C3C6C:
    lfs f0, lbl_8047DFA0(r2)
    mr r3, r26
    fmuls f1, f1, f0
    fmr f2, f1
    fmr f3, f1
    bl fn_800E01F4
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
