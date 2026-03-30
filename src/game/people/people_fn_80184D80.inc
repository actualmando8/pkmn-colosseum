    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stmw r27, 0x4c(r1)
    lis r4, lbl_80273FC0@ha
    mr r31, r3
    addi r5, r4, lbl_80273FC0@l
    lwz r4, 0x0(r5)
    lwz r3, 0x4(r5)
    lwz r0, 0x8(r5)
    stw r4, 0x2c(r1)
    stw r3, 0x30(r1)
    stw r0, 0x34(r1)
    lbz r0, 0x55(r31)
    cmpwi r0, 0x1
    beq @L_80184E60
    bge @L_80184DD8
    cmpwi r0, 0x0
    bge @L_80184DE4
    b @L_80185230
    @L_80184DD8:
    cmpwi r0, 0x3
    bge @L_80185230
    b @L_80185088
    @L_80184DE4:
    lfs f1, 0x84(r31)
    lfs f0, lbl_8047D7A0(r2)
    fcmpo cr0, f1, f0
    ble @L_80184E58
    bl fn_800D37CC
    xoris r3, r3, 0x8000
    lis r0, 0x4330
    stw r3, 0x3c(r1)
    lfd f1, lbl_8047D7C8(r2)
    stw r0, 0x38(r1)
    lfd f0, 0x38(r1)
    fsubs f31, f0, f1
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x44(r1)
    lfd f3, lbl_8047D7D0(r2)
    stw r0, 0x40(r1)
    lfs f1, 0x84(r31)
    lfd f2, 0x40(r1)
    lfs f0, lbl_8047D7A0(r2)
    fsubs f2, f2, f3
    fdivs f2, f2, f31
    fsubs f1, f1, f2
    stfs f1, 0x84(r31)
    lfs f1, 0x84(r31)
    fcmpo cr0, f1, f0
    bge @L_80185230
    stfs f0, 0x84(r31)
    b @L_80185230
    @L_80184E58:
    li r0, 0x1
    stb r0, 0x55(r31)
    @L_80184E60:
    bl fn_800E0BE4
    lfs f2, lbl_8047D7C4(r2)
    lfs f0, lbl_8047D818(r2)
    fmuls f1, f2, f1
    fmuls f31, f0, f1
    fmr f1, f31
    bl fn_800CE148
    lfs f0, 0x80(r31)
    fmul f0, f0, f1
    fmr f1, f31
    frsp f0, f0
    stfs f0, 0x2c(r1)
    bl fn_800CDBE0
    lfs f0, 0x80(r31)
    addi r3, r31, 0x5c
    addi r4, r31, 0x74
    addi r5, r1, 0x2c
    fmul f0, f0, f1
    frsp f0, f0
    stfs f0, 0x34(r1)
    bl fn_800E019C
    mr r3, r31
    bl fn_8018FCBC
    mr r5, r3
    addi r3, r1, 0x20
    addi r4, r31, 0x5c
    bl fn_800E0168
    lfs f1, 0x20(r1)
    lfs f2, 0x28(r1)
    bl fn_800CE2D8
    frsp f1, f1
    lfs f0, lbl_8047D7A0(r2)
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f1, f0
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f31, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f31, f0
    ble @L_80184F10
    lfd f0, lbl_8047D7F0(r2)
    fsub f31, f31, f0
    frsp f31, f31
    b @L_80184F28
    @L_80184F10:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f31, f0
    bge @L_80184F28
    lfd f0, lbl_8047D7F0(r2)
    fadd f31, f0, f31
    frsp f31, f31
    @L_80184F28:
    lwz r28, 0x2c(r31)
    li r30, 0x0
    lwz r29, 0x28(r31)
    b @L_80184F70
    @L_80184F38:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184F6C
    lwz r0, 0x28(r3)
    cmplw r0, r29
    bne @L_80184F6C
    lwz r0, 0x2c(r3)
    cmplw r0, r28
    bne @L_80184F6C
    lwz r28, 0x4(r3)
    b @L_80184FDC
    @L_80184F6C:
    addi r30, r30, 0x1
    @L_80184F70:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80184F38
    li r27, 0x0
    b @L_80184FCC
    @L_80184F84:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184FC8
    lwz r0, 0x2c(r30)
    cmplw r0, r28
    bne @L_80184FC8
    lis r3, lbl_80273FD8@ha
    mr r4, r29
    addi r3, r3, lbl_80273FD8@l
    mr r5, r28
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_80184FDC
    @L_80184FC8:
    addi r27, r27, 0x1
    @L_80184FCC:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80184F84
    li r28, 0x0
    @L_80184FDC:
    li r29, 0x0
    b @L_80185010
    @L_80184FE4:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018500C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018500C
    mr r28, r3
    b @L_80185020
    @L_8018500C:
    addi r29, r29, 0x1
    @L_80185010:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80184FE4
    li r28, 0x0
    @L_80185020:
    cmplwi r28, 0x0
    beq @L_80185080
    mr r3, r28
    addi r4, r1, 0x14
    bl fn_8018FC2C
    lfs f3, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0x18(r1)
    li r0, 0x1
    stw r3, 0x38(r1)
    fdivs f1, f0, f3
    lfd f2, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    lfs f0, lbl_8047D79C(r2)
    fctiwz f1, f1
    stfd f1, 0x40(r1)
    lwz r0, 0x44(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x3c(r1)
    lfd f1, 0x38(r1)
    fsubs f1, f1, f2
    fmadds f1, f3, f1, f31
    stfs f1, 0x40(r28)
    stfs f0, 0x44(r28)
    @L_80185080:
    li r0, 0x2
    stb r0, 0x55(r31)
    @L_80185088:
    mr r3, r31
    bl fn_80185AAC
    cmpwi r3, 0x2
    bne @L_8018520C
    lfd f1, lbl_8047D7A8(r2)
    li r30, 0x0
    lfs f0, 0x40(r31)
    lwz r29, 0x2c(r31)
    fadd f31, f1, f0
    lwz r28, 0x28(r31)
    frsp f31, f31
    b @L_801850F0
    @L_801850B8:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801850EC
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_801850EC
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_801850EC
    lwz r28, 0x4(r3)
    b @L_8018515C
    @L_801850EC:
    addi r30, r30, 0x1
    @L_801850F0:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_801850B8
    li r27, 0x0
    b @L_8018514C
    @L_80185104:
    mr r3, r27
    bl fn_8018FD88
    mr r30, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185148
    lwz r0, 0x2c(r30)
    cmplw r0, r29
    bne @L_80185148
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r28, 0x4(r30)
    b @L_8018515C
    @L_80185148:
    addi r27, r27, 0x1
    @L_8018514C:
    bl fn_8018FDB4
    cmpw r27, r3
    blt @L_80185104
    li r28, 0x0
    @L_8018515C:
    li r29, 0x0
    b @L_80185190
    @L_80185164:
    mr r3, r29
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018518C
    lwz r0, 0x4(r3)
    cmplw r0, r28
    bne @L_8018518C
    mr r28, r3
    b @L_801851A0
    @L_8018518C:
    addi r29, r29, 0x1
    @L_80185190:
    bl fn_8018FDB4
    cmpw r29, r3
    blt @L_80185164
    li r28, 0x0
    @L_801851A0:
    cmplwi r28, 0x0
    beq @L_80185200
    mr r3, r28
    addi r4, r1, 0x8
    bl fn_8018FC2C
    lfs f3, lbl_8047D7C0(r2)
    lis r3, 0x4330
    lfs f0, 0xc(r1)
    li r0, 0x1
    stw r3, 0x38(r1)
    fdivs f1, f0, f3
    lfd f2, lbl_8047D7C8(r2)
    stb r0, 0x22(r28)
    lfs f0, lbl_8047D79C(r2)
    fctiwz f1, f1
    stfd f1, 0x40(r1)
    lwz r0, 0x44(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x3c(r1)
    lfd f1, 0x38(r1)
    fsubs f1, f1, f2
    fmadds f31, f3, f1, f31
    stfs f31, 0x40(r28)
    stfs f0, 0x44(r28)
    @L_80185200:
    li r0, 0x2
    stb r0, 0x55(r31)
    b @L_80185230
    @L_8018520C:
    cmpwi r3, 0x1
    bne @L_80185230
    bl fn_800E0BA0
    lfs f2, 0x8c(r31)
    li r0, 0x0
    lfs f0, 0x88(r31)
    fmadds f0, f2, f1, f0
    stfs f0, 0x84(r31)
    stb r0, 0x55(r31)
    @L_80185230:
    psq_l f31, 0x68(r1), 0, 0
    lfd f31, 0x60(r1)
    lmw r27, 0x4c(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
