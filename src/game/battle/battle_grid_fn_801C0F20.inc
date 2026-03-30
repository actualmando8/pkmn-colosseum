    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr. r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    bne @L_801C0F58
    la r3, lbl_8047DF40(r2)
    li r4, 0x2cb
    la r5, lbl_8047DF48(r2)
    bl fn_80196E10
    @L_801C0F58:
    rlwinm. r0, r28, 0, 26, 26
    beq @L_801C10A0
    lwz r0, 0x7c(r27)
    cmplwi r0, 0x0
    beq @L_801C10A0
    cmplwi r30, 0xb
    lwz r3, 0x7c(r27)
    bgt @L_801C10A0
    lis r4, jumptable_8036D9D4@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D9D4@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    mr r4, r27
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    mr r4, r27
    li r5, 0x6
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    mr r4, r27
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    mr r4, r27
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    mr r4, r27
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    mr r4, r27
    lfs f1, 0x0(r31)
    li r5, 0x6
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    mr r4, r27
    lwz r6, 0x0(r31)
    li r5, 0x6
    mtctr r12
    bctrl
    b @L_801C10A0
    mr r12, r29
    mr r4, r27
    lwz r6, 0x0(r31)
    li r5, 0x6
    mtctr r12
    bctrl
    @L_801C10A0:
    lwz r0, 0x14(r27)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801C10CC
    lwz r3, 0x18(r27)
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mr r7, r31
    bl fn_801C1274
    @L_801C10CC:
    lwz r26, 0x80(r27)
    b @L_801C1220
    @L_801C10D4:
    rlwinm. r0, r28, 0, 22, 22
    beq @L_801C121C
    lwz r0, 0x18(r26)
    cmplwi r0, 0x0
    beq @L_801C121C
    cmplwi r30, 0xb
    lwz r3, 0x18(r26)
    bgt @L_801C121C
    lis r4, jumptable_8036D9A4@ha
    slwi r0, r30, 2
    addi r4, r4, jumptable_8036D9A4@l
    lwzx r4, r4, r0
    mtctr r4
    bctr
    mr r12, r29
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    lwz r4, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    mr r4, r26
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    mr r4, r26
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    mr r4, r26
    lwz r5, 0x0(r31)
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    mr r4, r26
    lfs f1, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    b @L_801C121C
    mr r12, r29
    mr r4, r26
    lwz r6, 0x0(r31)
    li r5, 0xa
    mtctr r12
    bctrl
    @L_801C121C:
    lwz r26, 0x0(r26)
    @L_801C1220:
    cmplwi r26, 0x0
    bne @L_801C10D4
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801C1260
    lwz r27, 0x10(r27)
    b @L_801C1258
    @L_801C123C:
    mr r3, r27
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mr r7, r31
    bl fn_801C0F20
    lwz r27, 0x8(r27)
    @L_801C1258:
    cmplwi r27, 0x0
    bne @L_801C123C
    @L_801C1260:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
