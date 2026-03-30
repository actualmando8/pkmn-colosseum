    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    la r3, lbl_8047B1E8(r13)
    stw r0, 0x44(r1)
    stmw r20, 0x10(r1)
    li r29, 0x0
    lwz r31, 0x4(r3)
    b @L_80180B74
    @L_8018096C:
    lwz r0, 0x0(r31)
    cmpwi r0, 0x1
    bne @L_80180B6C
    lwz r0, 0x8(r31)
    cmplwi r0, 0x0
    beq @L_8018099C
    lwz r12, 0x8(r31)
    lwz r3, 0x20(r31)
    lwz r4, 0x28(r31)
    mtctr r12
    bctrl
    b @L_80180B80
    @L_8018099C:
    lwz r0, 0x18(r31)
    cmplwi r0, 0x0
    beq @L_80180B6C
    lwz r0, 0xc(r31)
    cmpwi r0, 0x1
    beq @L_80180B80
    lwz r0, 0xc(r31)
    cmpwi r0, 0x2
    bne @L_801809EC
    lwz r0, 0x14(r31)
    cmpwi r0, 0x0
    bne @L_801809E0
    lwz r3, 0x20(r31)
    mr r6, r31
    lwz r4, 0x28(r31)
    lwz r5, 0x24(r31)
    bl fn_8017C1D8
    @L_801809E0:
    li r0, 0x0
    stw r0, 0xc(r31)
    b @L_80180B80
    @L_801809EC:
    lwz r0, lbl_8047B1E0(r13)
    cmplwi r0, 0x0
    beq @L_80180A24
    lwz r3, lbl_8047B1E0(r13)
    bl fn_800E202C
    clrlwi. r0, r3, 16
    mr r28, r3
    beq @L_80180A1C
    mr r3, r28
    bl fn_800E24B0
    mr r3, r28
    bl fn_800E209C
    @L_80180A1C:
    li r0, 0x0
    stw r0, lbl_8047B1E0(r13)
    @L_80180A24:
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x0(r31)
    li r0, 0x0
    stw r3, 0x18(r31)
    stw r0, 0xc(r31)
    lwz r0, 0x1c(r31)
    cmplwi r0, 0x0
    beq @L_80180B60
    lwz r30, 0x1c(r31)
    lwz r0, 0x14(r30)
    cmpwi r0, 0x0
    beq @L_80180A5C
    b @L_80180AC4
    @L_80180A5C:
    stw r30, lbl_8047B1E4(r13)
    lwz r3, lbl_8047B1E4(r13)
    lwz r4, lbl_8047B1E4(r13)
    lwz r5, lbl_8047B1E4(r13)
    lwz r3, 0x20(r3)
    lwz r4, 0x28(r4)
    lwz r5, 0x24(r5)
    lwz r6, lbl_8047B1E4(r13)
    bl fn_8017C074
    bl fn_8017AC30
    lwz r7, 0x20(r30)
    lis r5, fn_8018114C@ha
    addi r6, r5, fn_8018114C@l
    li r4, 0xc8
    lwz r5, 0xf8(r7)
    bl fn_800FE834
    stw r3, 0x18(r30)
    lwz r0, 0x18(r30)
    cmplwi r0, 0x0
    beq @L_80180B80
    li r3, 0x1
    li r0, 0x1
    stw r3, 0x0(r30)
    stw r0, 0xc(r30)
    stw r30, lbl_8047B1E4(r13)
    b @L_80180B80
    @L_80180AC4:
    lis r4, fn_80181224@ha
    li r3, 0x2
    addi r6, r4, fn_80181224@l
    li r5, 0x0
    li r4, 0x1e
    bl fn_800FE834
    stw r3, 0x18(r30)
    lwz r0, 0x18(r30)
    cmplwi r0, 0x0
    beq @L_80180B80
    li r0, 0x1
    lis r3, lbl_80273F80@ha
    stw r0, 0x0(r30)
    li r0, 0x1
    addi r3, r3, lbl_80273F80@l
    stw r0, 0xc(r30)
    bl fn_80167F28
    mr r27, r3
    bl fn_80167E5C
    mr r22, r3
    mr r3, r27
    bl fn_80167E64
    mr r20, r22
    li r4, 0x20
    addi r0, r20, 0x1f
    clrrwi r21, r0, 5
    mr r3, r21
    bl fn_800E2C04
    clrlwi. r0, r3, 16
    mr r25, r3
    beq @L_80180B50
    mr r3, r25
    bl fn_800E27B0
    mr r26, r3
    b @L_80180B54
    @L_80180B50:
    li r26, 0x0
    @L_80180B54:
    stw r26, lbl_8047B1E0(r13)
    stw r30, lbl_8047B1E4(r13)
    b @L_80180B80
    @L_80180B60:
    li r0, 0x0
    stw r0, lbl_8047B1E4(r13)
    b @L_80180B80
    @L_80180B6C:
    addi r31, r31, 0x40
    addi r29, r29, 0x1
    @L_80180B74:
    lwz r0, lbl_8047B1E8(r13)
    cmpw r29, r0
    blt @L_8018096C
    @L_80180B80:
    lmw r20, 0x10(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
