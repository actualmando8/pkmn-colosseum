    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r31, r3
    bl fn_80185AAC
    cmpwi r3, 0x0
    beq @L_801858EC
    li r0, 0x0
    stb r0, 0x54(r31)
    @L_801858EC:
    cmplwi r31, 0x0
    beq @L_80185A98
    mr r3, r31
    bl fn_8018FCBC
    mr r0, r3
    mr r3, r31
    mr r27, r0
    bl fn_8018FC00
    mr r5, r3
    mr r4, r27
    addi r3, r1, 0x8
    bl fn_800E0168
    lfs f0, 0x8(r1)
    lfd f1, lbl_8047D828(r2)
    fabs f0, f0
    fcmpo cr0, f0, f1
    bge @L_80185A90
    lfs f0, 0xc(r1)
    fabs f0, f0
    fcmpo cr0, f0, f1
    bge @L_80185A90
    lfs f0, 0x10(r1)
    fabs f0, f0
    fcmpo cr0, f0, f1
    bge @L_80185A90
    lbz r3, 0x97(r31)
    addi r0, r3, 0x1
    stb r0, 0x97(r31)
    lbz r0, 0x97(r31)
    cmplwi r0, 0x3c
    ble @L_80185A98
    lwz r28, 0x2c(r31)
    li r30, 0x0
    lwz r29, 0x28(r31)
    b @L_801859B0
    @L_80185978:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801859AC
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_801859AC
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_801859AC
    lwz r28, 0x4(r3)
    b @L_80185A1C
    @L_801859AC:
    addi r30, r30, 0x1
    @L_801859B0:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80185978
    li r27, 0x0
    b @L_80185A0C
    @L_801859C4:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185A08
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_80185A08
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_80185A1C
    @L_80185A08:
    addi r27, r27, 0x1
    @L_80185A0C:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_801859C4
    li r28, 0x0
    @L_80185A1C:
    li r29, 0x0
    b @L_80185A50
    @L_80185A24:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185A4C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_80185A4C
    mr r28, r3
    b @L_80185A60
    @L_80185A4C:
    addi r29, r29, 0x1
    @L_80185A50:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80185A24
    li r28, 0x0
    @L_80185A60:
    cmplwi r28, 0x0
    beq @L_80185A80
    mr r3, r28
    addi r4, r31, 0x5c
    bl fn_8018FC74
    mr r3, r28
    addi r4, r31, 0x5c
    bl fn_8018FBDC
    @L_80185A80:
    li r0, 0x0
    stb r0, 0x54(r31)
    stb r0, 0x97(r31)
    b @L_80185A98
    @L_80185A90:
    li r0, 0x0
    stb r0, 0x97(r31)
    @L_80185A98:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
