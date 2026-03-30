    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stmw r20, 0x50(r1)
    mr r21, r3
    mr r26, r4
    mr r27, r5
    mr r20, r6
    mr r28, r7
    mr r29, r8
    mr r30, r9
    bl fn_8019F024
    slwi r23, r20, 1
    li r3, 0x0
    li r4, 0x0
    bl fn_801AB63C
    lis r3, lbl_804016D0@ha
    lwz r31, 0x18(r21)
    addi r3, r3, lbl_804016D0@l
    addi r24, r3, 0x30
    b @L_800EACA4
    @L_800EA9B4:
    lwz r3, 0x14(r31)
    clrlwi. r0, r3, 31
    bne @L_800EACA0
    and. r0, r3, r23
    beq @L_800EACA0
    mr r3, r31
    bl fn_80199704
    lwz r25, 0xc(r31)
    b @L_800EAC98
    @L_800EA9D8:
    lhz r3, 0xc(r25)
    rlwinm. r0, r3, 0, 20, 20
    bne @L_800EAC94
    clrlwi. r0, r28, 24
    beq @L_800EAC08
    rlwinm r0, r3, 0, 18, 19
    cmpwi r0, 0x1000
    beq @L_800EAB9C
    bge @L_800EAA08
    cmpwi r0, 0x0
    beq @L_800EAA14
    b @L_800EAC08
    @L_800EAA08:
    cmpwi r0, 0x2000
    beq @L_800EABF0
    b @L_800EAC08
    @L_800EAA14:
    lwz r0, 0x14(r25)
    cmplwi r0, 0x0
    bne @L_800EAA74
    bl fn_8019F01C
    mr r21, r3
    addi r4, r1, 0x18
    addi r5, r1, 0x1c
    li r3, 0x0
    bl fn_801AB538
    lwz r0, 0x18(r1)
    cmplw r0, r21
    bne @L_800EAA50
    lwz r0, 0x1c(r1)
    cmplwi r0, 0x1
    beq @L_800EAC08
    @L_800EAA50:
    mr r4, r21
    li r3, 0x0
    li r5, 0x1
    bl fn_801AB5F8
    lis r3, lbl_804016D0@ha
    mr r4, r27
    addi r3, r3, lbl_804016D0@l
    bl fn_800E0628
    b @L_800EAC08
    @L_800EAA74:
    li r21, 0x0
    mr r22, r21
    bl fn_8019F01C
    mr r20, r3
    addi r4, r1, 0x10
    addi r5, r1, 0x14
    li r3, 0x0
    bl fn_801AB538
    lwz r0, 0x10(r1)
    cmplw r0, r20
    beq @L_800EAAB0
    lwz r0, 0x14(r1)
    cmplwi r0, 0x1
    beq @L_800EAAB0
    li r21, 0x1
    @L_800EAAB0:
    mr r4, r20
    li r3, 0x0
    li r5, 0x1
    bl fn_801AB5F8
    addi r4, r1, 0x10
    addi r5, r1, 0x14
    li r3, 0x1
    bl fn_801AB538
    lwz r0, 0x10(r1)
    lwz r4, 0x14(r25)
    cmplw r0, r4
    beq @L_800EAAF0
    lwz r0, 0x14(r1)
    cmplwi r0, 0x1
    beq @L_800EAAF0
    li r22, 0x1
    @L_800EAAF0:
    li r3, 0x1
    li r5, 0x1
    bl fn_801AB5F8
    clrlwi. r0, r21, 24
    bne @L_800EAB0C
    clrlwi. r0, r22, 24
    beq @L_800EAC08
    @L_800EAB0C:
    clrlwi. r0, r21, 24
    beq @L_800EAB24
    lis r3, lbl_804016D0@ha
    mr r4, r27
    addi r3, r3, lbl_804016D0@l
    bl fn_800E0628
    @L_800EAB24:
    clrlwi. r0, r22, 24
    beq @L_800EAC08
    lwz r20, 0x14(r25)
    cmplwi r20, 0x0
    beq @L_800EAB78
    bne @L_800EAB4C
    la r3, lbl_8047CC00(r2)
    li r4, 0x25d
    la r5, lbl_8047CC08(r2)
    bl fn_80196E10
    @L_800EAB4C:
    lwz r4, 0x14(r20)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800EAB68
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800EAB68
    li r3, 0x1
    @L_800EAB68:
    cmpwi r3, 0x0
    beq @L_800EAB78
    mr r3, r20
    bl fn_8019D9DC
    @L_800EAB78:
    lwz r4, 0x14(r25)
    mr r3, r26
    addi r5, r1, 0x20
    addi r4, r4, 0x44
    bl fn_800A2D98
    mr r3, r24
    addi r4, r1, 0x20
    bl fn_800E0628
    b @L_800EAC08
    @L_800EAB9C:
    bl fn_8019F01C
    mr r20, r3
    addi r4, r1, 0x8
    addi r5, r1, 0xc
    li r3, 0x0
    bl fn_801AB538
    lwz r0, 0x8(r1)
    cmplw r0, r20
    bne @L_800EABCC
    lwz r0, 0xc(r1)
    cmplwi r0, 0x1
    beq @L_800EAC08
    @L_800EABCC:
    mr r4, r20
    li r3, 0x0
    li r5, 0x1
    bl fn_801AB5F8
    lis r3, lbl_804016D0@ha
    mr r4, r27
    addi r3, r3, lbl_804016D0@l
    bl fn_800E0628
    b @L_800EAC08
    @L_800EABF0:
    lis r4, lbl_804016D0@ha
    mr r3, r25
    addi r6, r4, lbl_804016D0@l
    mr r5, r27
    mr r4, r26
    bl fn_800EAFE4
    @L_800EAC08:
    lhz r0, 0xc(r25)
    rlwinm r0, r0, 0, 18, 19
    cmpwi r0, 0x1000
    bne @L_800EAC3C
    mr r12, r29
    mr r3, r25
    mr r4, r26
    mr r5, r27
    mr r7, r30
    li r6, 0x0
    mtctr r12
    bctrl
    b @L_800EAC94
    @L_800EAC3C:
    cmplwi r29, 0x0
    beq @L_800EAC94
    clrlwi. r0, r28, 24
    beq @L_800EAC74
    lis r3, lbl_804016D0@ha
    mr r12, r29
    addi r6, r3, lbl_804016D0@l
    mr r4, r26
    mr r3, r25
    mr r5, r27
    mr r7, r30
    mtctr r12
    bctrl
    b @L_800EAC94
    @L_800EAC74:
    mr r12, r29
    mr r3, r25
    mr r4, r26
    mr r5, r27
    mr r7, r30
    li r6, 0x0
    mtctr r12
    bctrl
    @L_800EAC94:
    lwz r25, 0x4(r25)
    @L_800EAC98:
    cmplwi r25, 0x0
    bne @L_800EA9D8
    @L_800EACA0:
    lwz r31, 0x4(r31)
    @L_800EACA4:
    cmplwi r31, 0x0
    bne @L_800EA9B4
    li r3, 0x0
    bl fn_80199704
    li r3, 0x0
    bl fn_8019F024
    lmw r20, 0x50(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
