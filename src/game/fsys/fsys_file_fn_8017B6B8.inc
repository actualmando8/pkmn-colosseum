    nofralloc
    stwu r1, -0xf0(r1)
    mflr r0
    stw r0, 0xf4(r1)
    li r0, 0x65
    stmw r17, 0xb4(r1)
    mr r31, r3
    li r3, 0x1
    mr r29, r4
    stw r5, 0x8(r1)
    lwz r30, 0xfc(r31)
    stw r3, 0x0(r30)
    stw r0, 0x48(r31)
    lwz r0, 0xc(r4)
    clrrwi. r0, r0, 31
    beq @L_8017B74C
    lwz r27, 0x14(r29)
    li r4, 0x20
    addi r0, r27, 0x1f
    clrrwi r0, r0, 5
    stw r0, 0x90(r1)
    lwz r3, 0x90(r1)
    bl fn_800E2B00
    sth r3, 0xc(r1)
    lhz r0, 0xc(r1)
    cmplwi r0, 0x0
    beq @L_8017B730
    lhz r3, 0xc(r1)
    bl fn_800E27B0
    stw r3, 0x94(r1)
    b @L_8017B738
    @L_8017B730:
    li r0, 0x0
    stw r0, 0x94(r1)
    @L_8017B738:
    lwz r0, 0x94(r1)
    stw r0, 0x4(r30)
    lwz r0, 0x4(r30)
    stw r0, 0x100(r31)
    b @L_8017B988
    @L_8017B74C:
    lwz r5, 0x20(r29)
    li r0, 0x0
    lwz r3, lbl_80478C48(r13)
    lis r4, lbl_8036C2A0@ha
    stw r5, 0x5c(r1)
    addi r24, r4, lbl_8036C2A0@l
    stw r3, 0x64(r1)
    stw r0, 0x60(r1)
    b @L_8017B798
    @L_8017B770:
    lwz r3, 0x4(r24)
    lwz r0, 0x5c(r1)
    cmplw r3, r0
    bne @L_8017B788
    stw r24, 0x68(r1)
    b @L_8017B7B0
    @L_8017B788:
    lwz r3, 0x60(r1)
    addi r24, r24, 0x10
    addi r0, r3, 0x1
    stw r0, 0x60(r1)
    @L_8017B798:
    lwz r3, 0x60(r1)
    lwz r0, 0x64(r1)
    cmpw r3, r0
    blt @L_8017B770
    li r0, 0x0
    stw r0, 0x68(r1)
    @L_8017B7B0:
    lwz r18, 0x40(r31)
    lwz r3, 0x14(r29)
    lwz r0, 0x10(r18)
    stw r3, 0x78(r1)
    clrlwi. r0, r0, 31
    lwz r20, 0x68(r1)
    beq @L_8017B8F4
    lwz r4, 0x40(r31)
    li r0, 0x0
    lwz r3, 0x1c(r29)
    add r3, r4, r3
    stw r0, 0x6c(r31)
    stw r3, 0x84(r1)
    lwz r3, 0x84(r1)
    bl fn_80167EF8
    clrlwi. r0, r3, 24
    beq @L_8017B800
    lwz r3, 0x84(r1)
    bl fn_80167F28
    stw r3, 0x6c(r31)
    @L_8017B800:
    lwz r0, 0x6c(r31)
    addi r3, r29, 0x28
    stw r3, 0x80(r1)
    cmplwi r0, 0x0
    beq @L_8017B8F4
    lwz r3, 0x6c(r31)
    bl fn_80167E5C
    stw r3, 0x88(r1)
    li r23, 0x0
    li r19, 0x0
    lwz r0, 0x88(r1)
    stw r0, 0x14(r29)
    lwz r0, 0x14(r29)
    stw r0, 0x78(r1)
    b @L_8017B8AC
    @L_8017B83C:
    lwz r21, 0x40(r31)
    stw r19, 0x1c(r1)
    cmplwi r21, 0x0
    beq @L_8017B88C
    stw r21, 0x20(r1)
    lwz r0, 0x1c(r1)
    lwz r3, 0x20(r1)
    slwi r0, r0, 2
    lwz r3, 0x18(r3)
    add r3, r21, r3
    stw r3, 0x24(r1)
    lwz r3, 0x24(r1)
    lwz r3, 0x0(r3)
    add r3, r21, r3
    stw r3, 0x28(r1)
    lwz r3, 0x28(r1)
    lwzx r0, r3, r0
    add r0, r21, r0
    stw r0, 0x2c(r1)
    b @L_8017B894
    @L_8017B88C:
    li r0, 0x0
    stw r0, 0x2c(r1)
    @L_8017B894:
    lwz r0, 0x2c(r1)
    addi r19, r19, 0x1
    stw r0, 0x54(r1)
    lwz r3, 0x54(r1)
    lwz r0, 0x14(r3)
    add r23, r23, r0
    @L_8017B8AC:
    lwz r0, 0xc(r31)
    cmplw r19, r0
    blt @L_8017B83C
    lwz r3, 0x40(r31)
    lwz r0, 0x18(r31)
    add r0, r3, r0
    stw r0, 0x58(r1)
    lwz r3, 0x58(r1)
    lwz r0, 0x8(r3)
    add r23, r23, r0
    stw r23, 0x20(r31)
    lwz r3, 0x6c(r31)
    bl fn_80167E64
    li r0, 0x0
    lwz r4, 0x80(r1)
    stw r0, 0x6c(r31)
    li r0, 0x1
    stw r0, 0x8(r4)
    @L_8017B8F4:
    lwz r0, 0x8(r20)
    cmplwi r0, 0x0
    beq @L_8017B928
    lwz r0, 0x8(r20)
    lwz r3, 0xf8(r31)
    stw r0, 0x7c(r1)
    lwz r4, 0x0(r29)
    lwz r12, 0x7c(r1)
    lwz r5, 0x78(r1)
    mtctr r12
    bctrl
    mr r17, r3
    b @L_8017B97C
    @L_8017B928:
    lwz r3, 0x78(r1)
    li r4, 0x20
    lwz r5, 0x0(r29)
    li r7, 0x0
    addi r0, r3, 0x1f
    lwz r3, 0xf8(r31)
    clrrwi r0, r0, 5
    stw r5, 0x44(r1)
    stw r3, 0x48(r1)
    lwz r6, 0x44(r1)
    stw r0, 0x4c(r1)
    lwz r5, 0x48(r1)
    stw r0, 0x18(r1)
    lwz r3, 0x4c(r1)
    bl fn_800F9418
    stw r3, 0x50(r1)
    lwz r0, 0x50(r1)
    stw r0, 0x14(r1)
    lwz r0, 0x14(r1)
    stw r0, 0x10(r1)
    lwz r17, 0x10(r1)
    @L_8017B97C:
    stw r17, 0x8c(r1)
    lwz r0, 0x8c(r1)
    stw r0, 0x4(r30)
    @L_8017B988:
    lwz r0, 0x4(r30)
    cmplwi r0, 0x0
    bne @L_8017B9A8
    li r3, 0x64
    li r0, 0x7
    stw r3, 0x48(r31)
    stw r0, 0x0(r30)
    b @L_8017BB6C
    @L_8017B9A8:
    lwz r26, 0x40(r31)
    lwz r0, 0x8(r1)
    lwz r4, 0x40(r31)
    lwz r3, 0x18(r26)
    slwi r0, r0, 2
    lwz r27, 0x14(r29)
    add r3, r4, r3
    lwz r6, 0x40(r31)
    stw r3, 0xa0(r1)
    addi r3, r27, 0x1f
    lwz r5, 0x40(r31)
    clrrwi r27, r3, 5
    lwz r4, 0xa0(r1)
    lwz r3, 0x8(r26)
    lwz r4, 0x0(r4)
    add r4, r6, r4
    stw r4, 0xa4(r1)
    lwz r4, 0xa4(r1)
    lwzx r0, r4, r0
    add r28, r5, r0
    lwz r4, 0x20(r28)
    lwz r5, 0x0(r28)
    bl fn_8017F794
    stw r3, 0x9c(r1)
    lwz r0, 0x9c(r1)
    cmplwi r0, 0x0
    beq @L_8017BAC4
    lwz r22, 0x40(r31)
    lwz r0, 0xf4(r31)
    cmplwi r22, 0x0
    stw r0, 0x30(r1)
    beq @L_8017BA68
    stw r22, 0x34(r1)
    lwz r0, 0x30(r1)
    lwz r3, 0x34(r1)
    slwi r0, r0, 2
    lwz r3, 0x18(r3)
    add r3, r22, r3
    stw r3, 0x38(r1)
    lwz r3, 0x38(r1)
    lwz r3, 0x0(r3)
    add r3, r22, r3
    stw r3, 0x3c(r1)
    lwz r3, 0x3c(r1)
    lwzx r0, r3, r0
    add r0, r22, r0
    stw r0, 0x40(r1)
    b @L_8017BA70
    @L_8017BA68:
    li r0, 0x0
    stw r0, 0x40(r1)
    @L_8017BA70:
    lwz r25, 0x40(r1)
    lwz r3, 0xfc(r31)
    lwz r0, 0x14(r25)
    stw r3, 0x74(r1)
    lwz r3, 0xf8(r31)
    stw r0, 0x70(r1)
    lwz r4, 0x20(r25)
    lwz r5, 0x0(r25)
    bl fn_8017F794
    li r0, 0xa1
    stw r3, 0x6c(r1)
    lwz r5, 0x74(r1)
    lis r3, fn_8017F25C@ha
    stw r0, 0x48(r31)
    addi r6, r3, fn_8017F25C@l
    lwz r4, 0x6c(r1)
    mr r7, r31
    lwz r3, 0x4(r5)
    lwz r5, 0x70(r1)
    bl fn_80180584
    b @L_8017BB6C
    @L_8017BAC4:
    lwz r0, 0x10(r26)
    clrlwi. r0, r0, 31
    beq @L_8017BB50
    lwz r4, 0x40(r31)
    li r0, 0x0
    lwz r3, 0x1c(r28)
    add r3, r4, r3
    stw r0, 0x6c(r31)
    stw r3, 0x98(r1)
    lwz r3, 0x98(r1)
    bl fn_80167EF8
    clrlwi. r0, r3, 24
    beq @L_8017BB04
    lwz r3, 0x98(r1)
    bl fn_80167F28
    stw r3, 0x6c(r31)
    @L_8017BB04:
    lwz r0, 0x6c(r31)
    cmplwi r0, 0x0
    bne @L_8017BB30
    lis r4, fn_8017F108@ha
    lwz r3, 0x68(r31)
    addi r7, r4, fn_8017F108@l
    lwz r4, 0x4(r30)
    lwz r6, 0x4(r28)
    mr r5, r27
    bl fn_80167E98
    b @L_8017BB6C
    @L_8017BB30:
    lis r4, fn_8017F108@ha
    lwz r3, 0x6c(r31)
    addi r7, r4, fn_8017F108@l
    lwz r4, 0x4(r30)
    mr r5, r27
    li r6, 0x0
    bl fn_80167E98
    b @L_8017BB6C
    @L_8017BB50:
    lis r4, fn_8017F108@ha
    lwz r3, 0x68(r31)
    addi r7, r4, fn_8017F108@l
    lwz r4, 0x4(r30)
    lwz r6, 0x4(r28)
    mr r5, r27
    bl fn_80167E98
    @L_8017BB6C:
    lmw r17, 0xb4(r1)
    lwz r0, 0xf4(r1)
    mtlr r0
    addi r1, r1, 0xf0
    blr
