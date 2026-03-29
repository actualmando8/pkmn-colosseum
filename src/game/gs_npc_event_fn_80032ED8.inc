    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    mr r3, r30
    bl fn_8020E0F8
    lwz r0, 0x1c(r29)
    mr r26, r3
    cmpwi r0, 0x0
    bne @L_80032F18
    li r4, 0x0
    bl fn_8020DFA0
    b @L_80032F20
    @L_80032F18:
    li r4, 0x1
    bl fn_8020DFA0
    @L_80032F20:
    lwz r0, lbl_8047A444(r13)
    mr r3, r26
    clrlwi r4, r0, 16
    bl fn_8020DF90
    li r3, 0x9
    bl fn_801FCCC4
    lbz r4, 0xc(r29)
    mr r27, r3
    bl fn_801FCB94
    mr r3, r27
    li r4, 0x26
    bl fn_801FCC54
    lhz r4, 0x20(r29)
    mr r3, r27
    bl fn_801FCB84
    lbz r4, 0x24(r29)
    mr r3, r27
    bl fn_801FCAFC
    mr r3, r29
    bl fn_800896E0
    li r3, 0x0
    bl fn_800896D8
    li r3, 0x9
    bl fn_800896D0
    mr r28, r29
    li r26, 0x0
    @L_80032F88:
    lhz r5, 0x12(r28)
    mr r3, r27
    clrlwi r4, r26, 24
    bl fn_801FCB40
    addi r28, r28, 0x2
    addi r26, r26, 0x1
    cmpwi r26, 0x4
    blt @L_80032F88
    mr r3, r27
    bl fn_801FCC3C
    bl fn_801FCA2C
    mr r25, r3
    mr r27, r31
    li r24, 0x0
    mr r26, r25
    lis r3, lbl_803A3334@ha
    addi r28, r3, lbl_803A3334@l
    @L_80032FCC:
    cmplwi r31, 0x0
    beq @L_80032FE4
    mr r3, r26
    mr r4, r27
    bl fn_800330B8
    b @L_80033004
    @L_80032FE4:
    addi r0, r24, 0xd
    mr r3, r26
    lbzx r0, r29, r0
    extsb r0, r0
    mulli r0, r0, 0x2a
    add r4, r28, r0
    addi r4, r4, 0x514
    bl fn_800330B8
    @L_80033004:
    addi r27, r27, 0x2a
    addi r26, r26, 0x50
    addi r24, r24, 0x1
    cmpwi r24, 0x4
    blt @L_80032FCC
    mulli r0, r24, 0x50
    add r26, r25, r0
    b @L_80033038
    @L_80033024:
    mr r3, r26
    li r4, 0x0
    bl fn_801FC794
    addi r26, r26, 0x50
    addi r24, r24, 0x1
    @L_80033038:
    cmpwi r24, 0x6
    blt @L_80033024
    mr r3, r30
    li r4, 0x1
    li r5, 0x0
    bl fn_801CA5C4
    mr r26, r3
    bl fn_80113F48
    lis r4, 0x1117
    li r5, 0x0
    addi r4, r4, 0x1800
    li r6, 0x0
    bl fn_80176E0C
    lbz r0, lbl_8047A439(r13)
    cmplwi r0, 0x0
    beq @L_80033080
    li r28, 0x446
    b @L_80033084
    @L_80033080:
    li r28, 0x4cd
    @L_80033084:
    bl fn_801653C4
    cmplw r28, r3
    beq @L_800330A0
    mr r3, r28
    li r4, 0x0
    li r5, 0x7f
    bl fn_80165A20
    @L_800330A0:
    mr r3, r26
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
