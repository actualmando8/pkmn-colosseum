    nofralloc
    stwu r1, -0x188(r1)
    lis r4, -32728
    addi r4, r4, -25880
    stmw r21, 0x15c(r1)
    addi r6, r4, 0x50
    lwz r3, -21104(r13)
    lwz r5, 0x69c(r3)
    addi r8, r3, 0x69c
    addi r3, r1, 0x14
    lbz r7, 0x0(r5)
    addi r0, r5, 0x2
    lbz r5, 0x1(r5)
    rlwimi r5, r7, 8, 16, 23
    stw r0, 0x0(r8)
    clrlwi r7, r5, 16
    lis r0, 0x4330
    lfd f0, -20976(r2)
    subi r7, r7, 0x2
@L_801E5A70:
    lwz r11, -21104(r13)
    addi r5, r4, 0x0
    li r9, 0x0
    lwz r10, 0x69c(r11)
    addi r8, r10, 0x1
    stw r8, 0x69c(r11)
    lbz r8, 0x0(r10)
    b @L_801E5C50
@L_801E5A90:
    lwz r30, -21104(r13)
    addi r28, r9, 0x1
    addi r29, r9, 0x2
    lwz r27, 0x69c(r30)
    addi r12, r9, 0x3
    addi r11, r9, 0x4
    addi r10, r27, 0x1
    stw r10, 0x69c(r30)
    addi r10, r9, 0x5
    clrlwi r28, r28, 16
    lbz r30, 0x0(r27)
    clrlwi r29, r29, 16
    lbz r27, 0x0(r5)
    clrlwi r12, r12, 16
    stw r30, 0x154(r1)
    slwi r27, r27, 2
    stw r0, 0x150(r1)
    clrlwi r11, r11, 16
    clrlwi r10, r10, 16
    lfd f1, 0x150(r1)
    fsubs f1, f1, f0
    stfsx f1, r3, r27
    lwz r27, -21104(r13)
    lwz r31, 0x69c(r27)
    addi r30, r31, 0x1
    stw r30, 0x69c(r27)
    lbz r30, 0x0(r31)
    lbzx r28, r4, r28
    stw r30, 0x14c(r1)
    slwi r28, r28, 2
    stw r0, 0x148(r1)
    lfd f1, 0x148(r1)
    fsubs f1, f1, f0
    stfsx f1, r3, r28
    lwz r28, -21104(r13)
    lwz r31, 0x69c(r28)
    addi r30, r31, 0x1
    stw r30, 0x69c(r28)
    lbz r30, 0x0(r31)
    lbzx r29, r4, r29
    stw r30, 0x144(r1)
    slwi r29, r29, 2
    stw r0, 0x140(r1)
    lfd f1, 0x140(r1)
    fsubs f1, f1, f0
    stfsx f1, r3, r29
    lwz r31, -21104(r13)
    lwz r30, 0x69c(r31)
    addi r29, r30, 0x1
    stw r29, 0x69c(r31)
    lbz r29, 0x0(r30)
    lbzx r12, r4, r12
    stw r29, 0x13c(r1)
    slwi r12, r12, 2
    stw r0, 0x138(r1)
    lfd f1, 0x138(r1)
    fsubs f1, f1, f0
    stfsx f1, r3, r12
    lwz r30, -21104(r13)
    lwz r29, 0x69c(r30)
    addi r12, r29, 0x1
    stw r12, 0x69c(r30)
    lbz r12, 0x0(r29)
    lbzx r11, r4, r11
    stw r12, 0x134(r1)
    slwi r11, r11, 2
    stw r0, 0x130(r1)
    lfd f1, 0x130(r1)
    fsubs f1, f1, f0
    stfsx f1, r3, r11
    lwz r29, -21104(r13)
    lwz r12, 0x69c(r29)
    addi r11, r12, 0x1
    stw r11, 0x69c(r29)
    lbz r11, 0x0(r12)
    lbzx r10, r4, r10
    stw r11, 0x12c(r1)
    slwi r10, r10, 2
    stw r0, 0x128(r1)
    lfd f1, 0x128(r1)
    fsubs f1, f1, f0
    stfsx f1, r3, r10
    lwz r30, -21104(r13)
    addi r11, r9, 0x6
    addi r10, r9, 0x7
    lwz r29, 0x69c(r30)
    clrlwi r11, r11, 16
    clrlwi r10, r10, 16
    addi r12, r29, 0x1
    stw r12, 0x69c(r30)
    addi r5, r5, 0x8
    addi r9, r9, 0x8
    lbz r12, 0x0(r29)
    lbzx r11, r4, r11
    stw r12, 0x124(r1)
    slwi r11, r11, 2
    stw r0, 0x120(r1)
    lfd f1, 0x120(r1)
    fsubs f1, f1, f0
    stfsx f1, r3, r11
    lwz r29, -21104(r13)
    lwz r12, 0x69c(r29)
    addi r11, r12, 0x1
    stw r11, 0x69c(r29)
    lbz r11, 0x0(r12)
    lbzx r10, r4, r10
    stw r11, 0x11c(r1)
    slwi r10, r10, 2
    stw r0, 0x118(r1)
    lfd f1, 0x118(r1)
    fsubs f1, f1, f0
    stfsx f1, r3, r10
