    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stmw r25, 0x14(r1)
    mr r28, r3
    mr r25, r4
    lha r0, 0x6(r25)
    cmpwi r0, 0x12b2
    beq @L_800443B0
    b @L_800443D8
    @L_800443B0:
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BCA0(r2)
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x54(r3)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r28)
    b @L_800443FC
    @L_800443D8:
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BCA0(r2)
    addi r3, r3, lbl_803A6818@l
    lfs f0, 0x4c(r3)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r28)
    @L_800443FC:
    lha r0, 0x6(r25)
    cmpwi r0, 0xd46
    beq @L_8004460C
    bge @L_8004442C
    cmpwi r0, 0x76d
    beq @L_80044474
    bge @L_8004448C
    cmpwi r0, 0x31f
    bge @L_8004448C
    cmpwi r0, 0x31d
    bge @L_8004460C
    b @L_8004448C
    @L_8004442C:
    cmpwi r0, 0x12b2
    beq @L_8004460C
    bge @L_8004448C
    cmpwi r0, 0x119b
    beq @L_80044444
    b @L_8004448C
    @L_80044444:
    bl fn_8004BDEC
    extsb r0, r3
    cmpwi r0, 0x1
    bne @L_8004460C
    bl fn_8004BDFC
    extsb r0, r3
    cmpwi r0, 0x1
    blt @L_8004460C
    mr r3, r28
    mr r4, r25
    bl fn_800492CC
    b @L_8004460C
    @L_80044474:
    li r3, 0x0
    bl fn_8025FEE4
    clrlwi r4, r3, 16
    li r3, 0x34
    bl fn_80132A38
    b @L_8004460C
    @L_8004448C:
    mulli r0, r0, 0x1c
    lis r3, lbl_802EF0A8@ha
    addi r6, r3, lbl_802EF0A8@l
    add r4, r6, r0
    lha r3, 0x5712(r6)
    lha r5, 0x2(r4)
    lha r4, 0x4(r4)
    lha r0, 0x5714(r6)
    subf r3, r5, r3
    lha r5, 0x5716(r6)
    lha r6, 0x5718(r6)
    subf r4, r4, r0
    bl fn_800FE38C
    lis r3, lbl_803A6818@ha
    li r30, 0x0
    addi r31, r3, lbl_803A6818@l
    lfs f30, 0x30(r31)
    mr r29, r30
    lfs f31, lbl_8047BCF4(r2)
    b @L_800445FC
    @L_800444DC:
    lwz r3, 0x8(r31)
    subi r0, r3, 0x1
    cmpw r30, r0
    blt @L_800445F0
    lwz r3, 0xc(r31)
    addi r0, r3, 0x1
    cmpw r30, r0
    bgt @L_800445F0
    lwz r25, lbl_8047A4E0(r13)
    cmplwi r25, 0x0
    beq @L_800445A8
    beq @L_80044580
    lwz r3, lbl_8047A4E4(r13)
    lhzx r27, r3, r29
    cmplwi r27, 0x8000
    blt @L_80044520
    clrlwi r27, r27, 18
    @L_80044520:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r25
    clrlwi r4, r27, 16
    li r5, 0xa
    bl fn_801240C4
    mr r4, r27
    li r3, 0x0
    bl fn_8025FDDC
    mr r26, r3
    mr r4, r27
    li r3, 0x0
    bl fn_8025FD34
    mr r27, r3
    mr r3, r25
    mr r4, r26
    bl fn_8011DFE0
    mr r3, r25
    mr r4, r27
    bl fn_8011DF90
    lwz r3, lbl_8047A4E0(r13)
    b @L_80044584
    @L_80044580:
    li r3, 0x0
    @L_80044584:
    cmplwi r3, 0x0
    beq @L_800445A0
    bl fn_8011F5C8
    bl fn_8011E778
    bl fn_8011E760
    bl fn_800FA280
    b @L_800445AC
    @L_800445A0:
    li r3, 0x0
    b @L_800445AC
    @L_800445A8:
    li r3, 0x0
    @L_800445AC:
    cmplwi r3, 0x0
    bne @L_800445BC
    li r3, 0x1
    bl fn_800FA280
    @L_800445BC:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    fctiwz f0, f30
    lbz r4, 0x8b(r28)
    li r0, -0x100
    li r3, 0x0
    or r5, r4, r0
    li r6, 0xe7
    stfd f0, 0x8(r1)
    lwz r4, 0xc(r1)
    subi r4, r4, 0x2
    bl fn_800FB680
    @L_800445F0:
    fadds f30, f30, f31
    addi r29, r29, 0x2
    addi r30, r30, 0x1
    @L_800445FC:
    lhz r0, lbl_8047A4E8(r13)
    cmpw r30, r0
    blt @L_800444DC
    bl fn_800FE35C
    @L_8004460C:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    lmw r25, 0x14(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
