    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r23, 0x1c(r1)
    mr r25, r3
    mr r26, r4
    lwz r30, 0x60(r25)
    lis r3, lbl_80266DD8@ha
    addi r3, r3, lbl_80266DD8@l
    lwz r0, 0x1c(r30)
    addi r27, r3, 0x4
    slwi r0, r0, 4
    lwzx r0, r27, r0
    cmpwi r0, 0x7
    beq @L_80026938
    li r0, 0x0
    stb r0, 0x67(r26)
    b @L_80026B2C
    @L_80026938:
    li r31, 0x0
    lwz r24, 0x18(r30)
    mr r29, r31
    addi r28, r1, 0xc
    b @L_800269B0
    @L_8002694C:
    lbz r5, 0x8b(r25)
    li r0, 0x0
    li r3, -0x100
    mr r4, r28
    sth r6, 0xc(r1)
    or r23, r5, r3
    li r3, 0x37
    sth r0, 0xe(r1)
    bl fn_80132A38
    li r3, 0xce
    bl fn_800FA444
    srwi r0, r3, 16
    mr r5, r23
    extsh r0, r0
    li r4, 0x0
    subfic r3, r0, 0x1b
    li r6, 0xce
    srwi r0, r3, 31
    add r0, r0, r3
    srawi r0, r0, 1
    add r3, r29, r0
    bl fn_800FB680
    addi r29, r29, 0x1a
    addi r31, r31, 0x1
    addi r24, r24, 0x2
    @L_800269B0:
    lhz r6, 0x0(r24)
    cmplwi r6, 0x0
    bne @L_8002694C
    lwz r0, 0x1c(r30)
    slwi r0, r0, 4
    lwzx r0, r27, r0
    cmpw r31, r0
    bge @L_80026B24
    lbz r0, 0x98(r25)
    cmplwi r0, 0x0
    bne @L_80026B24
    lwz r3, 0x24(r30)
    lwz r4, 0x28(r30)
    lwz r5, 0x0(r3)
    lwz r3, 0x2c(r30)
    cmpwi r5, 0x0
    lwz r25, 0x0(r4)
    lwz r0, 0x0(r3)
    blt @L_80026A04
    cmpwi r5, 0x2
    blt @L_80026A0C
    @L_80026A04:
    li r23, 0x0
    b @L_80026A70
    @L_80026A0C:
    cmpwi r0, 0x0
    blt @L_80026A1C
    cmpwi r0, 0x4
    blt @L_80026A24
    @L_80026A1C:
    li r23, 0x0
    b @L_80026A70
    @L_80026A24:
    mulli r3, r5, 0x18
    lis r4, lbl_80266E18@ha
    slwi r0, r0, 2
    addi r4, r4, lbl_80266E18@l
    add r3, r4, r3
    add r3, r3, r0
    lwz r23, 0x8(r3)
    mr r3, r23
    bl fn_800FA314
    cmpwi r25, 0x0
    blt @L_80026A58
    cmpw r25, r3
    blt @L_80026A60
    @L_80026A58:
    li r23, 0x0
    b @L_80026A70
    @L_80026A60:
    mr r3, r23
    bl fn_800FA280
    slwi r0, r25, 1
    lhzx r23, r3, r0
    @L_80026A70:
    clrlwi r0, r23, 16
    cmplwi r0, 0x0
    beq @L_80026B24
    li r3, 0x2efc
    bl fn_800FA280
    lhz r0, 0x0(r3)
    clrlwi r3, r23, 16
    cmplw r3, r0
    bne @L_80026A9C
    li r0, 0x0
    b @L_80026AA0
    @L_80026A9C:
    li r0, 0x6
    @L_80026AA0:
    cmpwi r0, 0x6
    bne @L_80026B24
    lwz r3, 0x30(r30)
    li r0, 0x0
    lfs f1, lbl_8047B934(r2)
    addi r4, r1, 0x8
    lfs f0, 0x0(r3)
    li r3, 0x37
    lfs f2, lbl_8047B938(r2)
    fsubs f0, f1, f0
    sth r23, 0x8(r1)
    sth r0, 0xa(r1)
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    clrlwi r0, r0, 24
    oris r23, r0, 0xff
    bl fn_80132A38
    li r3, 0xce
    bl fn_800FA444
    srwi r0, r3, 16
    mr r5, r23
    extsh r0, r0
    li r4, 0x0
    subfic r3, r0, 0x1b
    li r6, 0xce
    srwi r0, r3, 31
    add r3, r0, r3
    mulli r0, r31, 0x1a
    srawi r3, r3, 1
    add r3, r0, r3
    bl fn_800FB680
    @L_80026B24:
    li r0, 0xff
    stb r0, 0x67(r26)
    @L_80026B2C:
    li r3, 0x0
    lmw r23, 0x1c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
