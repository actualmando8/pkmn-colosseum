    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r31, r4
    lwz r26, 0x60(r3)
    lwz r28, 0x4(r26)
    mr r3, r28
    bl fn_801D16F0
    bl fn_8017B2CC
    cmpwi r3, 0x1
    bne @L_8004E318
    li r0, 0x0
    b @L_8004E31C
    @L_8004E318:
    li r0, 0x1
    @L_8004E31C:
    cmpwi r0, 0x0
    bne @L_8004E32C
    li r28, -0x1
    b @L_8004E374
    @L_8004E32C:
    mr r3, r28
    bl fn_801D16F0
    mr r27, r3
    bl fn_8017B448
    li r28, 0x0
    mr r30, r3
    mr r29, r28
    b @L_8004E36C
    @L_8004E34C:
    mr r3, r27
    mr r4, r29
    bl fn_8017B4BC
    bl fn_8017B5A4
    cmplwi r3, 0x9
    bne @L_8004E368
    addi r28, r28, 0x1
    @L_8004E368:
    addi r29, r29, 0x1
    @L_8004E36C:
    cmpw r29, r30
    blt @L_8004E34C
    @L_8004E374:
    cmpwi r28, 0x2
    bge @L_8004E38C
    mr r3, r31
    li r4, 0x0
    bl fn_80109220
    b @L_8004E398
    @L_8004E38C:
    mr r3, r31
    li r4, 0x1
    bl fn_80109220
    @L_8004E398:
    lha r0, 0x6(r31)
    cmpwi r0, 0x507
    bne @L_8004E3E8
    lwz r3, 0xc(r26)
    lis r0, 0x4330
    lwz r4, 0x0(r26)
    xoris r3, r3, 0x8000
    stw r0, 0x8(r1)
    lfd f1, lbl_8047BE40(r2)
    stw r3, 0xc(r1)
    lfs f3, lbl_8047BE38(r2)
    lfd f0, 0x8(r1)
    lfs f2, 0x0(r4)
    fsubs f0, f0, f1
    fmadds f0, f3, f2, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r0, 0x14(r1)
    sth r0, 0x50(r31)
    b @L_8004E428
    @L_8004E3E8:
    lwz r3, 0x10(r26)
    lis r0, 0x4330
    lwz r4, 0x0(r26)
    xoris r3, r3, 0x8000
    stw r0, 0x10(r1)
    lfd f1, lbl_8047BE40(r2)
    stw r3, 0x14(r1)
    lfs f3, lbl_8047BE3C(r2)
    lfd f0, 0x10(r1)
    lfs f2, 0x0(r4)
    fsubs f0, f0, f1
    fmadds f0, f3, f2, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    sth r0, 0x50(r31)
    @L_8004E428:
    li r3, 0x0
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
