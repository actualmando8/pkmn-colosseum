    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    lis r3, lbl_803A9A08@ha
    li r30, 0x0
    addi r3, r3, lbl_803A9A08@l
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_80057CEC
    lwz r0, 0x4(r3)
    cmpwi r0, 0x0
    beq @L_80057CE4
    li r0, 0x1
    b @L_80057CF0
    @L_80057CE4:
    li r0, 0x0
    b @L_80057CF0
    @L_80057CEC:
    li r0, 0x2
    @L_80057CF0:
    cmpwi r0, 0x2
    beq @L_80057D48
    lis r3, lbl_803A9A08@ha
    li r0, 0x1
    addi r31, r3, lbl_803A9A08@l
    li r30, 0x1
    stw r0, 0x8(r31)
    b @L_80057D14
    @L_80057D10:
    bl fn_800F0308
    @L_80057D14:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    beq @L_80057D3C
    lwz r0, 0x4(r31)
    cmpwi r0, 0x0
    beq @L_80057D34
    li r0, 0x1
    b @L_80057D40
    @L_80057D34:
    li r0, 0x0
    b @L_80057D40
    @L_80057D3C:
    li r0, 0x2
    @L_80057D40:
    cmpwi r0, 0x1
    bne @L_80057D10
    @L_80057D48:
    li r3, 0x70b
    bl fn_8017B1CC
    li r3, 0x70b
    bl fn_800F915C
    li r3, 0x66f
    bl fn_8017B3E4
    b @L_80057D68
    @L_80057D64:
    bl fn_800F0308
    @L_80057D68:
    li r3, 0x66f
    bl fn_8017B2CC
    cmpwi r3, 0x1
    beq @L_80057D64
    mr r3, r27
    mr r4, r28
    mr r5, r29
    bl fn_80097D94
    li r3, 0x66f
    bl fn_8017B1CC
    li r3, 0x66f
    bl fn_800F915C
    li r3, 0x70b
    bl fn_8017B3E4
    b @L_80057DA8
    @L_80057DA4:
    bl fn_800F0308
    @L_80057DA8:
    li r3, 0x70b
    bl fn_8017B2CC
    cmpwi r3, 0x1
    beq @L_80057DA4
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_80057DD4
    lis r3, lbl_803A9A08@ha
    li r0, 0x0
    addi r3, r3, lbl_803A9A08@l
    stw r0, 0x8(r3)
    @L_80057DD4:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
