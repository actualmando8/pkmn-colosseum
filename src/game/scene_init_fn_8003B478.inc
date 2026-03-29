    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r31, r3
    lis r3, lbl_803A6748@ha
    lfs f1, lbl_8047BAC0(r2)
    addi r3, r3, lbl_803A6748@l
    lfs f0, 0x44(r3)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0x8b(r31)
    lwz r3, 0x0(r3)
    lwz r4, lbl_8047A4D4(r13)
    clrlwi r0, r3, 16
    slwi r3, r3, 2
    addi r5, r4, 0x2
    lhzx r29, r4, r3
    slwi r0, r0, 2
    lhzx r26, r5, r3
    lhzx r27, r5, r0
    lwz r28, lbl_8047A4D0(r13)
    mr r3, r27
    bl fn_801EE614
    mr r3, r27
    bl fn_801EE8F4
    mr r3, r27
    bl fn_801EEAD0
    mr r30, r3
    mr r3, r27
    bl fn_801EEC74
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_8003B524
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003B51C
    li r27, 0x1
    b @L_8003B528
    @L_8003B51C:
    li r27, 0x0
    b @L_8003B528
    @L_8003B524:
    li r27, 0x2
    @L_8003B528:
    li r3, 0x0
    li r4, 0x1
    bl fn_80135938
    mr r6, r3
    mr r3, r28
    mr r4, r29
    li r5, 0xa
    bl fn_801240C4
    lis r4, lbl_803A6748@ha
    mr r3, r26
    addi r30, r4, lbl_803A6748@l
    lwz r29, 0x98(r30)
    bl fn_801EE750
    mr r3, r28
    mr r4, r29
    bl fn_8011DFE0
    cmpwi r27, 0x1
    beq @L_8003B5A4
    bge @L_8003B580
    cmpwi r27, 0x0
    bge @L_8003B588
    b @L_8003B5BC
    @L_8003B580:
    cmpwi r27, 0x3
    bge @L_8003B5BC
    @L_8003B588:
    mr r3, r28
    li r4, 0x1
    bl fn_8011D8F4
    mr r3, r28
    li r4, 0xa
    bl fn_8011D8D8
    b @L_8003B5BC
    @L_8003B5A4:
    mr r3, r28
    li r4, 0x0
    bl fn_8011D8F4
    mr r3, r28
    li r4, 0x0
    bl fn_8011D8D8
    @L_8003B5BC:
    lwz r27, lbl_8047A4D0(r13)
    cmplwi r27, 0x0
    beq @L_8003B628
    clrlwi r0, r26, 16
    cmplwi r0, 0x0
    beq @L_8003B620
    mr r3, r26
    bl fn_801EE8F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003B618
    mr r3, r26
    bl fn_801EE750
    mr r4, r3
    mr r3, r27
    bl fn_8011DFE0
    mr r3, r27
    bl fn_801231A4
    lwz r4, 0x98(r30)
    mr r30, r3
    mr r3, r27
    bl fn_8011DFE0
    b @L_8003B62C
    @L_8003B618:
    li r30, 0x2
    b @L_8003B62C
    @L_8003B620:
    li r30, 0x80
    b @L_8003B62C
    @L_8003B628:
    li r30, 0x80
    @L_8003B62C:
    clrlwi r0, r30, 24
    cmplwi r0, 0x80
    bne @L_8003B644
    li r3, 0x1
    bl fn_800FA280
    b @L_8003B694
    @L_8003B644:
    cmpwi r0, 0x1
    beq @L_8003B674
    bge @L_8003B65C
    cmpwi r0, 0x0
    bge @L_8003B668
    b @L_8003B68C
    @L_8003B65C:
    cmpwi r0, 0x3
    bge @L_8003B68C
    b @L_8003B680
    @L_8003B668:
    li r3, 0xd67
    bl fn_800FA280
    b @L_8003B694
    @L_8003B674:
    li r3, 0xd68
    bl fn_800FA280
    b @L_8003B694
    @L_8003B680:
    li r3, 0xd69
    bl fn_800FA280
    b @L_8003B694
    @L_8003B68C:
    li r3, 0xd68
    bl fn_800FA280
    @L_8003B694:
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    lbz r5, 0x8b(r31)
    li r0, -0x100
    li r3, 0x0
    li r4, 0x0
    or r5, r5, r0
    li r6, 0xcf
    bl fn_800FB680
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
