    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stmw r24, 0x50(r1)
    mr. r26, r3
    lis r3, 0x8027
    addi r31, r3, 21184
    bne @L_801B0C08
    addi r3, r31, 0x0
    li r4, 0x229
    addi r5, r2, -22740
    bl fn_80196E10
@L_801B0C08:
    lwz r3, 0x8(r26)
    lwz r27, 0x58(r3)
    lwz r0, 0x0(r27)
    cmplwi r0, 0x0
    bne @L_801B0D8C
    cmplwi r26, 0x0
    lhz r28, 0x6(r27)
    lhz r29, 0x4(r27)
    bne @L_801B0C3C
    addi r3, r31, 0x0
    li r4, 0x12c
    addi r5, r2, -22740
    bl fn_80196E10
@L_801B0C3C:
    clrlwi. r0, r29, 16
    bne @L_801B0C54
    addi r3, r31, 0x0
    addi r5, r31, 0x88
    li r4, 0x12d
    bl fn_80196E10
@L_801B0C54:
    clrlwi. r0, r28, 16
    bne @L_801B0C6C
    addi r3, r31, 0x0
    addi r5, r31, 0x94
    li r4, 0x12e
    bl fn_80196E10
@L_801B0C6C:
    lwz r3, 0x8(r26)
    lwz r30, 0x58(r3)
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801B0CA0
    lhz r3, 0x4(r30)
    clrlwi r0, r29, 16
    cmplw r3, r0
    bne @L_801B0CA0
    lhz r3, 0x6(r30)
    clrlwi r0, r28, 16
    cmplw r3, r0
    beq @L_801B0D8C
@L_801B0CA0:
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801B0CE0
    lwz r3, 0x8(r26)
    lwz r25, 0x58(r3)
    lwz r0, 0x0(r25)
    cmplwi r0, 0x0
    beq @L_801B0CE0
    lwz r3, 0x0(r25)
    bl fn_800E202C
    mr r24, r3
    bl fn_800E24B0
    mr r3, r24
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0x0(r25)
@L_801B0CE0:
    mr r3, r29
    mr r4, r28
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_800BA7C0
    mr. r25, r3
    bne @L_801B0D10
    addi r3, r31, 0x0
    addi r5, r31, 0xa0
    li r4, 0x13f
    bl fn_80196E10
@L_801B0D10:
    mr r3, r25
    li r4, 0x20
    bl fn_800E2B00
    bl fn_800E27B0
    clrlwi r6, r29, 16
    lis r5, 0x4330
    stw r6, 0x44(r1)
    clrlwi r4, r28, 16
    lis r0, 0x4330
    lfd f2, -22728(r2)
    stw r5, 0x40(r1)
    lfd f4, -22728(r2)
    stw r3, 0x0(r30)
    lfd f0, 0x40(r1)
    sth r29, 0x4(r30)
    fsubs f2, f0, f2
    lfs f1, -22752(r2)
    sth r28, 0x6(r30)
    lfs f3, -22752(r2)
    stw r4, 0x4c(r1)
    lwz r3, 0x4(r26)
    stw r0, 0x48(r1)
    lfd f0, 0x48(r1)
    fsubs f4, f0, f4
    bl fn_801943E4
    lwz r3, 0x4(r26)
    mr r5, r29
    mr r7, r28
    li r4, 0x0
    li r6, 0x0
    bl fn_801944A4
@L_801B0D8C:
    lwz r3, 0x0(r27)
    li r4, 0x1
    bl fn_800B9FE4
    bl fn_800B8E74
    bl fn_800BB29C
    lwz r3, 0x4(r26)
    bl fn_801943CC
    cmpwi r3, 0x2
    beq @L_801B0DF8
    bge @L_801B0DC0
    cmpwi r3, 0x1
    bge @L_801B0DCC
    b @L_801B0E7C
@L_801B0DC0:
    cmpwi r3, 0x4
    bge @L_801B0E7C
    b @L_801B0E40
@L_801B0DCC:
    lwz r5, 0x4(r26)
    addi r3, r1, 0x10
    lwz r4, 0x4(r26)
    lfs f1, 0x40(r5)
    lfs f2, 0x44(r4)
    lfs f3, 0xc(r26)
    lfs f4, 0x10(r26)
    lfs f5, 0x14(r26)
    lfs f6, 0x18(r26)
    bl fn_800A3678
    b @L_801B0E8C
@L_801B0DF8:
    lfs f0, 0x18(r26)
    addi r3, r1, 0x10
    stfs f0, 0x8(r1)
    lwz r8, 0x4(r26)
    lwz r7, 0x4(r26)
    lwz r6, 0x4(r26)
    lwz r5, 0x4(r26)
    lwz r4, 0x4(r26)
    lfs f1, 0x40(r8)
    lfs f2, 0x44(r7)
    lfs f3, 0x48(r6)
    lfs f4, 0x4c(r5)
    lfs f5, 0x38(r4)
    lfs f6, 0xc(r26)
    lfs f7, 0x10(r26)
    lfs f8, 0x14(r26)
    bl fn_800A35E4
    b @L_801B0E8C
@L_801B0E40:
    lwz r7, 0x4(r26)
    addi r3, r1, 0x10
    lwz r6, 0x4(r26)
    lwz r5, 0x4(r26)
    lwz r4, 0x4(r26)
    lfs f1, 0x40(r7)
    lfs f2, 0x44(r6)
    lfs f3, 0x48(r5)
    lfs f4, 0x4c(r4)
    lfs f5, 0xc(r26)
    lfs f6, 0x10(r26)
    lfs f7, 0x14(r26)
    lfs f8, 0x18(r26)
    bl fn_800A3744
    b @L_801B0E8C
@L_801B0E7C:
    addi r3, r31, 0x0
    li r4, 0x305
    addi r5, r2, -22732
    bl fn_80196E10
@L_801B0E8C:
    lwz r4, 0x4(r26)
    addi r3, r1, 0x10
    lwz r5, 0x8(r26)
    addi r4, r4, 0x54
    addi r5, r5, 0x74
    bl fn_800A2D98
    lmw r24, 0x50(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
