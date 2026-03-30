    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r22, 0x18(r1)
    mr. r25, r3
    mr r26, r4
    mr r27, r5
    beq @L_80142970
    clrlwi. r0, r26, 16
    beq @L_80142970
    clrlwi. r0, r27, 24
    beq @L_80142620
    cmplwi r0, 0x1
    bne @L_801427AC
    @L_80142620:
    clrlwi r29, r26, 16
    mr r31, r25
    subi r30, r29, 0x1
    li r28, 0x0
    b @L_801427A4
    @L_80142634:
    cmplwi r31, 0x0
    bne @L_80142644
    li r0, 0x0
    b @L_801426B8
    @L_80142644:
    mr r3, r31
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r24, r3, 16
    bne @L_80142668
    li r0, 0x0
    b @L_801426B8
    @L_80142668:
    mr r4, r24
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_8014268C
    li r0, 0x0
    b @L_801426A4
    @L_8014268C:
    lwz r0, lbl_80478BD8(r13)
    cmplw r24, r0
    blt @L_801426A0
    li r0, 0x0
    b @L_801426A4
    @L_801426A0:
    li r0, 0x1
    @L_801426A4:
    clrlwi. r0, r0, 24
    bne @L_801426B4
    li r0, 0x0
    b @L_801426B8
    @L_801426B4:
    li r0, 0x1
    @L_801426B8:
    clrlwi. r0, r0, 24
    bne @L_8014279C
    addi r22, r28, 0x1
    slwi r0, r22, 2
    add r23, r25, r0
    b @L_80142794
    @L_801426D0:
    cmplwi r23, 0x0
    bne @L_801426E0
    li r0, 0x0
    b @L_80142754
    @L_801426E0:
    mr r3, r23
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r24, r3, 16
    bne @L_80142704
    li r0, 0x0
    b @L_80142754
    @L_80142704:
    mr r4, r24
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80142728
    li r0, 0x0
    b @L_80142740
    @L_80142728:
    lwz r0, lbl_80478BD8(r13)
    cmplw r24, r0
    blt @L_8014273C
    li r0, 0x0
    b @L_80142740
    @L_8014273C:
    li r0, 0x1
    @L_80142740:
    clrlwi. r0, r0, 24
    bne @L_80142750
    li r0, 0x0
    b @L_80142754
    @L_80142750:
    li r0, 0x1
    @L_80142754:
    clrlwi r0, r0, 24
    cmplwi r0, 0x1
    bne @L_8014278C
    cmplwi r31, 0x0
    beq @L_8014279C
    slwi r0, r22, 2
    add. r4, r25, r0
    beq @L_8014279C
    lwz r3, 0x0(r31)
    lwz r0, 0x0(r4)
    stw r3, 0xc(r1)
    stw r0, 0x0(r31)
    stw r3, 0x0(r4)
    b @L_8014279C
    @L_8014278C:
    addi r23, r23, 0x4
    addi r22, r22, 0x1
    @L_80142794:
    cmpw r22, r29
    blt @L_801426D0
    @L_8014279C:
    addi r31, r31, 0x4
    addi r28, r28, 0x1
    @L_801427A4:
    cmpw r28, r30
    blt @L_80142634
    @L_801427AC:
    clrlwi r0, r27, 24
    cmplwi r0, 0x1
    bne @L_80142970
    clrlwi r29, r26, 16
    mr r30, r25
    subi r31, r29, 0x1
    li r28, 0x0
    b @L_80142968
    @L_801427CC:
    cmplwi r30, 0x0
    bne @L_801427DC
    li r0, 0x0
    b @L_80142850
    @L_801427DC:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r24, r3, 16
    bne @L_80142800
    li r0, 0x0
    b @L_80142850
    @L_80142800:
    mr r4, r24
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80142824
    li r0, 0x0
    b @L_8014283C
    @L_80142824:
    lwz r0, lbl_80478BD8(r13)
    cmplw r24, r0
    blt @L_80142838
    li r0, 0x0
    b @L_8014283C
    @L_80142838:
    li r0, 0x1
    @L_8014283C:
    clrlwi. r0, r0, 24
    bne @L_8014284C
    li r0, 0x0
    b @L_80142850
    @L_8014284C:
    li r0, 0x1
    @L_80142850:
    clrlwi. r0, r0, 24
    beq @L_80142960
    addi r26, r28, 0x1
    slwi r0, r26, 2
    add r23, r25, r0
    b @L_80142958
    @L_80142868:
    cmplwi r23, 0x0
    bne @L_80142878
    li r0, 0x0
    b @L_801428EC
    @L_80142878:
    mr r3, r23
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r24, r3, 16
    bne @L_8014289C
    li r0, 0x0
    b @L_801428EC
    @L_8014289C:
    mr r4, r24
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801428C0
    li r0, 0x0
    b @L_801428D8
    @L_801428C0:
    lwz r0, lbl_80478BD8(r13)
    cmplw r24, r0
    blt @L_801428D4
    li r0, 0x0
    b @L_801428D8
    @L_801428D4:
    li r0, 0x1
    @L_801428D8:
    clrlwi. r0, r0, 24
    bne @L_801428E8
    li r0, 0x0
    b @L_801428EC
    @L_801428E8:
    li r0, 0x1
    @L_801428EC:
    clrlwi. r0, r0, 24
    beq @L_80142950
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r22, r3, 16
    mr r3, r23
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r0, r3, 16
    cmplw r22, r0
    ble @L_80142950
    cmplwi r30, 0x0
    beq @L_80142950
    cmplwi r23, 0x0
    beq @L_80142950
    lwz r3, 0x0(r30)
    lwz r0, 0x0(r23)
    stw r3, 0x8(r1)
    stw r0, 0x0(r30)
    stw r3, 0x0(r23)
    @L_80142950:
    addi r23, r23, 0x4
    addi r26, r26, 0x1
    @L_80142958:
    cmpw r26, r29
    blt @L_80142868
    @L_80142960:
    addi r30, r30, 0x4
    addi r28, r28, 0x1
    @L_80142968:
    cmpw r28, r31
    blt @L_801427CC
    @L_80142970:
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
