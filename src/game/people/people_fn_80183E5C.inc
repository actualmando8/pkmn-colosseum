    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r31, r5
    li r30, 0x0
    b @L_80183EB8
    @L_80183E80:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183EB4
    lwz r0, 0x28(r3)
    cmplw r0, r27
    bne @L_80183EB4
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_80183EB4
    lwz r30, 0x4(r3)
    b @L_80183F24
    @L_80183EB4:
    addi r30, r30, 0x1
    @L_80183EB8:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80183E80
    li r29, 0x0
    b @L_80183F14
    @L_80183ECC:
    mr r3, r29
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183F10
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_80183F10
    lis r3, lbl_80273FD8@ha
    mr r4, r27
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r30, 0x4(r30)
    b @L_80183F24
    @L_80183F10:
    addi r29, r29, 0x1
    @L_80183F14:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80183ECC
    li r30, 0x0
    @L_80183F24:
    li r29, 0x0
    b @L_80183F54
    @L_80183F2C:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183F50
    lwz r0, 0x4(r3)
    cmplw r0, r30
    bne @L_80183F50
    b @L_80183F64
    @L_80183F50:
    addi r29, r29, 0x1
    @L_80183F54:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80183F2C
    li r3, 0x0
    @L_80183F64:
    cmplwi r3, 0x0
    bne @L_80183F74
    li r3, 0x0
    b @L_80183FB0
    @L_80183F74:
    li r4, 0x0
    lfs f0, lbl_8047D79C(r2)
    stb r4, 0x55(r3)
    cmplwi r31, 0x0
    li r0, 0x1
    sth r4, 0x6a(r3)
    stfs f0, 0x58(r3)
    stb r0, 0x55(r3)
    beq @L_80183FA4
    li r0, 0x3
    stb r0, 0x54(r3)
    b @L_80183FAC
    @L_80183FA4:
    li r0, 0x2
    stb r0, 0x54(r3)
    @L_80183FAC:
    li r3, 0x1
    @L_80183FB0:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
