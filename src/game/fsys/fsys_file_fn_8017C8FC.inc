    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    li r0, 0x0
    stmw r17, 0x64(r1)
    mr r31, r3
    stw r0, 0xfc(r3)
    lwz r0, 0x4c(r3)
    cmpwi r0, 0x3
    bne @L_8017CAA4
    li r28, 0x0
    b @L_8017CA94
    @L_8017C92C:
    lwz r25, 0x40(r31)
    cmplwi r25, 0x0
    beq @L_8017C974
    stw r25, 0x48(r1)
    slwi r0, r28, 2
    lwz r3, 0x48(r1)
    lwz r3, 0x18(r3)
    add r3, r25, r3
    stw r3, 0x4c(r1)
    lwz r3, 0x4c(r1)
    lwz r3, 0x0(r3)
    add r3, r25, r3
    stw r3, 0x50(r1)
    lwz r3, 0x50(r1)
    lwzx r0, r3, r0
    add r0, r25, r0
    stw r0, 0x54(r1)
    b @L_8017C97C
    @L_8017C974:
    li r0, 0x0
    stw r0, 0x54(r1)
    @L_8017C97C:
    lwz r30, 0x54(r1)
    lwz r0, 0x104(r31)
    lwz r3, 0x0(r30)
    cmplw r3, r0
    bne @L_8017CA90
    stw r28, 0xf4(r31)
    addi r29, r30, 0x28
    lwz r3, 0xf8(r31)
    lwz r4, 0x20(r30)
    lwz r5, 0x0(r30)
    bl fn_8017F794
    lwz r0, 0x0(r29)
    mr r21, r3
    cmpwi r0, 0x0
    bne @L_8017C9F0
    cmplwi r21, 0x0
    bne @L_8017C9D8
    stw r29, 0xfc(r31)
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_8017B6B8
    b @L_8017CE48
    @L_8017C9D8:
    stw r29, 0xfc(r31)
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_8017B6B8
    b @L_8017CE48
    @L_8017C9F0:
    lwz r0, 0x0(r29)
    cmpwi r0, 0x4
    bne @L_8017CA40
    lwz r0, 0x50(r31)
    cmpwi r0, 0x1
    bne @L_8017CA40
    cmplwi r21, 0x0
    bne @L_8017CA28
    stw r29, 0xfc(r31)
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_8017B6B8
    b @L_8017CE48
    @L_8017CA28:
    stw r29, 0xfc(r31)
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_8017B5C0
    b @L_8017CE48
    @L_8017CA40:
    lwz r0, 0x0(r29)
    cmpwi r0, 0x6
    bne @L_8017CE48
    lwz r0, 0x50(r31)
    cmpwi r0, 0x1
    bne @L_8017CE48
    cmplwi r21, 0x0
    bne @L_8017CA78
    stw r29, 0xfc(r31)
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_8017B6B8
    b @L_8017CE48
    @L_8017CA78:
    stw r29, 0xfc(r31)
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_8017B5C0
    b @L_8017CE48
    @L_8017CA90:
    addi r28, r28, 0x1
    @L_8017CA94:
    lwz r0, 0xc(r31)
    cmplw r28, r0
    blt @L_8017C92C
    b @L_8017CE48
    @L_8017CAA4:
    li r28, 0x0
    b @L_8017CE3C
    @L_8017CAAC:
    lwz r24, 0x40(r31)
    cmplwi r24, 0x0
    beq @L_8017CAF4
    stw r24, 0x38(r1)
    slwi r0, r28, 2
    lwz r3, 0x38(r1)
    lwz r3, 0x18(r3)
    add r3, r24, r3
    stw r3, 0x3c(r1)
    lwz r3, 0x3c(r1)
    lwz r3, 0x0(r3)
    add r3, r24, r3
    stw r3, 0x40(r1)
    lwz r3, 0x40(r1)
    lwzx r0, r3, r0
    add r0, r24, r0
    stw r0, 0x44(r1)
    b @L_8017CAFC
    @L_8017CAF4:
    li r0, 0x0
    stw r0, 0x44(r1)
    @L_8017CAFC:
    stw r28, 0xf4(r31)
    lwz r30, 0x44(r1)
    lwz r3, 0xf8(r31)
    lwz r4, 0x20(r30)
    addi r29, r30, 0x28
    lwz r5, 0x0(r30)
    bl fn_8017F794
    lwz r0, 0x0(r29)
    stw r3, 0x58(r1)
    cmpwi r0, 0x0
    bne @L_8017CD94
    lwz r4, 0x40(r31)
    slwi r0, r28, 2
    lwz r3, 0x18(r31)
    lwz r5, 0x40(r31)
    add r20, r4, r3
    lwz r3, 0x40(r31)
    lwz r4, 0x0(r20)
    add r19, r5, r4
    lwzx r0, r19, r0
    add r30, r3, r0
    lwz r0, 0x14(r30)
    cmplwi r0, 0x0
    bne @L_8017CB9C
    lwz r0, 0x4c(r31)
    cmpwi r0, 0x2
    beq @L_8017CB90
    bge @L_8017CB78
    cmpwi r0, 0x0
    beq @L_8017CB84
    b @L_8017CE38
    @L_8017CB78:
    cmpwi r0, 0x7
    beq @L_8017CB90
    b @L_8017CE38
    @L_8017CB84:
    li r0, 0x6
    stw r0, 0x0(r29)
    b @L_8017CE38
    @L_8017CB90:
    li r0, 0x6
    stw r0, 0x0(r29)
    b @L_8017CE38
    @L_8017CB9C:
    lwz r0, 0x4c(r31)
    cmpwi r0, 0x2
    beq @L_8017CBDC
    bge @L_8017CBB8
    cmpwi r0, 0x0
    beq @L_8017CBC4
    b @L_8017CE48
    @L_8017CBB8:
    cmpwi r0, 0x7
    beq @L_8017CBDC
    b @L_8017CE48
    @L_8017CBC4:
    stw r29, 0xfc(r31)
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_8017B6B8
    b @L_8017CE48
    @L_8017CBDC:
    stw r29, 0xfc(r31)
    lwz r0, 0xc(r30)
    clrrwi. r0, r0, 31
    beq @L_8017CCC0
    lwz r3, 0x40(r31)
    slwi r0, r28, 2
    lwz r4, 0x14(r30)
    stw r3, 0x24(r1)
    addi r5, r4, 0x1f
    lwz r4, 0x40(r31)
    lwz r3, 0x24(r1)
    clrrwi r7, r5, 5
    lwz r6, 0x40(r31)
    lwz r3, 0x18(r3)
    lwz r5, 0x40(r31)
    add r3, r4, r3
    stw r7, 0x2c(r1)
    lwz r23, 0xfc(r31)
    stw r3, 0x28(r1)
    lwz r3, 0xf8(r31)
    lwz r4, 0x28(r1)
    lwz r4, 0x0(r4)
    add r4, r6, r4
    stw r4, 0x20(r1)
    lwz r4, 0x20(r1)
    lwzx r0, r4, r0
    add r27, r5, r0
    lwz r4, 0x20(r27)
    lwz r5, 0x0(r27)
    bl fn_8017F794
    stw r3, 0x34(r1)
    lwz r0, 0x34(r1)
    cmplwi r0, 0x0
    beq @L_8017CC78
    li r3, 0x4
    li r0, 0x64
    stw r3, 0x0(r23)
    stw r0, 0x48(r31)
    b @L_8017CE48
    @L_8017CC78:
    li r3, 0x3
    li r0, 0x12d
    stw r3, 0x0(r23)
    mr r3, r31
    mr r7, r31
    mr r10, r27
    stw r0, 0x48(r31)
    li r6, 0x0
    li r8, 0x0
    lwz r5, 0x40(r31)
    lwz r0, 0x1c(r27)
    lwz r4, 0x4(r27)
    add r0, r5, r0
    lwz r5, 0x14(r30)
    stw r0, 0x30(r1)
    lwz r9, 0x30(r1)
    bl fn_80179FA4
    b @L_8017CE48
    @L_8017CCC0:
    lwz r3, 0x40(r31)
    slwi r0, r28, 2
    lwz r4, 0x14(r30)
    stw r3, 0xc(r1)
    addi r5, r4, 0x1f
    lwz r4, 0x40(r31)
    lwz r3, 0xc(r1)
    clrrwi r7, r5, 5
    lwz r6, 0x40(r31)
    lwz r3, 0x18(r3)
    lwz r5, 0x40(r31)
    add r3, r4, r3
    stw r7, 0x14(r1)
    lwz r22, 0xfc(r31)
    stw r3, 0x10(r1)
    lwz r3, 0xf8(r31)
    lwz r4, 0x10(r1)
    lwz r4, 0x0(r4)
    add r4, r6, r4
    stw r4, 0x8(r1)
    lwz r4, 0x8(r1)
    lwzx r0, r4, r0
    add r26, r5, r0
    lwz r4, 0x20(r26)
    lwz r5, 0x0(r26)
    bl fn_8017F794
    stw r3, 0x1c(r1)
    lwz r0, 0x1c(r1)
    cmplwi r0, 0x0
    beq @L_8017CD4C
    li r3, 0x4
    li r0, 0x64
    stw r3, 0x0(r22)
    stw r0, 0x48(r31)
    b @L_8017CE48
    @L_8017CD4C:
    li r3, 0x3
    li r0, 0x12d
    stw r3, 0x0(r22)
    mr r3, r31
    mr r7, r31
    mr r10, r26
    stw r0, 0x48(r31)
    li r6, 0x0
    li r8, 0x0
    lwz r5, 0x40(r31)
    lwz r0, 0x1c(r26)
    lwz r4, 0x4(r26)
    add r0, r5, r0
    lwz r5, 0x14(r30)
    stw r0, 0x18(r1)
    lwz r9, 0x18(r1)
    bl fn_80179FA4
    b @L_8017CE48
    @L_8017CD94:
    lwz r0, 0x0(r29)
    cmpwi r0, 0x4
    bne @L_8017CE38
    lwz r0, 0x50(r31)
    cmpwi r0, 0x1
    bne @L_8017CE38
    lwz r5, 0x40(r31)
    slwi r3, r28, 2
    lwz r4, 0x18(r31)
    lwz r0, 0x4c(r31)
    add r18, r5, r4
    lwz r6, 0x40(r31)
    lwz r5, 0x0(r18)
    cmpwi r0, 0x2
    lwz r4, 0x40(r31)
    add r17, r6, r5
    lwzx r3, r17, r3
    add r30, r4, r3
    beq @L_8017CE48
    bge @L_8017CDF0
    cmpwi r0, 0x0
    beq @L_8017CDFC
    b @L_8017CE48
    @L_8017CDF0:
    cmpwi r0, 0x7
    beq @L_8017CE48
    b @L_8017CE48
    @L_8017CDFC:
    lwz r0, 0x58(r1)
    cmplwi r0, 0x0
    bne @L_8017CE20
    stw r29, 0xfc(r31)
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_8017B6B8
    b @L_8017CE48
    @L_8017CE20:
    stw r29, 0xfc(r31)
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_8017B5C0
    b @L_8017CE48
    @L_8017CE38:
    addi r28, r28, 0x1
    @L_8017CE3C:
    lwz r0, 0xc(r31)
    cmplw r28, r0
    blt @L_8017CAAC
    @L_8017CE48:
    lwz r0, 0xfc(r31)
    cmplwi r0, 0x0
    bne @L_8017CE64
    li r0, 0x7d0
    li r3, 0x1
    stw r0, 0x48(r31)
    b @L_8017CE68
    @L_8017CE64:
    li r3, 0x0
    @L_8017CE68:
    lmw r17, 0x64(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
