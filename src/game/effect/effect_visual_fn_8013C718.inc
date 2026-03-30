    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stmw r25, 0x14(r1)
    cmplwi r3, 0x0
    beq @L_8013CA10
    mr r31, r3
    li r0, 0x0
    sth r0, 0xa4(r3)
    li r4, 0x0
    li r5, 0x48
    bl memset
    lwz r4, 0x78(r31)
    cmpwi r4, 0x0
    beq @L_8013CA10
    lwz r3, 0x74(r31)
    bl fn_800F9318
    mr. r30, r3
    beq @L_8013CA10
    bl fn_800B8DF4
    bl fn_800B856C
    lhz r6, 0x72(r31)
    li r4, 0x20
    lhz r3, 0x70(r31)
    addi r0, r6, 0x1
    stw r30, 0x0(r31)
    addi r3, r3, 0x1
    sth r3, 0x18(r31)
    sth r0, 0x1a(r31)
    lhz r7, 0x18(r31)
    lhz r5, 0x1a(r31)
    slwi r3, r7, 4
    addi r0, r3, 0x3
    mullw r3, r6, r0
    mullw r8, r7, r5
    addi r0, r3, 0x1f
    clrrwi r0, r0, 5
    stw r0, 0x14(r31)
    slwi r6, r8, 2
    slwi r3, r8, 3
    lwz r5, 0x14(r31)
    addi r6, r6, 0x1f
    addi r0, r3, 0x1f
    mulli r7, r8, 0xc
    clrrwi r28, r6, 5
    clrrwi r0, r0, 5
    addi r3, r7, 0x1f
    add r0, r28, r0
    clrrwi r27, r3, 5
    add r25, r0, r5
    add r25, r27, r25
    mr r3, r25
    bl fn_800E2C04
    clrlwi. r0, r3, 16
    mr r29, r3
    beq @L_8013C848
    bl fn_800E27B0
    sth r29, 0x1c(r31)
    mr r29, r3
    mr r5, r25
    li r4, 0x0
    bl memset
    stw r29, 0x10(r31)
    li r3, 0x1
    lwz r0, 0x14(r31)
    add r0, r29, r0
    stw r0, 0x4(r31)
    add r0, r0, r27
    stw r0, 0xc(r31)
    add r0, r0, r28
    stw r0, 0x8(r31)
    b @L_8013C84C
    @L_8013C848:
    li r3, 0x0
    @L_8013C84C:
    clrlwi. r0, r3, 24
    beq @L_8013CA10
    lfs f1, 0x64(r31)
    mr r3, r31
    lfs f2, 0x68(r31)
    addi r4, r31, 0x48
    lfs f3, 0x6c(r31)
    addi r5, r31, 0x60
    bl fn_8013CBF0
    mr r3, r30
    mr r4, r31
    bl fn_8013D0A8
    clrlwi. r0, r3, 24
    beq @L_8013C9BC
    lhz r3, 0x70(r31)
    addi r25, r3, 0x1
    mulli r0, r25, 0x3
    slwi r3, r0, 2
    bl fn_800E3534
    clrlwi. r0, r3, 16
    beq @L_8013C92C
    sth r3, 0x8c(r31)
    bl fn_800E27B0
    slwi r0, r25, 2
    mr r26, r3
    add r29, r26, r0
    stw r3, 0x80(r31)
    add r28, r29, r0
    lfs f31, lbl_8047D230(r2)
    stw r29, 0x84(r31)
    li r27, 0x0
    stw r28, 0x88(r31)
    b @L_8013C91C
    @L_8013C8D0:
    lfs f30, 0x90(r31)
    bl fn_800E0BE4
    lfs f0, 0x94(r31)
    fsubs f0, f0, f30
    fmadds f0, f0, f1, f30
    stfs f0, 0x0(r26)
    lfs f30, 0x98(r31)
    bl fn_800E0BE4
    lfs f0, 0x9c(r31)
    fsubs f0, f0, f30
    fmadds f0, f0, f1, f30
    stfs f0, 0x0(r29)
    bl fn_800E0BE4
    fmuls f0, f31, f1
    addi r26, r26, 0x4
    addi r27, r27, 0x1
    addi r29, r29, 0x4
    stfs f0, 0x0(r28)
    addi r28, r28, 0x4
    @L_8013C91C:
    cmpw r27, r25
    blt @L_8013C8D0
    li r0, 0x1
    b @L_8013C930
    @L_8013C92C:
    li r0, 0x0
    @L_8013C930:
    clrlwi. r0, r0, 24
    beq @L_8013C9BC
    mr r3, r30
    li r4, 0x1
    bl fn_800E4014
    lfs f1, 0x58(r31)
    mr r3, r30
    lfs f0, lbl_8047D234(r2)
    addi r4, r31, 0x54
    fadds f0, f1, f0
    stfs f0, 0x58(r31)
    bl fn_800E4170
    mr r3, r30
    li r4, 0x0
    bl fn_800EC188
    mr r3, r30
    bl fn_800EC1B0
    clrlwi. r0, r3, 24
    beq @L_8013C9B4
    mr r3, r30
    li r4, 0x0
    bl fn_800EC35C
    lfs f1, lbl_8047D238(r2)
    mr r3, r30
    bl fn_800EC308
    lfs f1, lbl_8047D23C(r2)
    mr r3, r30
    bl fn_800EC2A4
    mr r3, r30
    li r4, 0x1
    bl fn_800EC208
    mr r3, r30
    bl fn_800EC1E4
    @L_8013C9B4:
    li r3, 0x1
    b @L_8013CA24
    @L_8013C9BC:
    mr r3, r30
    li r4, 0x0
    bl fn_800E4014
    lhz r25, 0x1c(r31)
    li r0, 0x0
    cmplwi r25, 0x0
    sth r0, 0x1c(r31)
    beq @L_8013C9EC
    mr r3, r25
    bl fn_800E24B0
    mr r3, r25
    bl fn_800E209C
    @L_8013C9EC:
    lhz r25, 0x8c(r31)
    li r0, 0x0
    cmplwi r25, 0x0
    sth r0, 0x8c(r31)
    beq @L_8013CA10
    mr r3, r25
    bl fn_800E24B0
    mr r3, r25
    bl fn_800E209C
    @L_8013CA10:
    lis r3, lbl_80272ED0@ha
    addi r3, r3, lbl_80272ED0@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013CA24:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    lmw r25, 0x14(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
