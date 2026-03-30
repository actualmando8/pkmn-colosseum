    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lfs f0, lbl_8047D814(r2)
    stw r0, 0x34(r1)
    fmuls f4, f0, f1
    stw r31, 0x2c(r1)
    fmuls f1, f0, f2
    fmuls f0, f0, f3
    li r31, 0x0
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    mr r29, r4
    stw r28, 0x20(r1)
    mr r28, r3
    stfs f4, 0x8(r1)
    stfs f1, 0xc(r1)
    stfs f0, 0x10(r1)
    b @L_80185FC4
    @L_80185F8C:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80185FC0
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_80185FC0
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_80185FC0
    lwz r31, 0x4(r3)
    b @L_80186030
    @L_80185FC0:
    addi r31, r31, 0x1
    @L_80185FC4:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_80185F8C
    li r30, 0x0
    b @L_80186020
    @L_80185FD8:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018601C
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_8018601C
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_80186030
    @L_8018601C:
    addi r30, r30, 0x1
    @L_80186020:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80185FD8
    li r31, 0x0
    @L_80186030:
    li r30, 0x0
    b @L_80186064
    @L_80186038:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80186060
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_80186060
    mr r31, r3
    b @L_80186074
    @L_80186060:
    addi r30, r30, 0x1
    @L_80186064:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80186038
    li r31, 0x0
    @L_80186074:
    cmplwi r31, 0x0
    beq @L_801860D8
    lfd f2, lbl_8047D7F0(r2)
    lfs f0, 0x8(r1)
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f3, f1
    lfd f2, lbl_8047D7F0(r2)
    lfs f0, 0xc(r1)
    fadd f1, f2, f0
    stfs f3, 0x8(r1)
    bl fn_800CE318
    frsp f3, f1
    lfd f2, lbl_8047D7F0(r2)
    lfs f0, 0x10(r1)
    fadd f1, f2, f0
    stfs f3, 0xc(r1)
    bl fn_800CE318
    frsp f0, f1
    mr r3, r31
    addi r4, r1, 0x8
    stfs f0, 0x10(r1)
    bl fn_8018FC08
    lfs f0, 0xc(r1)
    stfs f0, 0x40(r31)
    @L_801860D8:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
