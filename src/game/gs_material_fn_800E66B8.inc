    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r31, r3
    lhz r0, 0x152(r3)
    cmplwi r0, 0x0
    bne @L_800E67B4
    li r27, 0x0
    bl fn_800EE0E8
    mr r29, r3
    li r30, 0x0
    b @L_800E6710
    @L_800E66EC:
    mr r3, r31
    mr r4, r30
    bl fn_800EE150
    mr r28, r3
    bl fn_800EE758
    add r27, r27, r3
    mr r3, r28
    bl fn_800EE828
    addi r30, r30, 0x1
    @L_800E6710:
    cmpw r30, r29
    blt @L_800E66EC
    cmpwi r27, 0x0
    bne @L_800E6728
    li r27, 0x0
    b @L_800E67AC
    @L_800E6728:
    slwi r3, r27, 2
    bl fn_800E3534
    clrlwi. r0, r3, 16
    mr r30, r3
    beq @L_800E67AC
    bl fn_800E27B0
    stw r3, 0x14c(r31)
    mr r26, r3
    li r25, 0x0
    sth r30, 0x154(r31)
    sth r27, 0x150(r31)
    b @L_800E67A4
    @L_800E6758:
    mr r3, r31
    mr r4, r25
    bl fn_800EE150
    mr r24, r3
    bl fn_800EE758
    mr r30, r3
    li r28, 0x0
    b @L_800E6790
    @L_800E6778:
    mr r3, r24
    mr r4, r28
    bl fn_800EE6B4
    stw r3, 0x0(r26)
    addi r28, r28, 0x1
    addi r26, r26, 0x4
    @L_800E6790:
    cmpw r28, r30
    blt @L_800E6778
    mr r3, r24
    bl fn_800EE828
    addi r25, r25, 0x1
    @L_800E67A4:
    cmpw r25, r29
    blt @L_800E6758
    @L_800E67AC:
    mr r25, r27
    b @L_800E67B8
    @L_800E67B4:
    lhz r25, 0x150(r31)
    @L_800E67B8:
    lhz r3, 0x152(r31)
    li r24, 0x0
    addi r0, r3, 0x1
    sth r0, 0x152(r31)
    lwz r26, 0x14c(r31)
    b @L_800E67E8
    @L_800E67D0:
    lwz r3, 0x0(r26)
    cmplwi r3, 0x0
    beq @L_800E67E0
    bl fn_800DF188
    @L_800E67E0:
    addi r24, r24, 0x1
    addi r26, r26, 0x4
    @L_800E67E8:
    cmpw r24, r25
    blt @L_800E67D0
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
