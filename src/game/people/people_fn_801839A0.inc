    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stfd f30, 0x20(r1)
    psq_st f30, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    fmr f30, f1
    mr r28, r3
    fmr f31, f2
    mr r29, r4
    li r31, 0x0
    b @L_80183A1C
    @L_801839E4:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183A18
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80183A18
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80183A18
    lwz r31, 0x4(r3)
    b @L_80183A88
    @L_80183A18:
    addi r31, r31, 0x1
    @L_80183A1C:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_801839E4
    li r30, 0x0
    b @L_80183A78
    @L_80183A30:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183A74
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_80183A74
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_80183A88
    @L_80183A74:
    addi r30, r30, 0x1
    @L_80183A78:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80183A30
    li r31, 0x0
    @L_80183A88:
    li r30, 0x0
    b @L_80183ABC
    @L_80183A90:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80183AB8
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_80183AB8
    mr r31, r3
    b @L_80183ACC
    @L_80183AB8:
    addi r30, r30, 0x1
    @L_80183ABC:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80183A90
    li r31, 0x0
    @L_80183ACC:
    cmplwi r31, 0x0
    bne @L_80183ADC
    li r3, 0x0
    b @L_80183B14
    @L_80183ADC:
    li r3, 0x5
    li r0, 0x0
    stb r3, 0x54(r31)
    mr r3, r31
    addi r4, r31, 0x74
    stb r0, 0x55(r31)
    bl fn_8018FC98
    lfs f1, lbl_8047D7A0(r2)
    li r3, 0x1
    lfs f0, lbl_8047D79C(r2)
    stfs f1, 0x84(r31)
    stfs f30, 0x88(r31)
    stfs f31, 0x8c(r31)
    stfs f0, 0x58(r31)
    @L_80183B14:
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    psq_l f30, 0x28(r1), 0, 0
    lfd f30, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r0, 0x44(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
