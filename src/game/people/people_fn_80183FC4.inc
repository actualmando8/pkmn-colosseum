    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x28(r1)
    fmr f31, f3
    stfd f30, 0x20(r1)
    fmr f30, f2
    stfd f29, 0x18(r1)
    fmr f29, f1
    stw r31, 0x14(r1)
    li r31, 0x0
    stw r30, 0x10(r1)
    stw r29, 0xc(r1)
    mr r29, r4
    stw r28, 0x8(r1)
    mr r28, r3
    b @L_80184040
    @L_80184008:
    mr r3, r31
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8018403C
    lwz r0, 0x28(r3)
    cmplw r0, r28
    bne @L_8018403C
    lwz r0, 0x2c(r3)
    cmplw r0, r29
    bne @L_8018403C
    lwz r31, 0x4(r3)
    b @L_801840AC
    @L_8018403C:
    addi r31, r31, 0x1
    @L_80184040:
    bl fn_8018FDB4
    cmpw r31, r3
    blt @L_80184008
    li r30, 0x0
    b @L_8018409C
    @L_80184054:
    mr r3, r30
    bl fn_8018FD88
    mr r31, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80184098
    lwz r0, 0x2c(r31)
    cmplw r0, r29
    bne @L_80184098
    lis r3, lbl_80273FD8@ha
    mr r4, r28
    addi r3, r3, lbl_80273FD8@l
    mr r5, r29
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r31, 0x4(r31)
    b @L_801840AC
    @L_80184098:
    addi r30, r30, 0x1
    @L_8018409C:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_80184054
    li r31, 0x0
    @L_801840AC:
    li r30, 0x0
    b @L_801840E0
    @L_801840B4:
    mr r3, r30
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801840DC
    lwz r0, 0x4(r3)
    cmplw r0, r31
    bne @L_801840DC
    mr r31, r3
    b @L_801840F0
    @L_801840DC:
    addi r30, r30, 0x1
    @L_801840E0:
    bl fn_8018FDB4
    cmpw r30, r3
    blt @L_801840B4
    li r31, 0x0
    @L_801840F0:
    cmplwi r31, 0x0
    bne @L_80184100
    li r3, 0x0
    b @L_80184164
    @L_80184100:
    lwz r4, 0x70(r31)
    cmplwi r4, 0x0
    bne @L_80184114
    li r3, 0x0
    b @L_80184164
    @L_80184114:
    lhz r3, 0x6a(r31)
    lhz r0, 0x68(r31)
    cmplw r3, r0
    blt @L_8018413C
    lis r3, lbl_8027404C@ha
    addi r3, r3, lbl_8027404C@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_80184164
    @L_8018413C:
    mulli r0, r3, 0xc
    fmr f1, f29
    fmr f2, f30
    fmr f3, f31
    add r3, r4, r0
    bl fn_800E01F4
    lhz r4, 0x6a(r31)
    li r3, 0x1
    addi r0, r4, 0x1
    sth r0, 0x6a(r31)
    @L_80184164:
    lwz r0, 0x34(r1)
    lfd f31, 0x28(r1)
    lfd f30, 0x20(r1)
    lfd f29, 0x18(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    lwz r29, 0xc(r1)
    lwz r28, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