@L_801E5C50:
    clrlwi r10, r9, 16
    cmplwi r10, 0x40
    blt @L_801E5A90
    lwz r29, -21104(r13)
    addi r27, r6, 0x0
    slwi r28, r8, 8
    li r5, 0x0
    li r8, 0x0
    b @L_801E5DBC
@L_801E5C74:
    clrlslwi r11, r5, 16, 2
    lfd f1, 0x0(r27)
    lfsx f2, r3, r11
    addi r9, r5, 0x1
    clrlslwi r12, r9, 16, 2
    lfd f3, 0x0(r6)
    fmul f1, f2, f1
    addi r9, r5, 0x2
    clrlslwi r26, r9, 16, 2
    addi r9, r5, 0x3
    fmul f1, f3, f1
    clrlslwi r25, r9, 16, 2
    addi r10, r5, 0x4
    clrlslwi r24, r10, 16, 2
    frsp f1, f1
    add r9, r29, r11
    addi r10, r5, 0x7
    stfsx f1, r28, r9
    addi r9, r5, 0x5
    clrlslwi r23, r9, 16, 2
    lfsx f2, r3, r12
    addi r9, r5, 0x6
    lfd f1, 0x0(r27)
    clrlslwi r22, r9, 16, 2
    add r9, r29, r12
    fmul f1, f2, f1
    lfd f3, 0x8(r6)
    clrlslwi r21, r10, 16, 2
    add r30, r29, r26
    add r31, r29, r25
    fmul f1, f3, f1
    add r12, r29, r24
    add r11, r29, r23
    add r10, r29, r22
    addi r5, r5, 0x8
    frsp f1, f1
    addi r8, r8, 0x1
    stfsx f1, r28, r9
    add r9, r29, r21
    lfsx f2, r3, r26
    lfd f1, 0x0(r27)
    lfd f3, 0x10(r6)
    fmul f1, f2, f1
    fmul f1, f3, f1
    frsp f1, f1
    stfsx f1, r28, r30
    lfsx f2, r3, r25
    lfd f1, 0x0(r27)
    lfd f3, 0x18(r6)
    fmul f1, f2, f1
    fmul f1, f3, f1
    frsp f1, f1
    stfsx f1, r28, r31
    lfsx f2, r3, r24
    lfd f1, 0x0(r27)
    lfd f3, 0x20(r6)
    fmul f1, f2, f1
    fmul f1, f3, f1
    frsp f1, f1
    stfsx f1, r28, r12
    lfsx f2, r3, r23
    lfd f1, 0x0(r27)
    lfd f3, 0x28(r6)
    fmul f1, f2, f1
    fmul f1, f3, f1
    frsp f1, f1
    stfsx f1, r28, r11
    lfsx f2, r3, r22
    lfd f1, 0x0(r27)
    lfd f3, 0x30(r6)
    fmul f1, f2, f1
    fmul f1, f3, f1
    frsp f1, f1
    stfsx f1, r28, r10
    lfd f1, 0x0(r27)
    addi r27, r27, 0x8
    lfsx f2, r3, r21
    lfd f3, 0x38(r6)
    fmul f1, f2, f1
    fmul f1, f3, f1
    frsp f1, f1
    stfsx f1, r28, r9
@L_801E5DBC:
    clrlwi r9, r8, 16
    cmplwi r9, 0x8
    blt @L_801E5C74
    subi r7, r7, 0x41
    clrlwi. r5, r7, 16
    bne @L_801E5A70
    li r3, 0x0
    lmw r21, 0x15c(r1)
    addi r1, r1, 0x188
    blr
