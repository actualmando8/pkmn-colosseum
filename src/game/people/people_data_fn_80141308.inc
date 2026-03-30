    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r19, 0xc(r1)
    mr. r31, r3
    mr r30, r4
    mr r27, r5
    mr r21, r6
    mr r20, r7
    mr r26, r8
    mr r28, r9
    mr r19, r10
    bne @L_80141344
    li r3, -0x1
    b @L_80142354
    @L_80141344:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141368
    li r0, 0x0
    b @L_80141384
    @L_80141368:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r27, 16
    cmplw r3, r0
    blt @L_80141380
    li r0, 0x0
    b @L_80141384
    @L_80141380:
    li r0, 0x1
    @L_80141384:
    clrlwi. r0, r0, 24
    bne @L_80141394
    li r3, -0x1
    b @L_80142354
    @L_80141394:
    clrlwi. r0, r19, 24
    clrlwi r23, r21, 16
    li r29, 0x0
    mr r24, r23
    bne @L_80141A64
    cmplwi r31, 0x0
    bne @L_801413B8
    li r25, 0x0
    b @L_80141514
    @L_801413B8:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801413DC
    li r0, 0x0
    b @L_801413F8
    @L_801413DC:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r27, 16
    cmplw r3, r0
    blt @L_801413F4
    li r0, 0x0
    b @L_801413F8
    @L_801413F4:
    li r0, 0x1
    @L_801413F8:
    clrlwi. r0, r0, 24
    bne @L_80141408
    li r25, 0x0
    b @L_80141514
    @L_80141408:
    clrlwi r23, r27, 16
    clrlwi r21, r30, 16
    li r22, 0x0
    b @L_80141504
    @L_80141418:
    clrlslwi r0, r22, 16, 2
    clrlwi r25, r22, 16
    add. r20, r31, r0
    bne @L_80141430
    li r0, 0x0
    b @L_801414EC
    @L_80141430:
    cmplwi r20, 0x0
    bne @L_80141440
    li r0, 0x0
    b @L_801414B4
    @L_80141440:
    mr r3, r20
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_80141464
    li r0, 0x0
    b @L_801414B4
    @L_80141464:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141488
    li r0, 0x0
    b @L_801414A0
    @L_80141488:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_8014149C
    li r0, 0x0
    b @L_801414A0
    @L_8014149C:
    li r0, 0x1
    @L_801414A0:
    clrlwi. r0, r0, 24
    bne @L_801414B0
    li r0, 0x0
    b @L_801414B4
    @L_801414B0:
    li r0, 0x1
    @L_801414B4:
    clrlwi. r0, r0, 24
    bne @L_801414C4
    li r0, 0x0
    b @L_801414EC
    @L_801414C4:
    mr r3, r20
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    cmpw r23, r3
    bne @L_801414E8
    li r0, 0x1
    b @L_801414EC
    @L_801414E8:
    li r0, 0x0
    @L_801414EC:
    clrlwi. r0, r0, 24
    beq @L_80141500
    slwi r0, r25, 2
    add r25, r31, r0
    b @L_80141514
    @L_80141500:
    addi r22, r22, 0x1
    @L_80141504:
    clrlwi r0, r22, 16
    cmplw r0, r21
    blt @L_80141418
    li r25, 0x0
    @L_80141514:
    cmplwi r25, 0x0
    beq @L_80141780
    mr r3, r25
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r23, r26, 16
    cmpw r3, r23
    bge @L_80142334
    cmplwi r25, 0x0
    bne @L_8014154C
    li r20, -0x1
    b @L_80141768
    @L_8014154C:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141570
    li r0, 0x0
    b @L_8014158C
    @L_80141570:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r27, 16
    cmplw r3, r0
    blt @L_80141588
    li r0, 0x0
    b @L_8014158C
    @L_80141588:
    li r0, 0x1
    @L_8014158C:
    clrlwi. r0, r0, 24
    bne @L_8014159C
    li r20, -0x1
    b @L_80141768
    @L_8014159C:
    cmplwi r25, 0x0
    bne @L_801415AC
    li r0, 0x0
    b @L_80141668
    @L_801415AC:
    bne @L_801415B8
    li r0, 0x0
    b @L_8014162C
    @L_801415B8:
    mr r3, r25
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_801415DC
    li r0, 0x0
    b @L_8014162C
    @L_801415DC:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141600
    li r0, 0x0
    b @L_80141618
    @L_80141600:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_80141614
    li r0, 0x0
    b @L_80141618
    @L_80141614:
    li r0, 0x1
    @L_80141618:
    clrlwi. r0, r0, 24
    bne @L_80141628
    li r0, 0x0
    b @L_8014162C
    @L_80141628:
    li r0, 0x1
    @L_8014162C:
    clrlwi. r0, r0, 24
    bne @L_8014163C
    li r0, 0x0
    b @L_80141668
    @L_8014163C:
    mr r3, r25
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r0, r27, 16
    cmpw r0, r3
    bne @L_80141664
    li r0, 0x1
    b @L_80141668
    @L_80141664:
    li r0, 0x0
    @L_80141668:
    clrlwi. r0, r0, 24
    bne @L_80141678
    li r20, -0x1
    b @L_80141768
    @L_80141678:
    cmplwi r25, 0x0
    bne @L_80141688
    li r20, -0x1
    b @L_80141768
    @L_80141688:
    bne @L_80141694
    li r0, 0x0
    b @L_80141708
    @L_80141694:
    mr r3, r25
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_801416B8
    li r0, 0x0
    b @L_80141708
    @L_801416B8:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801416DC
    li r0, 0x0
    b @L_801416F4
    @L_801416DC:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_801416F0
    li r0, 0x0
    b @L_801416F4
    @L_801416F0:
    li r0, 0x1
    @L_801416F4:
    clrlwi. r0, r0, 24
    bne @L_80141704
    li r0, 0x0
    b @L_80141708
    @L_80141704:
    li r0, 0x1
    @L_80141708:
    clrlwi. r0, r0, 24
    bne @L_80141718
    li r20, -0x1
    b @L_80141768
    @L_80141718:
    mr r3, r25
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r3, r3, 16
    clrlwi r0, r24, 16
    add r0, r3, r0
    cmpw r0, r23
    ble @L_8014174C
    subf r20, r23, r0
    clrlwi r0, r26, 16
    b @L_80141750
    @L_8014174C:
    li r20, 0x0
    @L_80141750:
    clrlwi r7, r0, 16
    mr r3, r25
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142B24
    @L_80141768:
    cmpwi r20, 0x0
    bge @L_80141778
    mr r3, r20
    b @L_80142354
    @L_80141778:
    mr r24, r20
    b @L_80142334
    @L_80141780:
    cmplwi r31, 0x0
    bne @L_80141790
    li r21, 0x0
    b @L_8014189C
    @L_80141790:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801417B4
    li r0, 0x0
    b @L_801417D0
    @L_801417B4:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r27, 16
    cmplw r3, r0
    blt @L_801417CC
    li r0, 0x0
    b @L_801417D0
    @L_801417CC:
    li r0, 0x1
    @L_801417D0:
    clrlwi. r0, r0, 24
    bne @L_801417E0
    li r21, 0x0
    b @L_8014189C
    @L_801417E0:
    clrlwi r21, r30, 16
    li r22, 0x0
    b @L_8014188C
    @L_801417EC:
    clrlslwi r0, r22, 16, 2
    clrlwi r19, r22, 16
    add. r3, r31, r0
    bne @L_80141804
    li r0, 0x0
    b @L_80141874
    @L_80141804:
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r20, r3, 16
    bne @L_80141824
    li r0, 0x0
    b @L_80141874
    @L_80141824:
    mr r4, r20
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141848
    li r0, 0x0
    b @L_80141860
    @L_80141848:
    lwz r0, lbl_80478BD8(r13)
    cmplw r20, r0
    blt @L_8014185C
    li r0, 0x0
    b @L_80141860
    @L_8014185C:
    li r0, 0x1
    @L_80141860:
    clrlwi. r0, r0, 24
    bne @L_80141870
    li r0, 0x0
    b @L_80141874
    @L_80141870:
    li r0, 0x1
    @L_80141874:
    clrlwi. r0, r0, 24
    bne @L_80141888
    slwi r0, r19, 2
    add r21, r31, r0
    b @L_8014189C
    @L_80141888:
    addi r22, r22, 0x1
    @L_8014188C:
    clrlwi r0, r22, 16
    cmplw r0, r21
    blt @L_801417EC
    li r21, 0x0
    @L_8014189C:
    cmplwi r21, 0x0
    beq @L_80142334
    bne @L_801418B0
    li r20, -0x1
    b @L_80141A48
    @L_801418B0:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801418D4
    li r0, 0x0
    b @L_801418F0
    @L_801418D4:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r27, 16
    cmplw r3, r0
    blt @L_801418EC
    li r0, 0x0
    b @L_801418F0
    @L_801418EC:
    li r0, 0x1
    @L_801418F0:
    clrlwi. r0, r0, 24
    bne @L_80141900
    li r20, -0x1
    b @L_80141A48
    @L_80141900:
    cmplwi r21, 0x0
    beq @L_80141938
    mr r3, r21
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    mr r3, r21
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    @L_80141938:
    mr r3, r21
    clrlwi r7, r27, 16
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142B24
    cmplwi r21, 0x0
    bne @L_80141960
    li r20, -0x1
    b @L_80141A44
    @L_80141960:
    bne @L_8014196C
    li r0, 0x0
    b @L_801419E0
    @L_8014196C:
    mr r3, r21
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_80141990
    li r0, 0x0
    b @L_801419E0
    @L_80141990:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801419B4
    li r0, 0x0
    b @L_801419CC
    @L_801419B4:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_801419C8
    li r0, 0x0
    b @L_801419CC
    @L_801419C8:
    li r0, 0x1
    @L_801419CC:
    clrlwi. r0, r0, 24
    bne @L_801419DC
    li r0, 0x0
    b @L_801419E0
    @L_801419DC:
    li r0, 0x1
    @L_801419E0:
    clrlwi. r0, r0, 24
    bne @L_801419F0
    li r20, -0x1
    b @L_80141A44
    @L_801419F0:
    mr r3, r21
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r3, r3, 16
    clrlwi r0, r24, 16
    add r0, r3, r0
    clrlwi r3, r26, 16
    cmpw r0, r3
    ble @L_80141A28
    subf r20, r3, r0
    mr r0, r3
    b @L_80141A2C
    @L_80141A28:
    li r20, 0x0
    @L_80141A2C:
    clrlwi r7, r0, 16
    mr r3, r21
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142B24
    @L_80141A44:
    cmpwi r20, 0x0
    @L_80141A48:
    cmpwi r20, 0x0
    bge @L_80141A58
    mr r3, r20
    b @L_80142354
    @L_80141A58:
    mr r24, r20
    li r29, 0x1
    b @L_80142334
    @L_80141A64:
    extsh. r0, r20
    bge @L_801420FC
    clrlwi r25, r27, 16
    clrlwi r26, r26, 16
    @L_80141A74:
    cmplwi r31, 0x0
    bne @L_80141A84
    li r22, 0x0
    b @L_80141BE4
    @L_80141A84:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141AA8
    li r0, 0x0
    b @L_80141AC0
    @L_80141AA8:
    lwz r0, lbl_80478BD8(r13)
    cmplw r25, r0
    blt @L_80141ABC
    li r0, 0x0
    b @L_80141AC0
    @L_80141ABC:
    li r0, 0x1
    @L_80141AC0:
    clrlwi. r0, r0, 24
    bne @L_80141AD0
    li r22, 0x0
    b @L_80141BE4
    @L_80141AD0:
    clrlwi r21, r30, 16
    li r23, 0x0
    b @L_80141BD4
    @L_80141ADC:
    clrlslwi r0, r23, 16, 2
    add. r22, r31, r0
    bne @L_80141AF0
    li r0, 0x0
    b @L_80141BA8
    @L_80141AF0:
    bne @L_80141AFC
    li r0, 0x0
    b @L_80141B70
    @L_80141AFC:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_80141B20
    li r0, 0x0
    b @L_80141B70
    @L_80141B20:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141B44
    li r0, 0x0
    b @L_80141B5C
    @L_80141B44:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_80141B58
    li r0, 0x0
    b @L_80141B5C
    @L_80141B58:
    li r0, 0x1
    @L_80141B5C:
    clrlwi. r0, r0, 24
    bne @L_80141B6C
    li r0, 0x0
    b @L_80141B70
    @L_80141B6C:
    li r0, 0x1
    @L_80141B70:
    clrlwi. r0, r0, 24
    bne @L_80141B80
    li r0, 0x0
    b @L_80141BA8
    @L_80141B80:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    cmpw r25, r3
    bne @L_80141BA4
    li r0, 0x1
    b @L_80141BA8
    @L_80141BA4:
    li r0, 0x0
    @L_80141BA8:
    clrlwi. r0, r0, 24
    beq @L_80141BD0
    mr r3, r22
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    cmpw r3, r26
    bge @L_80141BD0
    b @L_80141BE4
    @L_80141BD0:
    addi r23, r23, 0x1
    @L_80141BD4:
    clrlwi r0, r23, 16
    cmplw r0, r21
    blt @L_80141ADC
    li r22, 0x0
    @L_80141BE4:
    cmplwi r22, 0x0
    beq @L_80141E24
    bne @L_80141BF8
    li r20, -0x1
    b @L_80141E0C
    @L_80141BF8:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141C1C
    li r0, 0x0
    b @L_80141C34
    @L_80141C1C:
    lwz r0, lbl_80478BD8(r13)
    cmplw r25, r0
    blt @L_80141C30
    li r0, 0x0
    b @L_80141C34
    @L_80141C30:
    li r0, 0x1
    @L_80141C34:
    clrlwi. r0, r0, 24
    bne @L_80141C44
    li r20, -0x1
    b @L_80141E0C
    @L_80141C44:
    cmplwi r22, 0x0
    bne @L_80141C54
    li r0, 0x0
    b @L_80141D0C
    @L_80141C54:
    bne @L_80141C60
    li r0, 0x0
    b @L_80141CD4
    @L_80141C60:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_80141C84
    li r0, 0x0
    b @L_80141CD4
    @L_80141C84:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141CA8
    li r0, 0x0
    b @L_80141CC0
    @L_80141CA8:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_80141CBC
    li r0, 0x0
    b @L_80141CC0
    @L_80141CBC:
    li r0, 0x1
    @L_80141CC0:
    clrlwi. r0, r0, 24
    bne @L_80141CD0
    li r0, 0x0
    b @L_80141CD4
    @L_80141CD0:
    li r0, 0x1
    @L_80141CD4:
    clrlwi. r0, r0, 24
    bne @L_80141CE4
    li r0, 0x0
    b @L_80141D0C
    @L_80141CE4:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    cmpw r25, r3
    bne @L_80141D08
    li r0, 0x1
    b @L_80141D0C
    @L_80141D08:
    li r0, 0x0
    @L_80141D0C:
    clrlwi. r0, r0, 24
    bne @L_80141D1C
    li r20, -0x1
    b @L_80141E0C
    @L_80141D1C:
    cmplwi r22, 0x0
    bne @L_80141D2C
    li r20, -0x1
    b @L_80141E0C
    @L_80141D2C:
    bne @L_80141D38
    li r0, 0x0
    b @L_80141DAC
    @L_80141D38:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_80141D5C
    li r0, 0x0
    b @L_80141DAC
    @L_80141D5C:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141D80
    li r0, 0x0
    b @L_80141D98
    @L_80141D80:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_80141D94
    li r0, 0x0
    b @L_80141D98
    @L_80141D94:
    li r0, 0x1
    @L_80141D98:
    clrlwi. r0, r0, 24
    bne @L_80141DA8
    li r0, 0x0
    b @L_80141DAC
    @L_80141DA8:
    li r0, 0x1
    @L_80141DAC:
    clrlwi. r0, r0, 24
    bne @L_80141DBC
    li r20, -0x1
    b @L_80141E0C
    @L_80141DBC:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r3, r3, 16
    clrlwi r0, r24, 16
    add r0, r3, r0
    cmpw r0, r26
    ble @L_80141DF0
    subf r20, r26, r0
    mr r0, r26
    b @L_80141DF4
    @L_80141DF0:
    li r20, 0x0
    @L_80141DF4:
    clrlwi r7, r0, 16
    mr r3, r22
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142B24
    @L_80141E0C:
    cmpwi r20, 0x0
    bge @L_80141E1C
    mr r3, r20
    b @L_80142354
    @L_80141E1C:
    mr r24, r20
    b @L_801420F0
    @L_80141E24:
    cmplwi r31, 0x0
    bne @L_80141E34
    li r22, 0x0
    b @L_80141F34
    @L_80141E34:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141E58
    li r0, 0x0
    b @L_80141E70
    @L_80141E58:
    lwz r0, lbl_80478BD8(r13)
    cmplw r25, r0
    blt @L_80141E6C
    li r0, 0x0
    b @L_80141E70
    @L_80141E6C:
    li r0, 0x1
    @L_80141E70:
    clrlwi. r0, r0, 24
    bne @L_80141E80
    li r22, 0x0
    b @L_80141F34
    @L_80141E80:
    clrlwi r23, r30, 16
    li r21, 0x0
    b @L_80141F24
    @L_80141E8C:
    clrlslwi r0, r21, 16, 2
    add. r22, r31, r0
    bne @L_80141EA0
    li r0, 0x0
    b @L_80141F14
    @L_80141EA0:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r20, r3, 16
    bne @L_80141EC4
    li r0, 0x0
    b @L_80141F14
    @L_80141EC4:
    mr r4, r20
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141EE8
    li r0, 0x0
    b @L_80141F00
    @L_80141EE8:
    lwz r0, lbl_80478BD8(r13)
    cmplw r20, r0
    blt @L_80141EFC
    li r0, 0x0
    b @L_80141F00
    @L_80141EFC:
    li r0, 0x1
    @L_80141F00:
    clrlwi. r0, r0, 24
    bne @L_80141F10
    li r0, 0x0
    b @L_80141F14
    @L_80141F10:
    li r0, 0x1
    @L_80141F14:
    clrlwi. r0, r0, 24
    bne @L_80141F20
    b @L_80141F34
    @L_80141F20:
    addi r21, r21, 0x1
    @L_80141F24:
    clrlwi r0, r21, 16
    cmplw r0, r23
    blt @L_80141E8C
    li r22, 0x0
    @L_80141F34:
    cmplwi r22, 0x0
    beq @L_80142334
    bne @L_80141F48
    li r20, -0x1
    b @L_801420D8
    @L_80141F48:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141F6C
    li r0, 0x0
    b @L_80141F84
    @L_80141F6C:
    lwz r0, lbl_80478BD8(r13)
    cmplw r25, r0
    blt @L_80141F80
    li r0, 0x0
    b @L_80141F84
    @L_80141F80:
    li r0, 0x1
    @L_80141F84:
    clrlwi. r0, r0, 24
    bne @L_80141F94
    li r20, -0x1
    b @L_801420D8
    @L_80141F94:
    cmplwi r22, 0x0
    beq @L_80141FCC
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    mr r3, r22
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    @L_80141FCC:
    mr r3, r22
    mr r7, r25
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142B24
    cmplwi r22, 0x0
    bne @L_80141FF4
    li r20, -0x1
    b @L_801420D4
    @L_80141FF4:
    bne @L_80142000
    li r0, 0x0
    b @L_80142074
    @L_80142000:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_80142024
    li r0, 0x0
    b @L_80142074
    @L_80142024:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80142048
    li r0, 0x0
    b @L_80142060
    @L_80142048:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_8014205C
    li r0, 0x0
    b @L_80142060
    @L_8014205C:
    li r0, 0x1
    @L_80142060:
    clrlwi. r0, r0, 24
    bne @L_80142070
    li r0, 0x0
    b @L_80142074
    @L_80142070:
    li r0, 0x1
    @L_80142074:
    clrlwi. r0, r0, 24
    bne @L_80142084
    li r20, -0x1
    b @L_801420D4
    @L_80142084:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r3, r3, 16
    clrlwi r0, r24, 16
    add r0, r3, r0
    cmpw r0, r26
    ble @L_801420B8
    subf r20, r26, r0
    mr r0, r26
    b @L_801420BC
    @L_801420B8:
    li r20, 0x0
    @L_801420BC:
    clrlwi r7, r0, 16
    mr r3, r22
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142B24
    @L_801420D4:
    cmpwi r20, 0x0
    @L_801420D8:
    cmpwi r20, 0x0
    bge @L_801420E8
    mr r3, r20
    b @L_80142354
    @L_801420E8:
    li r29, 0x1
    mr r24, r20
    @L_801420F0:
    cmpwi r24, 0x0
    ble @L_80142334
    b @L_80141A74
    @L_801420FC:
    extsh r0, r20
    slwi r0, r0, 2
    add. r22, r31, r0
    bne @L_80142114
    li r20, -0x1
    b @L_80142330
    @L_80142114:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80142138
    li r0, 0x0
    b @L_80142154
    @L_80142138:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r27, 16
    cmplw r3, r0
    blt @L_80142150
    li r0, 0x0
    b @L_80142154
    @L_80142150:
    li r0, 0x1
    @L_80142154:
    clrlwi. r0, r0, 24
    bne @L_80142164
    li r20, -0x1
    b @L_80142330
    @L_80142164:
    cmplwi r22, 0x0
    bne @L_80142174
    li r0, 0x0
    b @L_80142230
    @L_80142174:
    bne @L_80142180
    li r0, 0x0
    b @L_801421F4
    @L_80142180:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_801421A4
    li r0, 0x0
    b @L_801421F4
    @L_801421A4:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801421C8
    li r0, 0x0
    b @L_801421E0
    @L_801421C8:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_801421DC
    li r0, 0x0
    b @L_801421E0
    @L_801421DC:
    li r0, 0x1
    @L_801421E0:
    clrlwi. r0, r0, 24
    bne @L_801421F0
    li r0, 0x0
    b @L_801421F4
    @L_801421F0:
    li r0, 0x1
    @L_801421F4:
    clrlwi. r0, r0, 24
    bne @L_80142204
    li r0, 0x0
    b @L_80142230
    @L_80142204:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r0, r27, 16
    cmpw r0, r3
    bne @L_8014222C
    li r0, 0x1
    b @L_80142230
    @L_8014222C:
    li r0, 0x0
    @L_80142230:
    clrlwi. r0, r0, 24
    bne @L_80142240
    li r20, -0x1
    b @L_80142330
    @L_80142240:
    cmplwi r22, 0x0
    bne @L_80142250
    li r20, -0x1
    b @L_80142330
    @L_80142250:
    bne @L_8014225C
    li r0, 0x0
    b @L_801422D0
    @L_8014225C:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r19, r3, 16
    bne @L_80142280
    li r0, 0x0
    b @L_801422D0
    @L_80142280:
    mr r4, r19
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_801422A4
    li r0, 0x0
    b @L_801422BC
    @L_801422A4:
    lwz r0, lbl_80478BD8(r13)
    cmplw r19, r0
    blt @L_801422B8
    li r0, 0x0
    b @L_801422BC
    @L_801422B8:
    li r0, 0x1
    @L_801422BC:
    clrlwi. r0, r0, 24
    bne @L_801422CC
    li r0, 0x0
    b @L_801422D0
    @L_801422CC:
    li r0, 0x1
    @L_801422D0:
    clrlwi. r0, r0, 24
    bne @L_801422E0
    li r20, -0x1
    b @L_80142330
    @L_801422E0:
    mr r3, r22
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r0, r3, 16
    clrlwi r3, r26, 16
    add r0, r0, r23
    cmpw r0, r3
    ble @L_80142314
    subf r20, r3, r0
    mr r0, r3
    b @L_80142318
    @L_80142314:
    li r20, 0x0
    @L_80142318:
    clrlwi r7, r0, 16
    mr r3, r22
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142B24
    @L_80142330:
    mr r24, r20
    @L_80142334:
    clrlwi r0, r29, 24
    cmplwi r0, 0x1
    bne @L_80142350
    mr r3, r31
    mr r4, r30
    mr r5, r28
    bl fn_801425E8
    @L_80142350:
    mr r3, r24
    @L_80142354:
    lmw r19, 0xc(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
