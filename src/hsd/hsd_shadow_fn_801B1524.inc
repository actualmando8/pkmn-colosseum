    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr. r27, r3
    lis r3, 0x8027
    mr r28, r4
    addi r31, r3, 21184
    mr r29, r5
    bne @L_801B155C
    addi r3, r31, 0x0
    li r4, 0x12c
    addi r5, r2, -22740
    bl fn_80196E10
@L_801B155C:
    clrlwi. r0, r28, 16
    bne @L_801B1574
    addi r3, r31, 0x0
    addi r5, r31, 0x88
    li r4, 0x12d
    bl fn_80196E10
@L_801B1574:
    clrlwi. r0, r29, 16
    bne @L_801B158C
    addi r3, r31, 0x0
    addi r5, r31, 0x94
    li r4, 0x12e
    bl fn_80196E10
@L_801B158C:
    lwz r3, 0x8(r27)
    lwz r30, 0x58(r3)
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801B15C0
    lhz r3, 0x4(r30)
    clrlwi r0, r28, 16
    cmplw r3, r0
    bne @L_801B15C0
    lhz r3, 0x6(r30)
    clrlwi r0, r29, 16
    cmplw r3, r0
    beq @L_801B16AC
@L_801B15C0:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801B1600
    lwz r3, 0x8(r27)
    lwz r26, 0x58(r3)
    lwz r0, 0x0(r26)
    cmplwi r0, 0x0
    beq @L_801B1600
    lwz r3, 0x0(r26)
    bl fn_800E202C
    mr r25, r3
    bl fn_800E24B0
    mr r3, r25
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0x0(r26)
@L_801B1600:
    mr r3, r28
    mr r4, r29
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_800BA7C0
    mr. r25, r3
    bne @L_801B1630
    addi r3, r31, 0x0
    addi r5, r31, 0xa0
    li r4, 0x13f
    bl fn_80196E10
@L_801B1630:
    mr r3, r25
    li r4, 0x20
    bl fn_800E2B00
    bl fn_800E27B0
    clrlwi r6, r28, 16
    lis r5, 0x4330
    stw r6, 0xc(r1)
    clrlwi r4, r29, 16
    lis r0, 0x4330
    lfd f2, -22728(r2)
    stw r5, 0x8(r1)
    lfd f4, -22728(r2)
    stw r3, 0x0(r30)
    lfd f0, 0x8(r1)
    sth r28, 0x4(r30)
    fsubs f2, f0, f2
    lfs f1, -22752(r2)
    sth r29, 0x6(r30)
    lfs f3, -22752(r2)
    stw r4, 0x14(r1)
    lwz r3, 0x4(r27)
    stw r0, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f4, f0, f4
    bl fn_801943E4
    lwz r3, 0x4(r27)
    mr r5, r28
    mr r7, r29
    li r4, 0x0
    li r6, 0x0
    bl fn_801944A4
@L_801B16AC:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
