    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x28(r1)
    fmr f31, f1
    stmw r25, 0xc(r1)
    mr. r25, r3
    mr r28, r4
    beq @L_801A355C
    cmplwi r25, 0x0
    beq @L_801A3314
    clrlwi. r0, r28, 31
    beq @L_801A32E0
    fmr f1, f31
    lwz r3, 0x7c(r25)
    bl fn_801C29C4
@L_801A32E0:
    lwz r0, 0x14(r25)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A3304
    fmr f1, f31
    lwz r3, 0x18(r25)
    mr r4, r28
    bl fn_801995D4
@L_801A3304:
    fmr f1, f31
    lwz r3, 0x80(r25)
    mr r4, r28
    bl fn_801AFF64
@L_801A3314:
    lwz r0, 0x14(r25)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A355C
    lwz r29, 0x10(r25)
    b @L_801A3554
@L_801A3328:
    cmplwi r29, 0x0
    beq @L_801A3550
    cmplwi r29, 0x0
    beq @L_801A3380
    clrlwi. r0, r28, 31
    beq @L_801A334C
    fmr f1, f31
    lwz r3, 0x7c(r29)
    bl fn_801C29C4
@L_801A334C:
    lwz r0, 0x14(r29)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A3370
    fmr f1, f31
    lwz r3, 0x18(r29)
    mr r4, r28
    bl fn_801995D4
@L_801A3370:
    fmr f1, f31
    lwz r3, 0x80(r29)
    mr r4, r28
    bl fn_801AFF64
@L_801A3380:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3550
    lwz r31, 0x10(r29)
    b @L_801A3548
@L_801A3394:
    cmplwi r31, 0x0
    beq @L_801A3544
    cmplwi r31, 0x0
    beq @L_801A33EC
    clrlwi. r0, r28, 31
    beq @L_801A33B8
    fmr f1, f31
    lwz r3, 0x7c(r31)
    bl fn_801C29C4
@L_801A33B8:
    lwz r0, 0x14(r31)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A33DC
    fmr f1, f31
    lwz r3, 0x18(r31)
    mr r4, r28
    bl fn_801995D4
@L_801A33DC:
    fmr f1, f31
    lwz r3, 0x80(r31)
    mr r4, r28
    bl fn_801AFF64
@L_801A33EC:
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3544
    lwz r30, 0x10(r31)
    b @L_801A353C
@L_801A3400:
    cmplwi r30, 0x0
    beq @L_801A3538
    cmplwi r30, 0x0
    beq @L_801A3458
    clrlwi. r0, r28, 31
    beq @L_801A3424
    fmr f1, f31
    lwz r3, 0x7c(r30)
    bl fn_801C29C4
@L_801A3424:
    lwz r0, 0x14(r30)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A3448
    fmr f1, f31
    lwz r3, 0x18(r30)
    mr r4, r28
    bl fn_801995D4
@L_801A3448:
    fmr f1, f31
    lwz r3, 0x80(r30)
    mr r4, r28
    bl fn_801AFF64
@L_801A3458:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3538
    lwz r27, 0x10(r30)
    b @L_801A3530
@L_801A346C:
    cmplwi r27, 0x0
    beq @L_801A352C
    cmplwi r27, 0x0
    beq @L_801A34C4
    clrlwi. r0, r28, 31
    beq @L_801A3490
    fmr f1, f31
    lwz r3, 0x7c(r27)
    bl fn_801C29C4
@L_801A3490:
    lwz r0, 0x14(r27)
    andi. r0, r0, 0x4020
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A34B4
    fmr f1, f31
    lwz r3, 0x18(r27)
    mr r4, r28
    bl fn_801995D4
@L_801A34B4:
    fmr f1, f31
    lwz r3, 0x80(r27)
    mr r4, r28
    bl fn_801AFF64
@L_801A34C4:
    lwz r0, 0x14(r27)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A352C
    lwz r25, 0x10(r27)
    b @L_801A3524
@L_801A34D8:
    cmplwi r25, 0x0
    beq @L_801A3520
    fmr f1, f31
    mr r3, r25
    mr r4, r28
    bl fn_801A3574
    lwz r0, 0x14(r25)
    rlwinm. r0, r0, 0, 19, 19
    bne @L_801A3520
    lwz r26, 0x10(r25)
    b @L_801A3518
@L_801A3504:
    fmr f1, f31
    mr r3, r26
    mr r4, r28
    bl fn_801A32A0
    lwz r26, 0x8(r26)
@L_801A3518:
    cmplwi r26, 0x0
    bne @L_801A3504
@L_801A3520:
    lwz r25, 0x8(r25)
@L_801A3524:
    cmplwi r25, 0x0
    bne @L_801A34D8
@L_801A352C:
    lwz r27, 0x8(r27)
@L_801A3530:
    cmplwi r27, 0x0
    bne @L_801A346C
@L_801A3538:
    lwz r30, 0x8(r30)
@L_801A353C:
    cmplwi r30, 0x0
    bne @L_801A3400
@L_801A3544:
    lwz r31, 0x8(r31)
@L_801A3548:
    cmplwi r31, 0x0
    bne @L_801A3394
@L_801A3550:
    lwz r29, 0x8(r29)
@L_801A3554:
    cmplwi r29, 0x0
    bne @L_801A3328
@L_801A355C:
    lfd f31, 0x28(r1)
    lmw r25, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
