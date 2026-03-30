    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    li r5, 0x5
    stw r0, 0xb4(r1)
    li r0, 0x65
    stmw r17, 0x74(r1)
    mr r31, r3
    lis r3, lbl_8036C2A0@ha
    mr r30, r4
    addi r29, r3, lbl_8036C2A0@l
    li r18, 0x0
    li r23, 0x0
    li r21, 0x0
    lwz r20, 0xfc(r31)
    stw r5, 0x0(r20)
    stw r0, 0x48(r31)
    lwz r3, 0x20(r4)
    lwz r0, lbl_80478C48(r13)
    stw r3, 0x40(r1)
    stw r0, 0x44(r1)
    b @L_8017BDA8
    @L_8017BD88:
    lwz r3, 0x4(r29)
    lwz r0, 0x40(r1)
    cmplw r3, r0
    bne @L_8017BDA0
    stw r29, 0x48(r1)
    b @L_8017BDBC
    @L_8017BDA0:
    addi r29, r29, 0x10
    addi r21, r21, 0x1
    @L_8017BDA8:
    lwz r0, 0x44(r1)
    cmpw r21, r0
    blt @L_8017BD88
    li r0, 0x0
    stw r0, 0x48(r1)
    @L_8017BDBC:
    lwz r24, 0x40(r31)
    lwz r26, 0x48(r1)
    lwz r0, 0x10(r24)
    lwz r22, 0x14(r30)
    clrlwi. r0, r0, 31
    beq @L_8017BEF4
    lwz r4, 0x40(r31)
    li r0, 0x0
    lwz r3, 0x1c(r30)
    add r17, r4, r3
    stw r0, 0x6c(r31)
    mr r3, r17
    bl fn_80167EF8
    clrlwi. r0, r3, 24
    beq @L_8017BE04
    mr r3, r17
    bl fn_80167F28
    stw r3, 0x6c(r31)
    @L_8017BE04:
    lwz r0, 0x6c(r31)
    addi r3, r30, 0x28
    stw r3, 0x50(r1)
    cmplwi r0, 0x0
    beq @L_8017BEF4
    lwz r3, 0x6c(r31)
    bl fn_80167E5C
    stw r3, 0x54(r1)
    li r28, 0x0
    li r25, 0x0
    lwz r0, 0x54(r1)
    stw r0, 0x14(r30)
    lwz r22, 0x14(r30)
    b @L_8017BEAC
    @L_8017BE3C:
    lwz r27, 0x40(r31)
    stw r25, 0x14(r1)
    cmplwi r27, 0x0
    beq @L_8017BE8C
    stw r27, 0x18(r1)
    lwz r0, 0x14(r1)
    lwz r3, 0x18(r1)
    slwi r0, r0, 2
    lwz r3, 0x18(r3)
    add r3, r27, r3
    stw r3, 0x1c(r1)
    lwz r3, 0x1c(r1)
    lwz r3, 0x0(r3)
    add r3, r27, r3
    stw r3, 0x20(r1)
    lwz r3, 0x20(r1)
    lwzx r0, r3, r0
    add r0, r27, r0
    stw r0, 0x24(r1)
    b @L_8017BE94
    @L_8017BE8C:
    li r0, 0x0
    stw r0, 0x24(r1)
    @L_8017BE94:
    lwz r0, 0x24(r1)
    addi r25, r25, 0x1
    stw r0, 0x38(r1)
    lwz r3, 0x38(r1)
    lwz r0, 0x14(r3)
    add r28, r28, r0
    @L_8017BEAC:
    lwz r0, 0xc(r31)
    cmplw r25, r0
    blt @L_8017BE3C
    lwz r3, 0x40(r31)
    lwz r0, 0x18(r31)
    add r0, r3, r0
    stw r0, 0x3c(r1)
    lwz r3, 0x3c(r1)
    lwz r0, 0x8(r3)
    add r28, r28, r0
    stw r28, 0x20(r31)
    lwz r3, 0x6c(r31)
    bl fn_80167E64
    li r0, 0x0
    lwz r4, 0x50(r1)
    stw r0, 0x6c(r31)
    li r0, 0x1
    stw r0, 0x8(r4)
    @L_8017BEF4:
    lwz r0, 0x8(r26)
    cmplwi r0, 0x0
    beq @L_8017BF28
    lwz r0, 0x8(r26)
    mr r5, r22
    lwz r3, 0xf8(r31)
    stw r0, 0x4c(r1)
    lwz r4, 0x0(r30)
    lwz r12, 0x4c(r1)
    mtctr r12
    bctrl
    mr r23, r3
    b @L_8017BF78
    @L_8017BF28:
    addi r0, r22, 0x1f
    lwz r4, 0x0(r30)
    clrrwi r0, r0, 5
    lwz r3, 0xf8(r31)
    stw r4, 0x28(r1)
    li r4, 0x20
    li r7, 0x0
    stw r3, 0x2c(r1)
    lwz r6, 0x28(r1)
    stw r0, 0x30(r1)
    lwz r5, 0x2c(r1)
    stw r0, 0x10(r1)
    lwz r3, 0x30(r1)
    bl fn_800F9418
    stw r3, 0x34(r1)
    lwz r0, 0x34(r1)
    stw r0, 0xc(r1)
    lwz r0, 0xc(r1)
    stw r0, 0x8(r1)
    lwz r23, 0x8(r1)
    @L_8017BF78:
    lwz r3, 0x14(r30)
    stw r23, 0x58(r1)
    addi r0, r3, 0x1f
    lwz r3, 0xf8(r31)
    clrrwi r0, r0, 5
    lwz r18, 0x58(r1)
    stw r0, 0x60(r1)
    lwz r4, 0x20(r30)
    lwz r5, 0x0(r30)
    bl fn_8017F794
    mr r19, r3
    lwz r3, 0xf8(r31)
    lwz r4, 0x20(r30)
    lwz r5, 0x0(r30)
    bl fn_8017F728
    stw r3, 0x5c(r1)
    lis r4, fn_8017A95C@ha
    addi r6, r4, fn_8017A95C@l
    mr r3, r18
    lwz r5, 0x5c(r1)
    mr r4, r19
    mr r7, r31
    bl fn_80180584
    lmw r17, 0x74(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
