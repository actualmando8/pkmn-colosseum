    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr. r27, r3
    lis r3, 0x8027
    mr r25, r4
    addi r31, r3, 21184
    bne @L_801B08B4
    addi r3, r31, 0x0
    li r4, 0x278
    addi r5, r2, -22740
    bl fn_80196E10
@L_801B08B4:
    lwz r0, 0x1c(r27)
    cmpwi r0, 0x0
    beq @L_801B08C8
    cmpwi r25, 0x0
    bne @L_801B0A84
@L_801B08C8:
    lwz r0, 0x1c(r27)
    cmpwi r0, 0x0
    bne @L_801B08E0
    cmpwi r25, 0x0
    bne @L_801B08E0
    b @L_801B0A84
@L_801B08E0:
    cmpwi r25, 0x0
    stw r25, 0x1c(r27)
    beq @L_801B0A7C
    lwz r3, 0x8(r27)
    lwz r3, 0x58(r3)
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_801B0A70
    cmplwi r27, 0x0
    lhz r28, 0x6(r3)
    lhz r29, 0x4(r3)
    bne @L_801B0920
    addi r3, r31, 0x0
    li r4, 0x12c
    addi r5, r2, -22740
    bl fn_80196E10
@L_801B0920:
    clrlwi. r0, r29, 16
    bne @L_801B0938
    addi r3, r31, 0x0
    addi r5, r31, 0x88
    li r4, 0x12d
    bl fn_80196E10
@L_801B0938:
    clrlwi. r0, r28, 16
    bne @L_801B0950
    addi r3, r31, 0x0
    addi r5, r31, 0x94
    li r4, 0x12e
    bl fn_80196E10
@L_801B0950:
    lwz r3, 0x8(r27)
    lwz r30, 0x58(r3)
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801B0984
    lhz r3, 0x4(r30)
    clrlwi r0, r29, 16
    cmplw r3, r0
    bne @L_801B0984
    lhz r3, 0x6(r30)
    clrlwi r0, r28, 16
    cmplw r3, r0
    beq @L_801B0A70
@L_801B0984:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801B09C4
    lwz r3, 0x8(r27)
    lwz r26, 0x58(r3)
    lwz r0, 0x0(r26)
    cmplwi r0, 0x0
    beq @L_801B09C4
    lwz r3, 0x0(r26)
    bl fn_800E202C
    mr r25, r3
    bl fn_800E24B0
    mr r3, r25
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0x0(r26)
@L_801B09C4:
    mr r3, r29
    mr r4, r28
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_800BA7C0
    mr. r26, r3
    bne @L_801B09F4
    addi r3, r31, 0x0
    addi r5, r31, 0xa0
    li r4, 0x13f
    bl fn_80196E10
@L_801B09F4:
    mr r3, r26
    li r4, 0x20
    bl fn_800E2B00
    bl fn_800E27B0
    clrlwi r6, r29, 16
    lis r5, 0x4330
    stw r6, 0xc(r1)
    clrlwi r4, r28, 16
    lis r0, 0x4330
    lfd f2, -22728(r2)
    stw r5, 0x8(r1)
    lfd f4, -22728(r2)
    stw r3, 0x0(r30)
    lfd f0, 0x8(r1)
    sth r29, 0x4(r30)
    fsubs f2, f0, f2
    lfs f1, -22752(r2)
    sth r28, 0x6(r30)
    lfs f3, -22752(r2)
    stw r4, 0x14(r1)
    lwz r3, 0x4(r27)
    stw r0, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f4, f0, f4
    bl fn_801943E4
    lwz r3, 0x4(r27)
    mr r5, r29
    mr r7, r28
    li r4, 0x0
    li r6, 0x0
    bl fn_801944A4
@L_801B0A70:
    lwz r3, 0x8(r27)
    bl fn_801A6CA4
    b @L_801B0A84
@L_801B0A7C:
    lwz r3, 0x8(r27)
    bl fn_801A6C34
@L_801B0A84:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
