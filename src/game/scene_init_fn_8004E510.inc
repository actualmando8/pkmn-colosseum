    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r29, r4
    lwz r30, 0x60(r3)
    lwz r26, 0x4(r30)
    mr r3, r26
    bl fn_801D16F0
    bl fn_8017B2CC
    cmpwi r3, 0x1
    bne @L_8004E548
    li r0, 0x0
    b @L_8004E54C
    @L_8004E548:
    li r0, 0x1
    @L_8004E54C:
    cmpwi r0, 0x0
    bne @L_8004E55C
    li r26, -0x1
    b @L_8004E5A4
    @L_8004E55C:
    mr r3, r26
    bl fn_801D16F0
    mr r24, r3
    bl fn_8017B448
    li r26, 0x0
    mr r28, r3
    mr r27, r26
    b @L_8004E59C
    @L_8004E57C:
    mr r3, r24
    mr r4, r27
    bl fn_8017B4BC
    bl fn_8017B5A4
    cmplwi r3, 0x9
    bne @L_8004E598
    addi r26, r26, 0x1
    @L_8004E598:
    addi r27, r27, 0x1
    @L_8004E59C:
    cmpw r27, r28
    blt @L_8004E57C
    @L_8004E5A4:
    cmpwi r26, 0x0
    bgt @L_8004E5B4
    li r3, 0x0
    b @L_8004E77C
    @L_8004E5B4:
    lwz r3, 0x8(r30)
    lwz r26, 0x4(r30)
    lwz r25, 0x0(r3)
    mr r3, r26
    bl fn_801D16F0
    bl fn_8017B2CC
    cmpwi r3, 0x1
    bne @L_8004E5DC
    li r0, 0x0
    b @L_8004E5E0
    @L_8004E5DC:
    li r0, 0x1
    @L_8004E5E0:
    cmpwi r0, 0x0
    bne @L_8004E5F0
    li r24, -0x1
    b @L_8004E638
    @L_8004E5F0:
    mr r3, r26
    bl fn_801D16F0
    mr r23, r3
    bl fn_8017B448
    li r24, 0x0
    mr r28, r3
    mr r27, r24
    b @L_8004E630
    @L_8004E610:
    mr r3, r23
    mr r4, r27
    bl fn_8017B4BC
    bl fn_8017B5A4
    cmplwi r3, 0x9
    bne @L_8004E62C
    addi r24, r24, 0x1
    @L_8004E62C:
    addi r27, r27, 0x1
    @L_8004E630:
    cmpw r27, r28
    blt @L_8004E610
    @L_8004E638:
    cmpw r24, r25
    bgt @L_8004E648
    li r31, -0x1
    b @L_8004E694
    @L_8004E648:
    mr r3, r26
    bl fn_801D16F0
    li r26, 0x0
    mr r28, r3
    mr r27, r26
    b @L_8004E68C
    @L_8004E660:
    mr r3, r28
    mr r4, r27
    bl fn_8017B4BC
    mr r31, r3
    bl fn_8017B5A4
    cmplwi r3, 0x9
    bne @L_8004E688
    cmpw r26, r25
    bge @L_8004E694
    addi r26, r26, 0x1
    @L_8004E688:
    addi r27, r27, 0x1
    @L_8004E68C:
    cmpw r27, r24
    blt @L_8004E660
    @L_8004E694:
    addis r0, r31, 0x1
    cmplwi r0, 0xffff
    bne @L_8004E6A8
    li r3, 0x0
    b @L_8004E77C
    @L_8004E6A8:
    lwz r3, 0x4(r30)
    bl fn_801D16F0
    cmplwi r3, 0x0
    bne @L_8004E6C0
    li r3, 0x0
    b @L_8004E77C
    @L_8004E6C0:
    mr r4, r31
    bl fn_8017B07C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8004E6DC
    li r3, 0x0
    b @L_8004E77C
    @L_8004E6DC:
    mr r3, r31
    bl fn_800F92D4
    mr r23, r3
    cmplwi r23, 0x0
    bne @L_8004E6F8
    li r3, 0x0
    b @L_8004E77C
    @L_8004E6F8:
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    mr r4, r23
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x7
    bl fn_800D6A00
    li r3, 0x2
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    li r3, 0x0
    li r4, -0x1
    bl fn_800D5BA0
    lfs f1, lbl_8047BE48(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lha r3, 0x54(r29)
    lha r4, 0x56(r29)
    bl fn_800D61E4
    li r3, 0x0
    li r4, -0x1
    bl fn_800D5BA0
    lfs f1, lbl_8047BE4C(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    li r3, 0x0
    @L_8004E77C:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
