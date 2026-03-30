    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r8, 0x0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    mr r31, r3
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    li r29, 0x0
    stw r4, 0x8(r1)
    stw r5, 0xc(r1)
    lwz r4, 0x8(r1)
    stw r6, 0x10(r1)
    lwz r5, 0xc(r1)
    stw r7, 0x14(r1)
    lwz r6, 0x10(r1)
    stw r28, 0x20(r1)
    lwz r7, 0x14(r1)
    bl fn_8017EB6C
    lis r3, lbl_80453FEC@ha
    addi r3, r3, lbl_80453FEC@l
    lwz r0, 0x1c(r3)
    cmplw r0, r31
    bne @L_8017E1B8
    lwz r0, 0x50(r31)
    cmpwi r0, 0x1
    bne @L_8017E124
    lwz r0, 0x68(r31)
    cmplwi r0, 0x0
    bne @L_8017E1B8
    addi r3, r31, 0x70
    bl fn_80167F28
    stw r3, 0x68(r31)
    b @L_8017E1B8
    @L_8017E124:
    lwz r0, 0x68(r31)
    cmplwi r0, 0x0
    bne @L_8017E13C
    addi r3, r31, 0x70
    bl fn_80167F28
    stw r3, 0x68(r31)
    @L_8017E13C:
    lwz r3, 0xf8(r31)
    li r30, 0x40
    li r4, 0x0
    li r5, 0x0
    bl fn_8017F794
    mr. r28, r3
    beq @L_8017E188
    addi r0, r30, 0x1f
    mr r3, r31
    mr r4, r28
    clrrwi r5, r0, 5
    bl fn_80180320
    addi r0, r30, 0x1f
    mr r3, r31
    clrrwi r4, r0, 5
    bl DCFlushRange
    li r0, 0x2
    li r29, 0x1
    stw r0, 0x48(r31)
    @L_8017E188:
    lwz r0, 0x68(r31)
    cmplwi r0, 0x0
    clrlwi. r0, r29, 24
    bne @L_8017E1B8
    lis r3, fn_8017F108@ha
    addi r0, r30, 0x1f
    addi r7, r3, fn_8017F108@l
    lwz r3, 0x68(r31)
    mr r4, r31
    clrrwi r5, r0, 5
    li r6, 0x0
    bl fn_80167E98
    @L_8017E1B8:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
