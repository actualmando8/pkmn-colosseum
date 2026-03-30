    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r3
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    b @L_801A4F2C
    @L_801A4D44:
    cmplwi r31, 0x0
    beq @L_801A4F28
    lwz r3, lbl_8047B2B4(r13)
    b @L_801A4E6C
    @L_801A4D54:
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_801A4E68
    cmplwi r31, 0x0
    beq @L_801A4E74
    la r30, lbl_8047B2B4(r13)
    b @L_801A4E58
    @L_801A4D70:
    lwz r3, 0x0(r30)
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_801A4E54
    li r6, 0x0
    b @L_801A4DC0
    @L_801A4D88:
    lis r3, lbl_804655E0@ha
    slwi r4, r6, 2
    addi r0, r3, lbl_804655E0@l
    add r3, r0, r4
    lwz r0, 0x0(r3)
    cmplw r31, r0
    bne @L_801A4DBC
    lis r3, lbl_804655E0@ha
    slwi r4, r6, 2
    addi r0, r3, lbl_804655E0@l
    li r5, 0x0
    add r3, r0, r4
    stw r5, 0x0(r3)
    @L_801A4DBC:
    addi r6, r6, 0x1
    @L_801A4DC0:
    cmpwi r6, 0x9
    blt @L_801A4D88
    lwz r3, 0x0(r30)
    bl fn_801A3E64
    cmplwi r31, 0x0
    stw r3, 0x0(r30)
    beq @L_801A4E74
    lis r3, 0x1
    lhz r4, 0x4(r31)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_801A4E00
    b @L_801A4E18
    @L_801A4E00:
    lhz r0, 0x4(r31)
    lhz r3, 0x4(r31)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r31)
    srwi r3, r4, 5
    @L_801A4E18:
    cmpwi r3, 0x0
    beq @L_801A4E74
    cmplwi r31, 0x0
    beq @L_801A4E74
    lwz r4, 0x0(r31)
    mr r3, r31
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r31)
    mr r3, r31
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    b @L_801A4E74
    @L_801A4E54:
    lwz r30, 0x0(r30)
    @L_801A4E58:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    bne @L_801A4D70
    b @L_801A4E74
    @L_801A4E68:
    lwz r3, 0x0(r3)
    @L_801A4E6C:
    cmplwi r3, 0x0
    bne @L_801A4D54
    @L_801A4E74:
    cmplwi r31, 0x0
    beq @L_801A4EAC
    lhz r3, 0x4(r31)
    addi r0, r3, 0x1
    sth r0, 0x4(r31)
    lhz r0, 0x4(r31)
    cmplwi r0, 0xffff
    bne @L_801A4EAC
    lis r3, lbl_80274DD8@ha
    lis r5, lbl_80274DE4@ha
    addi r3, r3, lbl_80274DD8@l
    li r4, 0x5d
    addi r5, r5, lbl_80274DE4@l
    bl fn_80196E10
    @L_801A4EAC:
    la r30, lbl_8047B2B4(r13)
    b @L_801A4F0C
    @L_801A4EB4:
    cmplwi r31, 0x0
    bne @L_801A4ECC
    la r3, lbl_8047DBD8(r2)
    li r4, 0x163
    la r5, lbl_8047DBCC(r2)
    bl fn_80196E10
    @L_801A4ECC:
    lwz r3, 0x0(r30)
    lhz r0, 0xa(r31)
    lwz r29, 0x4(r3)
    clrlwi r28, r0, 24
    cmplwi r29, 0x0
    bne @L_801A4EF4
    la r3, lbl_8047DBD8(r2)
    li r4, 0x163
    la r5, lbl_8047DBCC(r2)
    bl fn_80196E10
    @L_801A4EF4:
    lhz r3, 0xa(r29)
    clrlwi r0, r28, 24
    clrlwi r3, r3, 24
    cmplw r3, r0
    bgt @L_801A4F18
    lwz r30, 0x0(r30)
    @L_801A4F0C:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    bne @L_801A4EB4
    @L_801A4F18:
    lwz r3, 0x0(r30)
    mr r4, r31
    bl fn_801A3EB4
    stw r3, 0x0(r30)
    @L_801A4F28:
    lwz r31, 0xc(r31)
    @L_801A4F2C:
    cmplwi r31, 0x0
    bne @L_801A4D44
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
