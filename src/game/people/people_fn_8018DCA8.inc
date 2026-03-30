    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r27, r3
    mr r28, r4
    li r26, 0x0
    lwz r30, 0x2c(r3)
    lwz r31, 0x28(r3)
    b @L_8018DD08
    @L_8018DCD0:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DD04
    lwz r0, 0x28(r3)
    cmplw r0, r31
    bne @L_8018DD04
    lwz r0, 0x2c(r3)
    cmplw r0, r30
    bne @L_8018DD04
    lwz r26, 0x4(r3)
    b @L_8018DD74
    @L_8018DD04:
    addi r26, r26, 0x1
    @L_8018DD08:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_8018DCD0
    li r29, 0x0
    b @L_8018DD64
    @L_8018DD1C:
    mr r3, r29
    bl fn_8018FD88
    mr r26, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DD60
    lwz r0, 0x2c(r26)
    cmplw r0, r30
    bne @L_8018DD60
    lis r3, lbl_80273FD8@ha
    mr r4, r31
    addi r3, r3, lbl_80273FD8@l
    mr r5, r30
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r26)
    b @L_8018DD74
    @L_8018DD60:
    addi r29, r29, 0x1
    @L_8018DD64:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018DD1C
    li r26, 0x0
    @L_8018DD74:
    li r29, 0x0
    b @L_8018DDA4
    @L_8018DD7C:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DDA0
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_8018DDA0
    b @L_8018DDB4
    @L_8018DDA0:
    addi r29, r29, 0x1
    @L_8018DDA4:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_8018DD7C
    li r3, 0x0
    @L_8018DDB4:
    cmplwi r3, 0x0
    beq @L_8018DED4
    li r4, 0x0
    bl fn_8018FB60
    li r29, 0x0
    mr r26, r29
    b @L_8018DE08
    @L_8018DDD0:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DE04
    lwz r0, 0x28(r3)
    cmplw r0, r31
    bne @L_8018DE04
    lwz r0, 0x2c(r3)
    cmplw r0, r30
    bne @L_8018DE04
    lwz r26, 0x4(r3)
    b @L_8018DE74
    @L_8018DE04:
    addi r26, r26, 0x1
    @L_8018DE08:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_8018DDD0
    li r25, 0x0
    b @L_8018DE64
    @L_8018DE1C:
    mr r3, r25
    bl fn_8018FD88
    mr r26, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DE60
    lwz r0, 0x2c(r26)
    cmplw r0, r30
    bne @L_8018DE60
    lis r3, lbl_80273FD8@ha
    mr r4, r31
    addi r3, r3, lbl_80273FD8@l
    mr r5, r30
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r26)
    b @L_8018DE74
    @L_8018DE60:
    addi r25, r25, 0x1
    @L_8018DE64:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018DE1C
    li r26, 0x0
    @L_8018DE74:
    li r30, 0x0
    b @L_8018DEA4
    @L_8018DE7C:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DEA0
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_8018DEA0
    b @L_8018DEB4
    @L_8018DEA0:
    addi r30, r30, 0x1
    @L_8018DEA4:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_8018DE7C
    li r3, 0x0
    @L_8018DEB4:
    cmplwi r3, 0x0
    beq @L_8018DED4
    lbz r0, 0x21(r3)
    cmplwi r0, 0x0
    bne @L_8018DECC
    li r29, 0x0
    @L_8018DECC:
    mr r4, r29
    bl fn_8018FB2C
    @L_8018DED4:
    clrlwi. r0, r28, 24
    beq @L_8018E010
    lwz r29, 0x2c(r27)
    li r28, 0x0
    lwz r26, 0x28(r27)
    b @L_8018DF24
    @L_8018DEEC:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DF20
    lwz r0, 0x28(r3)
    cmplw r0, r26
    bne @L_8018DF20
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_8018DF20
    lwz r26, 0x4(r3)
    b @L_8018DF90
    @L_8018DF20:
    addi r28, r28, 0x1
    @L_8018DF24:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018DEEC
    li r25, 0x0
    b @L_8018DF80
    @L_8018DF38:
    mr r3, r25
    bl fn_8018FD88
    mr r28, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DF7C
    lwz r0, 0x2c(r28)
    cmplw r0, r29
    bne @L_8018DF7C
    lis r3, lbl_80273FD8@ha
    mr r4, r26
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r26, 0x4(r28)
    b @L_8018DF90
    @L_8018DF7C:
    addi r25, r25, 0x1
    @L_8018DF80:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_8018DF38
    li r26, 0x0
    @L_8018DF90:
    li r28, 0x0
    b @L_8018DFC4
    @L_8018DF98:
    mr r3, r28
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018DFC0
    lwz r0, 0x4(r3)
    cmplw r0, r26
    bne @L_8018DFC0
    mr r26, r3
    b @L_8018DFD4
    @L_8018DFC0:
    addi r28, r28, 0x1
    @L_8018DFC4:
    bl fn_8018FDB4
    cmpw r28, r3
    blt @L_8018DF98
    li r26, 0x0
    @L_8018DFD4:
    cmplwi r26, 0x0
    beq @L_8018E010
    lhz r3, 0x6c(r26)
    cmplwi r3, 0x0
    beq @L_8018E010
    bl fn_800E24B0
    lhz r3, 0x6c(r26)
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0x70(r26)
    sth r0, 0x6c(r26)
    stb r0, 0x54(r26)
    sth r0, 0x68(r26)
    sth r0, 0x6a(r26)
    stb r0, 0x55(r26)
    @L_8018E010:
    lwz r3, 0x28(r27)
    lwz r4, 0x2c(r27)
    bl fn_800F9210
    lwz r3, 0x8(r27)
    cmplwi r3, 0x0
    beq @L_8018E034
    bl fn_800E4BF4
    li r0, 0x0
    stw r0, 0x8(r27)
    @L_8018E034:
    mr r3, r27
    bl fn_8018FDBC
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
