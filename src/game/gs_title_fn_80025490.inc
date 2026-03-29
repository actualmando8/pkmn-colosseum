    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    stw r28, 0x20(r1)
    mr r31, r4
    lwz r0, lbl_8047A370(r13)
    cmpwi r0, 0x1
    beq @L_800254C8
    bge @L_800254FC
    cmpwi r0, 0x0
    b @L_800254FC
    @L_800254C8:
    lfs f1, lbl_80478898(r13)
    bl fn_800E0CA0
    lis r3, lbl_803A2058@ha
    lis r5, lbl_803A204C@ha
    addi r4, r3, lbl_803A2058@l
    addi r3, r1, 0x8
    addi r5, r5, lbl_803A204C@l
    bl fn_800E090C
    lfs f0, 0xc(r1)
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r29, 0x1c(r1)
    b @L_80025570
    @L_800254FC:
    lwz r4, lbl_80478DD8(r13)
    lwz r28, lbl_8047A368(r13)
    lwz r0, 0x0(r4)
    cmplw r28, r0
    bge @L_80025570
    lwz r3, 0x4(r3)
    bl fn_8005DA18
    lha r3, 0x4(r3)
    bl fn_8005D934
    li r30, 0x0
    @L_80025524:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 24
    cmplwi r0, 0x0
    beq @L_80025544
    cmpw r28, r30
    bne @L_80025540
    b @L_80025564
    @L_80025540:
    addi r30, r30, 0x1
    @L_80025544:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 25
    cmplwi r0, 0x0
    bne @L_80025560
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_80025524
    @L_80025560:
    li r3, 0x0
    @L_80025564:
    cmplwi r3, 0x0
    beq @L_80025570
    lha r29, 0x4(r3)
    @L_80025570:
    li r3, 0x0
    addi r0, r29, 0xf
    sth r3, 0x50(r31)
    extsh r0, r0
    sth r0, 0x52(r31)
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r29, 0x24(r1)
    lwz r28, 0x20(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
