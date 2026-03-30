    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    li r4, 0x40
    stw r0, 0x84(r1)
    stmw r17, 0x44(r1)
    mr r31, r3
    li r21, 0x0
    bl DCFlushRange
    lwz r0, 0x4c(r31)
    lwz r3, 0xc(r31)
    cmpwi r0, 0x7
    lwz r28, 0x1c(r31)
    stw r3, 0x2c(r1)
    bne @L_8017DBB8
    lwz r0, 0x10(r31)
    oris r0, r0, 0x4000
    stw r0, 0x10(r31)
    @L_8017DBB8:
    lwz r3, 0xf8(r31)
    li r26, 0x40
    li r4, 0x0
    li r5, 0x0
    bl fn_8017F794
    mr. r25, r3
    bne @L_8017DDB0
    bl fn_8017FA5C
    addi r0, r26, 0x1f
    mr r23, r3
    clrrwi r0, r0, 5
    cmplw r0, r23
    ble @L_8017DD7C
    lwz r27, lbl_8047B1B8(r13)
    li r22, 0x0
    b @L_8017DC1C
    @L_8017DBF8:
    lwz r3, 0x0(r27)
    lwz r0, 0x8(r31)
    cmpw r3, r0
    bne @L_8017DC14
    lwz r0, 0x0(r27)
    stw r0, 0x24(r1)
    b @L_8017DC30
    @L_8017DC14:
    addi r27, r27, 0x8
    addi r22, r22, 0x1
    @L_8017DC1C:
    lwz r0, lbl_8047B1BC(r13)
    cmpw r22, r0
    blt @L_8017DBF8
    li r0, -0x1
    stw r0, 0x24(r1)
    @L_8017DC30:
    lwz r0, 0x24(r1)
    cmpwi r0, 0x0
    bge @L_8017DD7C
    li r19, 0x0
    @L_8017DC40:
    lwz r0, lbl_8047B1B8(r13)
    stw r0, 0x1c(r1)
    lwz r3, 0x1c(r1)
    lwz r0, 0x0(r3)
    stw r0, 0x18(r1)
    lwz r0, 0x18(r1)
    stw r0, 0x20(r1)
    lwz r24, 0x20(r1)
    cmpwi r24, 0x0
    blt @L_8017DD7C
    mr r3, r24
    bl fn_8017F800
    li r0, 0x0
    stw r24, 0x8(r1)
    lwz r30, lbl_8047B1B8(r13)
    li r29, 0x0
    stw r0, 0xc(r1)
    b @L_8017DCB8
    @L_8017DC88:
    lwz r3, 0x0(r30)
    lwz r0, 0x8(r1)
    cmpw r3, r0
    bne @L_8017DCB0
    li r3, -0x1
    li r0, 0x1
    stw r3, 0x0(r30)
    stw r0, 0xc(r1)
    stw r29, 0x10(r1)
    b @L_8017DCC4
    @L_8017DCB0:
    addi r30, r30, 0x8
    addi r29, r29, 0x1
    @L_8017DCB8:
    lwz r0, lbl_8047B1BC(r13)
    cmpw r29, r0
    blt @L_8017DC88
    @L_8017DCC4:
    lwz r0, 0xc(r1)
    cmpwi r0, 0x0
    beq @L_8017DD48
    lwz r30, lbl_8047B1B8(r13)
    li r29, 0x0
    b @L_8017DD10
    @L_8017DCDC:
    lwz r0, 0x10(r1)
    cmpw r29, r0
    blt @L_8017DD0C
    addi r3, r29, 0x1
    slwi r0, r29, 3
    slwi r3, r3, 3
    add r4, r30, r3
    add r5, r30, r0
    lwz r3, 0x0(r4)
    lwz r0, 0x4(r4)
    stw r3, 0x0(r5)
    stw r0, 0x4(r5)
    @L_8017DD0C:
    addi r29, r29, 0x1
    @L_8017DD10:
    lwz r3, lbl_8047B1BC(r13)
    subi r0, r3, 0x1
    cmpw r29, r0
    blt @L_8017DCDC
    lwz r4, lbl_8047B1BC(r13)
    li r0, 0x0
    li r3, -0x1
    stw r0, 0x14(r1)
    subi r0, r4, 0x1
    stw r0, lbl_8047B1BC(r13)
    lwz r0, lbl_8047B1BC(r13)
    slwi r0, r0, 3
    stwx r3, r30, r0
    b @L_8017DD50
    @L_8017DD48:
    li r0, -0x1
    stw r0, 0x14(r1)
    @L_8017DD50:
    lwz r0, 0x14(r1)
    cmpwi r0, 0x0
    blt @L_8017DD7C
    bl fn_8017FA5C
    addi r0, r26, 0x1f
    mr r23, r3
    clrrwi r0, r0, 5
    cmplw r0, r23
    ble @L_8017DD7C
    addi r19, r19, 0x1
    b @L_8017DC40
    @L_8017DD7C:
    addi r0, r26, 0x1f
    lwz r4, 0xf8(r31)
    clrrwi r3, r0, 5
    li r5, 0x0
    li r6, 0x0
    bl fn_8017F928
    mr. r25, r3
    beq @L_8017DDB0
    addi r0, r26, 0x1f
    mr r3, r31
    mr r4, r25
    clrrwi r5, r0, 5
    bl fn_80180450
    @L_8017DDB0:
    addi r0, r28, 0x1f
    li r4, 0x20
    clrrwi r3, r0, 5
    addi r0, r3, 0x1f
    clrrwi r0, r0, 5
    stw r0, 0x28(r1)
    lwz r3, 0x28(r1)
    bl fn_800E2C04
    clrlwi. r0, r3, 16
    mr r17, r3
    beq @L_8017DDEC
    mr r3, r17
    bl fn_800E27B0
    mr r18, r3
    b @L_8017DDF0
    @L_8017DDEC:
    li r18, 0x0
    @L_8017DDF0:
    stw r18, 0x40(r31)
    lwz r0, 0x40(r31)
    cmplwi r0, 0x0
    lwz r3, 0xf8(r31)
    li r4, 0x0
    li r5, 0x1
    bl fn_8017F794
    mr. r20, r3
    beq @L_8017DE44
    addi r0, r28, 0x1f
    lwz r3, 0x40(r31)
    mr r4, r20
    clrrwi r5, r0, 5
    bl fn_80180320
    addi r0, r28, 0x1f
    lwz r3, 0x40(r31)
    clrrwi r4, r0, 5
    bl DCFlushRange
    li r0, 0x4
    li r21, 0x1
    stw r0, 0x48(r31)
    @L_8017DE44:
    clrlwi. r0, r21, 24
    bne @L_8017DE90
    li r0, 0x3
    mr r4, r31
    stw r0, 0x48(r31)
    li r5, 0x40
    lwz r3, 0x40(r31)
    bl memcpy
    li r0, 0x40
    lis r4, fn_8017F108@ha
    stw r0, 0x30(r1)
    subi r0, r28, 0x21
    lwz r5, 0x40(r31)
    addi r7, r4, fn_8017F108@l
    lwz r3, 0x68(r31)
    addi r4, r5, 0x40
    lwz r6, 0x30(r1)
    clrrwi r5, r0, 5
    bl fn_80167E98
    @L_8017DE90:
    lmw r17, 0x44(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
