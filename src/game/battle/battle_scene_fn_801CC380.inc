    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    lis r5, -32698
    stw r0, 0xb4(r1)
    stmw r14, 0x68(r1)
    addi r31, r5, 28904
    li r5, 0x40
    stw r3, 0x8(r1)
    mr r3, r31
    bl memcpy
    lwz r3, 0x8(r1)
    lwz r12, 0x8(r31)
    lwz r0, 0x0(r3)
    lwz r8, 0x4(r3)
    lwz r4, 0x8(r3)
    lwz r7, 0xc(r3)
    rlwinm r5, r8, 30, 2, 31
    stw r0, 0xc(r1)
    rlwimi r5, r8, 30, 0, 1
    xor r0, r4, r7
    lwz r11, 0x10(r3)
    and r3, r8, r0
    lwz r0, 0xc(r1)
    xor r3, r7, r3
    xor r8, r5, r4
    addis r9, r3, 0x5a82
    rlwinm r6, r0, 5, 27, 31
    rlwimi r6, r0, 5, 0, 26
    lwz r3, 0x0(r31)
    addi r9, r9, 0x7999
    rlwinm r0, r0, 30, 2, 31
    add r9, r9, r6
    lwz r6, 0xc(r1)
    add r11, r9, r11
    lwz r17, 0xc(r31)
    and r6, r6, r8
    lwz r18, 0x14(r31)
    xor r6, r4, r6
    add r11, r3, r11
    addis r10, r6, 0x5a82
    lwz r6, 0xc(r1)
    rlwinm r9, r11, 5, 27, 31
    lwz r16, 0x18(r31)
    rlwimi r0, r6, 30, 0, 1
    addi r10, r10, 0x7999
    xor r8, r0, r5
    rlwimi r9, r11, 5, 0, 26
    and r8, r11, r8
    rlwinm r6, r11, 30, 2, 31
    xor r8, r5, r8
    add r9, r10, r9
    addis r14, r8, 0x5a82
    lwz r8, 0x4(r31)
    add r7, r9, r7
    rlwimi r6, r11, 30, 0, 1
    add r7, r8, r7
    addi r14, r14, 0x7999
    xor r9, r6, r0
    rlwinm r11, r7, 5, 27, 31
    rlwinm r10, r7, 30, 2, 31
    rlwimi r11, r7, 5, 0, 26
    and r9, r7, r9
    add r11, r14, r11
    rlwimi r10, r7, 30, 0, 1
    xor r9, r0, r9
    add r4, r11, r4
    xor r7, r10, r6
    add r4, r12, r4
    addis r14, r9, 0x5a82
    rlwinm r11, r4, 5, 27, 31
    rlwinm r9, r4, 30, 2, 31
    addi r14, r14, 0x7999
    rlwimi r11, r4, 5, 0, 26
    and r7, r4, r7
    add r11, r14, r11
    rlwimi r9, r4, 30, 0, 1
    xor r4, r6, r7
    add r5, r11, r5
    xor r7, r9, r10
    add r5, r17, r5
    addis r15, r4, 0x5a82
    rlwinm r14, r5, 5, 27, 31
    rlwinm r4, r5, 30, 2, 31
    and r7, r5, r7
    xor r7, r10, r7
    rlwimi r14, r5, 5, 0, 26
    addis r11, r7, 0x5a82
    addi r15, r15, 0x7999
    rlwimi r4, r5, 30, 0, 1
    add r5, r15, r14
    lwz r15, 0x10(r31)
    add r0, r5, r0
    addi r11, r11, 0x7999
    add r0, r15, r0
    xor r5, r4, r9
    rlwinm r7, r0, 5, 27, 31
    rlwimi r7, r0, 5, 0, 26
    and r5, r0, r5
    add r7, r11, r7
    add r6, r7, r6
    xor r5, r9, r5
    addis r7, r5, 0x5a82
    add r6, r18, r6
    rlwinm r5, r6, 5, 27, 31
    addi r7, r7, 0x7999
    rlwimi r5, r6, 5, 0, 26
    add r5, r7, r5
    add r10, r5, r10
    add r10, r16, r10
    rlwinm r7, r0, 30, 2, 31
    rlwinm r20, r6, 30, 2, 31
    rlwimi r7, r0, 30, 0, 1
    rlwinm r5, r10, 5, 27, 31
    xor r0, r7, r4
    rlwinm r14, r10, 30, 2, 31
    and r0, r6, r0
    rlwimi r20, r6, 30, 0, 1
    xor r11, r4, r0
    rlwimi r5, r10, 5, 0, 26
    addis r6, r11, 0x5a82
    xor r0, r20, r7
    addi r6, r6, 0x7999
    rlwimi r14, r10, 30, 0, 1
    add r6, r6, r5
    and r0, r10, r0
    xor r11, r7, r0
    lwz r5, 0x1c(r31)
    add r9, r6, r9
    xor r0, r14, r20
    add r9, r5, r9
    addis r21, r11, 0x5a82
    rlwinm r19, r9, 5, 27, 31
    lwz r6, 0x20(r31)
    rlwinm r11, r9, 30, 2, 31
    addi r21, r21, 0x7999
    rlwimi r19, r9, 5, 0, 26
    and r0, r9, r0
    add r10, r21, r19
    rlwimi r11, r9, 30, 0, 1
    add r4, r10, r4
    xor r10, r20, r0
    xor r0, r11, r14
    add r4, r6, r4
    addis r21, r10, 0x5a82
    rlwinm r19, r4, 5, 27, 31
    and r0, r4, r0
    rlwinm r10, r4, 30, 2, 31
    addi r21, r21, 0x7999
    xor r9, r14, r0
    rlwimi r19, r4, 5, 0, 26
    addis r22, r9, 0x5a82
    rlwimi r10, r4, 30, 0, 1
    add r0, r21, r19
    lwz r9, 0x2c(r31)
    add r7, r0, r7
    lwz r4, 0x24(r31)
    xor r19, r10, r11
    addi r22, r22, 0x7999
    add r7, r4, r7
    stw r9, 0x14(r1)
    and r9, r7, r19
    lwz r0, 0x28(r31)
    rlwinm r21, r7, 5, 27, 31
    rlwinm r19, r7, 30, 2, 31
    rlwimi r21, r7, 5, 0, 26
    xor r9, r11, r9
    add r21, r22, r21
    rlwimi r19, r7, 30, 0, 1
    add r20, r21, r20
    addis r22, r9, 0x5a82
    add r20, r0, r20
    xor r7, r19, r10
    rlwinm r9, r20, 5, 27, 31
    addi r22, r22, 0x7999
    rlwimi r9, r20, 5, 0, 26
    rlwinm r21, r20, 30, 2, 31
    add r22, r22, r9
    and r9, r20, r7
    lwz r7, 0x14(r1)
    add r14, r22, r14
    rlwimi r21, r20, 30, 0, 1
    add r14, r7, r14
    xor r7, r10, r9
    addis r22, r7, 0x5a82
    lwz r7, 0x30(r31)
    rlwinm r20, r14, 5, 27, 31
    xor r9, r21, r19
    stw r7, 0x18(r1)
    and r7, r14, r9
    xor r9, r19, r7
    lwz r7, 0x34(r31)
    addis r9, r9, 0x5a82
    rlwimi r20, r14, 5, 0, 26
    addi r22, r22, 0x7999
    stw r7, 0x10(r1)
    add r7, r22, r20
    addi r9, r9, 0x7999
    add r11, r7, r11
    lwz r7, 0x18(r1)
    add r11, r7, r11
    rlwinm r7, r11, 5, 27, 31
    rlwimi r7, r11, 5, 0, 26
    add r7, r9, r7
    add r10, r7, r10
    lwz r7, 0x10(r1)
    add r10, r7, r10
    rlwinm r20, r14, 30, 2, 31
    rlwinm r22, r10, 5, 27, 31
    rlwimi r20, r14, 30, 0, 1
    rlwinm r9, r11, 30, 2, 31
    xor r7, r20, r21
    rlwimi r22, r10, 5, 0, 26
    and r14, r11, r7
    lwz r7, 0x38(r31)
    rlwimi r9, r11, 30, 0, 1
    stw r7, 0x1c(r1)
    xor r7, r21, r14
    addis r7, r7, 0x5a82
    xor r14, r9, r20
    addi r7, r7, 0x7999
    add r22, r7, r22
    lwz r7, 0x10(r1)
    add r19, r22, r19
    and r14, r10, r14
    xor r11, r7, r6
    lwz r7, 0x1c(r1)
    xor r11, r12, r11
    xor r22, r20, r14
    add r19, r7, r19
    lwz r7, 0x3c(r31)
    xor r11, r3, r11
    addis r22, r22, 0x5a82
    rlwinm r3, r11, 1, 31, 31
    stw r7, 0x20(r1)
    rlwimi r3, r11, 1, 0, 30
    lwz r11, 0x1c(r1)
    rlwinm r14, r19, 5, 27, 31
    rlwinm r7, r10, 30, 2, 31
    rlwimi r7, r10, 30, 0, 1
    addis r10, r3, 0x5a82
    rlwimi r14, r19, 5, 0, 26
    addi r22, r22, 0x7999
    add r14, r22, r14
    xor r11, r11, r4
    add r21, r14, r21
    xor r22, r7, r9
    xor r14, r17, r11
    lwz r11, 0x20(r1)
    xor r24, r8, r14
    rlwinm r8, r19, 30, 2, 31
    add r21, r11, r21
    and r11, r19, r22
    rlwinm r14, r21, 5, 27, 31
    lwz r22, 0x20(r1)
    rlwinm r23, r24, 1, 31, 31
    addi r10, r10, 0x7999
    rlwimi r14, r21, 5, 0, 26
    xor r22, r22, r0
    add r14, r10, r14
    rlwimi r23, r24, 1, 0, 30
    xor r22, r15, r22
    rlwimi r8, r19, 30, 0, 1
    xor r25, r12, r22
    addis r19, r23, 0x5a82
    xor r22, r8, r7
    rlwinm r12, r21, 30, 2, 31
    rlwinm r24, r25, 1, 31, 31
    xor r11, r9, r11
    rlwimi r24, r25, 1, 0, 30
    add r20, r14, r20
    add r20, r11, r20
    rlwimi r12, r21, 30, 0, 1
    rlwinm r14, r20, 5, 27, 31
    and r22, r21, r22
    addis r10, r24, 0x5a82
    xor r11, r12, r8
    and r11, r20, r11
    rlwimi r14, r20, 5, 0, 26
    addi r19, r19, 0x7999
    xor r21, r7, r22
    add r14, r19, r14
    addi r10, r10, 0x7999
    add r9, r14, r9
    stw r3, 0x0(r31)
    add r9, r21, r9
    xor r11, r8, r11
    rlwinm r14, r9, 5, 27, 31
    stw r23, 0x4(r31)
    rlwimi r14, r9, 5, 0, 26
    add r10, r10, r14
    stw r24, 0x8(r31)
    add r7, r10, r7
    add r7, r11, r7
    lwz r10, 0x14(r1)
    rlwinm r11, r20, 30, 2, 31
    rlwinm r19, r7, 5, 27, 31
    xor r10, r3, r10
    rlwimi r11, r20, 30, 0, 1
    xor r10, r18, r10
    rlwimi r19, r7, 5, 0, 26
    xor r14, r17, r10
    lwz r10, 0x18(r1)
    rlwinm r25, r14, 1, 31, 31
    xor r17, r11, r12
    xor r10, r23, r10
    rlwimi r25, r14, 1, 0, 30
    xor r10, r16, r10
    stw r25, 0xc(r31)
    xor r20, r15, r10
    addis r14, r25, 0x5a82
    and r15, r9, r17
    lwz r17, 0x10(r1)
    addi r14, r14, 0x7999
    rlwinm r26, r20, 1, 31, 31
    add r14, r14, r19
    xor r19, r24, r17
    rlwimi r26, r20, 1, 0, 30
    rlwinm r10, r9, 30, 2, 31
    xor r19, r5, r19
    add r8, r14, r8
    xor r20, r18, r19
    lwz r19, 0x1c(r1)
    lwz r14, 0x20(r1)
    rlwimi r10, r9, 30, 0, 1
    xor r19, r25, r19
    addis r17, r26, 0x6eda
    xor r19, r6, r19
    rlwinm r27, r20, 1, 31, 31
    xor r19, r16, r19
    xor r16, r12, r15
    rlwimi r27, r20, 1, 0, 30
    subi r17, r17, 0x145f
    add r8, r16, r8
    rlwinm r28, r19, 1, 31, 31
    rlwimi r28, r19, 1, 0, 30
    xor r19, r26, r14
    rlwinm r20, r8, 5, 27, 31
    addis r15, r27, 0x6eda
    xor r19, r4, r19
    rlwinm r9, r7, 30, 2, 31
    xor r19, r5, r19
    rlwimi r20, r8, 5, 0, 26
    add r5, r17, r20
    xor r18, r7, r10
    rlwimi r9, r7, 30, 0, 1
    addis r14, r28, 0x6eda
    xor r16, r8, r9
    rlwinm r7, r8, 30, 2, 31
    rlwinm r29, r19, 1, 31, 31
    xor r17, r11, r18
    add r12, r5, r12
    subi r15, r15, 0x145f
    rlwimi r29, r19, 1, 0, 30
    xor r16, r10, r16
    add r12, r17, r12
    rlwimi r7, r8, 30, 0, 1
    rlwinm r18, r12, 5, 27, 31
    addis r5, r29, 0x6eda
    rlwimi r18, r12, 5, 0, 26
    rlwinm r8, r12, 30, 2, 31
    add r15, r15, r18
    subi r14, r14, 0x145f
    add r11, r15, r11
    xor r17, r12, r7
    add r11, r16, r11
    rlwimi r8, r12, 30, 0, 1
    rlwinm r16, r11, 5, 27, 31
    xor r15, r9, r17
    rlwimi r16, r11, 5, 0, 26
    xor r12, r11, r8
    add r14, r14, r16
    subi r5, r5, 0x145f
    add r10, r14, r10
    xor r12, r7, r12
    add r10, r15, r10
    stw r26, 0x10(r31)
    rlwinm r14, r10, 5, 27, 31
    rlwimi r14, r10, 5, 0, 26
    stw r27, 0x14(r31)
    add r5, r5, r14
    add r9, r5, r9
    stw r28, 0x18(r31)
    add r9, r12, r9
    stw r29, 0x1c(r31)
    xor r12, r27, r3
    rlwinm r5, r11, 30, 2, 31
    xor r12, r0, r12
    rlwinm r15, r9, 5, 27, 31
    xor r6, r6, r12
    rlwimi r5, r11, 30, 0, 1
    rlwinm r30, r6, 1, 31, 31
    xor r12, r28, r23
    rlwimi r30, r6, 1, 0, 30
    lwz r6, 0x14(r1)
    addis r11, r30, 0x6eda
    rlwimi r15, r9, 5, 0, 26
    xor r6, r6, r12
    stw r30, 0x20(r31)
    xor r16, r4, r6
    subi r11, r11, 0x145f
    add r6, r11, r15
    xor r4, r10, r5
    add r7, r6, r7
    lwz r6, 0x18(r1)
    xor r11, r29, r24
    xor r14, r8, r4
    xor r6, r6, r11
    rlwinm r4, r10, 30, 2, 31
    xor r6, r0, r6
    add r7, r14, r7
    rlwimi r4, r10, 30, 0, 1
    rlwinm r12, r16, 1, 31, 31
    rlwimi r12, r16, 1, 0, 30
    rlwinm r11, r6, 1, 31, 31
    rlwimi r11, r6, 1, 0, 30
    rlwinm r17, r7, 5, 27, 31
    addis r16, r12, 0x6eda
    rlwinm r14, r9, 30, 2, 31
    lwz r6, 0x10(r1)
    xor r10, r30, r25
    addis r15, r11, 0x6eda
    xor r0, r9, r4
    xor r10, r6, r10
    lwz r6, 0x14(r1)
    rlwimi r14, r9, 30, 0, 1
    rlwimi r17, r7, 5, 0, 26
    xor r19, r6, r10
    subi r16, r16, 0x145f
    rlwinm r10, r19, 1, 31, 31
    xor r6, r7, r14
    rlwimi r10, r19, 1, 0, 30
    xor r19, r5, r0
    add r0, r16, r17
    stw r6, 0x24(r1)
    add r8, r0, r8
    rlwinm r21, r7, 30, 2, 31
    add r8, r19, r8
    addis r6, r10, 0x6eda
    rlwinm r0, r8, 5, 27, 31
    rlwimi r21, r7, 30, 0, 1
    rlwinm r17, r8, 30, 2, 31
    subi r15, r15, 0x145f
    rlwimi r0, r8, 5, 0, 26
    lwz r9, 0x1c(r1)
    add r15, r15, r0
    lwz r0, 0x24(r1)
    xor r18, r12, r26
    xor r16, r8, r21
    add r5, r15, r5
    xor r0, r4, r0
    xor r18, r9, r18
    lwz r9, 0x18(r1)
    add r5, r0, r5
    rlwimi r17, r8, 30, 0, 1
    rlwinm r8, r5, 5, 27, 31
    xor r18, r9, r18
    rlwinm r9, r18, 1, 31, 31
    subi r6, r6, 0x145f
    rlwimi r9, r18, 1, 0, 30
    rlwimi r8, r5, 5, 0, 26
    add r8, r6, r8
    xor r0, r5, r17
    addis r7, r9, 0x6eda
    xor r15, r14, r16
    add r4, r8, r4
    xor r6, r21, r0
    add r4, r15, r4
    subi r7, r7, 0x145f
    rlwinm r0, r4, 5, 27, 31
    stw r12, 0x24(r31)
    rlwimi r0, r4, 5, 0, 26
    add r0, r7, r0
    stw r11, 0x28(r31)
    add r14, r0, r14
    stw r10, 0x2c(r31)
    add r14, r6, r14
    stw r9, 0x30(r31)
    lwz r0, 0x20(r1)
    xor r6, r11, r27
    rlwinm r18, r5, 30, 2, 31
    rlwinm r19, r4, 30, 2, 31
    xor r7, r0, r6
    lwz r6, 0x10(r1)
    rlwinm r0, r14, 5, 27, 31
    rlwimi r18, r5, 30, 0, 1
    xor r7, r6, r7
    xor r6, r10, r28
    rlwinm r8, r7, 1, 31, 31
    rlwinm r20, r14, 30, 2, 31
    rlwimi r8, r7, 1, 0, 30
    lwz r5, 0x1c(r1)
    addis r7, r8, 0x6eda
    rlwimi r0, r14, 5, 0, 26
    subi r7, r7, 0x145f
    xor r6, r3, r6
    xor r15, r5, r6
    xor r5, r4, r18
    add r0, r7, r0
    rlwimi r19, r4, 30, 0, 1
    add r21, r0, r21
    rlwinm r6, r15, 1, 31, 31
    xor r5, r17, r5
    xor r0, r9, r29
    rlwimi r6, r15, 1, 0, 30
    xor r16, r8, r30
    add r21, r5, r21
    xor r15, r23, r0
    lwz r0, 0x20(r1)
    addis r5, r6, 0x6eda
    rlwinm r7, r21, 5, 27, 31
    rlwimi r20, r14, 30, 0, 1
    xor r15, r0, r15
    xor r0, r14, r19
    rlwinm r4, r15, 1, 31, 31
    xor r14, r21, r20
    rlwimi r4, r15, 1, 0, 30
    xor r15, r24, r16
    xor r15, r3, r15
    stw r14, 0x28(r1)
    addis r3, r4, 0x6eda
    rlwinm r22, r21, 30, 2, 31
    rlwinm r14, r15, 1, 31, 31
    rlwimi r7, r21, 5, 0, 26
    stw r14, 0x2c(r1)
    subi r5, r5, 0x145f
    add r5, r5, r7
    xor r14, r6, r12
    lwz r16, 0x2c(r1)
    rlwimi r16, r15, 1, 0, 30
    xor r14, r25, r14
    xor r0, r18, r0
    add r17, r5, r17
    stw r16, 0x2c(r1)
    add r17, r0, r17
    addis r16, r16, 0x6eda
    rlwinm r0, r17, 5, 27, 31
    rlwimi r22, r21, 30, 0, 1
    xor r14, r23, r14
    subi r3, r3, 0x145f
    rlwimi r0, r17, 5, 0, 26
    xor r15, r17, r22
    add r3, r3, r0
    lwz r0, 0x28(r1)
    rlwinm r7, r14, 1, 31, 31
    subi r16, r16, 0x145f
    rlwimi r7, r14, 1, 0, 30
    rlwinm r14, r17, 30, 2, 31
    addis r5, r7, 0x6eda
    xor r0, r19, r0
    add r18, r3, r18
    xor r21, r20, r15
    add r18, r0, r18
    rlwimi r14, r17, 30, 0, 1
    rlwinm r3, r18, 5, 27, 31
    subi r5, r5, 0x145f
    rlwimi r3, r18, 5, 0, 26
    xor r0, r18, r14
    add r15, r16, r3
    stw r8, 0x34(r31)
    add r19, r15, r19
    xor r3, r22, r0
    add r19, r21, r19
    lwz r0, 0x2c(r1)
    rlwinm r15, r19, 5, 27, 31
    stw r6, 0x38(r31)
    rlwimi r15, r19, 5, 0, 26
    stw r0, 0x0(r31)
    add r0, r5, r15
    add r20, r0, r20
    stw r4, 0x3c(r31)
    add r20, r3, r20
    stw r7, 0x4(r31)
    xor r3, r4, r11
    rlwinm r0, r18, 30, 2, 31
    xor r5, r26, r3
    rlwinm r21, r20, 30, 2, 31
    xor r16, r24, r5
    lwz r5, 0x2c(r1)
    rlwimi r0, r18, 30, 0, 1
    rlwinm r3, r20, 5, 27, 31
    xor r15, r5, r10
    rlwinm r5, r16, 1, 31, 31
    xor r15, r27, r15
    xor r17, r19, r0
    xor r18, r25, r15
    rlwimi r5, r16, 1, 0, 30
    rlwinm r15, r18, 1, 31, 31
    rlwimi r3, r20, 5, 0, 26
    addis r16, r5, 0x6eda
    stw r15, 0x30(r1)
    subi r16, r16, 0x145f
    rlwimi r21, r20, 30, 0, 1
    add r15, r16, r3
    lwz r3, 0x30(r1)
    rlwimi r3, r18, 1, 0, 30
    xor r16, r7, r9
    rlwinm r18, r19, 30, 2, 31
    add r22, r15, r22
    addis r15, r3, 0x6eda
    stw r3, 0x30(r1)
    xor r3, r28, r16
    rlwimi r18, r19, 30, 0, 1
    xor r19, r5, r8
    xor r17, r14, r17
    xor r23, r26, r3
    xor r16, r20, r18
    rlwinm r3, r23, 1, 31, 31
    xor r19, r29, r19
    rlwimi r3, r23, 1, 0, 30
    add r22, r17, r22
    xor r23, r27, r19
    subi r15, r15, 0x145f
    rlwinm r20, r23, 1, 31, 31
    rlwinm r17, r22, 5, 27, 31
    stw r20, 0x34(r1)
    rlwimi r17, r22, 5, 0, 26
    lwz r20, 0x30(r1)
    addis r25, r3, 0x6eda
    lwz r24, 0x34(r1)
    rlwimi r24, r23, 1, 0, 30
    xor r20, r20, r6
    subi r25, r25, 0x145f
    xor r20, r30, r20
    mr r23, r24
    stw r24, 0x34(r1)
    addis r24, r23, 0x6eda
    xor r23, r0, r16
    add r16, r15, r17
    add r14, r16, r14
    rlwinm r19, r22, 30, 2, 31
    add r14, r23, r14
    xor r20, r28, r20
    rlwinm r17, r14, 5, 27, 31
    rlwimi r19, r22, 30, 0, 1
    rlwimi r17, r14, 5, 0, 26
    xor r26, r22, r21
    add r17, r25, r17
    rlwinm r15, r20, 1, 31, 31
    rlwinm r23, r14, 30, 2, 31
    stw r15, 0x38(r1)
    rlwimi r15, r20, 1, 0, 30
    xor r16, r14, r19
    stw r15, 0x38(r1)
    addis r15, r15, 0x6eda
    xor r22, r18, r26
    add r0, r17, r0
    add r0, r22, r0
    rlwimi r23, r14, 30, 0, 1
    xor r20, r21, r16
    subi r24, r24, 0x145f
    rlwinm r16, r0, 5, 27, 31
    xor r14, r0, r23
    rlwimi r16, r0, 5, 0, 26
    subi r15, r15, 0x145f
    add r17, r24, r16
    stw r5, 0x8(r31)
    xor r16, r19, r14
    lwz r14, 0x30(r1)
    add r18, r17, r18
    stw r3, 0x10(r31)
    add r18, r20, r18
    stw r14, 0xc(r31)
    rlwinm r17, r18, 5, 27, 31
    lwz r14, 0x34(r1)
    rlwimi r17, r18, 5, 0, 26
    stw r14, 0x14(r31)
    add r14, r15, r17
    add r21, r14, r21
    lwz r14, 0x38(r1)
    add r21, r16, r21
    stw r14, 0x18(r31)
    xor r14, r3, r4
    rlwinm r22, r0, 30, 2, 31
    xor r15, r12, r14
    rlwinm r20, r18, 30, 2, 31
    xor r17, r29, r15
    lwz r16, 0x34(r1)
    lwz r15, 0x2c(r1)
    rlwinm r14, r21, 5, 27, 31
    rlwimi r22, r0, 30, 0, 1
    rlwimi r20, r18, 30, 0, 1
    xor r16, r16, r15
    rlwinm r15, r17, 1, 31, 31
    xor r0, r11, r16
    stw r15, 0x3c(r1)
    rlwimi r15, r17, 1, 0, 30
    xor r16, r18, r22
    xor r17, r30, r0
    stw r15, 0x3c(r1)
    rlwinm r0, r17, 1, 31, 31
    addis r15, r15, 0x6eda
    stw r0, 0x40(r1)
    rlwimi r0, r17, 1, 0, 30
    rlwimi r14, r21, 5, 0, 26
    subi r15, r15, 0x145f
    stw r0, 0x40(r1)
    add r15, r15, r14
    lwz r0, 0x38(r1)
    add r19, r15, r19
    lwz r14, 0x40(r1)
    xor r16, r23, r16
    xor r0, r0, r7
    or r15, r21, r20
    subis r14, r14, 0x70e4
    add r19, r16, r19
    xor r0, r10, r0
    lwz r16, 0x3c(r1)
    xor r18, r12, r0
    rlwinm r12, r19, 5, 27, 31
    and r25, r22, r15
    and r15, r21, r20
    xor r16, r16, r5
    rlwinm r17, r18, 1, 31, 31
    rlwimi r17, r18, 1, 0, 30
    rlwinm r0, r21, 30, 2, 31
    xor r16, r9, r16
    rlwinm r18, r19, 30, 2, 31
    subis r24, r17, 0x70e4
    rlwimi r12, r19, 5, 0, 26
    subi r14, r14, 0x4324
    xor r26, r11, r16
    add r14, r14, r12
    rlwimi r0, r21, 30, 0, 1
    or r11, r19, r0
    rlwinm r16, r26, 1, 31, 31
    rlwimi r16, r26, 1, 0, 30
    or r15, r25, r15
    add r23, r14, r23
    subi r24, r24, 0x4324
    add r23, r15, r23
    subis r12, r16, 0x70e4
    rlwinm r14, r23, 5, 27, 31
    rlwimi r18, r19, 30, 0, 1
    rlwimi r14, r23, 5, 0, 26
    and r21, r20, r11
    and r15, r19, r0
    or r11, r23, r18
    add r14, r24, r14
    subi r12, r12, 0x4324
    add r22, r14, r22
    or r15, r21, r15
    add r22, r15, r22
    and r14, r23, r18
    and r15, r0, r11
    lwz r11, 0x3c(r1)
    rlwinm r19, r22, 5, 27, 31
    stw r17, 0x24(r31)
    rlwimi r19, r22, 5, 0, 26
    or r14, r15, r14
    stw r11, 0x1c(r31)
    add r12, r12, r19
    lwz r11, 0x40(r1)
    add r20, r12, r20
    stw r16, 0x28(r31)
    add r20, r14, r20
    stw r11, 0x20(r31)
    mr r12, r11
    lwz r11, 0x30(r1)
    rlwinm r14, r20, 5, 27, 31
    rlwinm r21, r22, 30, 2, 31
    xor r12, r12, r11
    rlwinm r11, r23, 30, 2, 31
    xor r12, r8, r12
    rlwimi r14, r20, 5, 0, 26
    xor r12, r10, r12
    xor r10, r17, r3
    xor r10, r6, r10
    rlwimi r11, r23, 30, 0, 1
    xor r10, r9, r10
    lwz r9, 0x34(r1)
    rlwinm r15, r12, 1, 31, 31
    or r19, r22, r11
    rlwimi r15, r12, 1, 0, 30
    xor r9, r16, r9
    subis r12, r15, 0x70e4
    and r24, r18, r19
    subi r12, r12, 0x4324
    xor r9, r4, r9
    add r23, r12, r14
    rlwinm r14, r10, 1, 31, 31
    rlwimi r14, r10, 1, 0, 30
    xor r10, r8, r9
    lwz r8, 0x38(r1)
    subis r19, r14, 0x70e4
    and r26, r22, r11
    rlwimi r21, r22, 30, 0, 1
    xor r25, r15, r8
    lwz r8, 0x2c(r1)
    add r0, r23, r0
    or r22, r20, r21
    xor r8, r8, r25
    subi r19, r19, 0x4324
    xor r8, r6, r8
    or r6, r24, r26
    add r0, r6, r0
    rlwinm r9, r20, 30, 2, 31
    rlwimi r9, r20, 30, 0, 1
    rlwinm r12, r10, 1, 31, 31
    rlwimi r12, r10, 1, 0, 30
    rlwinm r24, r0, 5, 27, 31
    rlwimi r24, r0, 5, 0, 26
    rlwinm r6, r8, 1, 31, 31
    add r19, r19, r24
    subis r10, r12, 0x70e4
    stw r6, 0x44(r1)
    rlwimi r6, r8, 1, 0, 30
    rlwinm r8, r0, 30, 2, 31
    and r22, r11, r22
    stw r6, 0x44(r1)
    subis r6, r6, 0x70e4
    and r20, r20, r21
    add r18, r19, r18
    or r19, r22, r20
    or r23, r0, r9
    add r18, r19, r18
    subi r10, r10, 0x4324
    rlwinm r22, r18, 5, 27, 31
    and r20, r21, r23
    rlwimi r22, r18, 5, 0, 26
    and r19, r0, r9
    add r10, r10, r22
    rlwimi r8, r0, 30, 0, 1
    or r19, r20, r19
    subi r6, r6, 0x4324
    add r11, r10, r11
    or r0, r18, r8
    add r11, r19, r11
    stw r15, 0x2c(r31)
    and r10, r9, r0
    and r0, r18, r8
    rlwinm r19, r11, 5, 27, 31
    stw r14, 0x30(r31)
    rlwimi r19, r11, 5, 0, 26
    or r10, r10, r0
    add r0, r6, r19
    stw r12, 0x34(r31)
    add r21, r0, r21
    lwz r0, 0x44(r1)
    add r21, r10, r21
    stw r0, 0x38(r31)
    lwz r0, 0x3c(r1)
    rlwinm r19, r21, 5, 27, 31
    rlwinm r23, r11, 30, 2, 31
    xor r6, r14, r0
    rlwinm r0, r18, 30, 2, 31
    xor r6, r7, r6
    rlwimi r23, r11, 30, 0, 1
    xor r10, r4, r6
    lwz r4, 0x40(r1)
    rlwinm r6, r10, 1, 31, 31
    rlwimi r0, r18, 30, 0, 1
    rlwimi r6, r10, 1, 0, 30
    xor r4, r12, r4
    subis r18, r6, 0x70e4
    or r20, r11, r0
    xor r10, r5, r4
    lwz r4, 0x2c(r1)
    rlwimi r19, r21, 5, 0, 26
    subi r18, r18, 0x4324
    xor r22, r4, r10
    or r24, r21, r23
    add r10, r18, r19
    lwz r18, 0x44(r1)
    rlwinm r4, r22, 1, 31, 31
    and r25, r8, r20
    xor r19, r18, r17
    lwz r18, 0x30(r1)
    rlwimi r4, r22, 1, 0, 30
    add r9, r10, r9
    xor r18, r18, r19
    rlwinm r19, r21, 30, 2, 31
    xor r20, r7, r18
    xor r7, r6, r16
    and r18, r11, r0
    subis r22, r4, 0x70e4
    xor r7, r3, r7
    rlwimi r19, r21, 30, 0, 1
    xor r26, r5, r7
    or r5, r25, r18
    add r9, r5, r9
    rlwinm r11, r20, 1, 31, 31
    rlwimi r11, r20, 1, 0, 30
    subi r22, r22, 0x4324
    rlwinm r5, r9, 5, 27, 31
    and r24, r0, r24
    rlwimi r5, r9, 5, 0, 26
    and r21, r21, r23
    add r22, r22, r5
    subis r20, r11, 0x70e4
    rlwinm r18, r9, 30, 2, 31
    rlwinm r10, r26, 1, 31, 31
    rlwimi r10, r26, 1, 0, 30
    add r8, r22, r8
    or r21, r24, r21
    or r7, r9, r19
    add r8, r21, r8
    subis r5, r10, 0x70e4
    rlwinm r24, r8, 5, 27, 31
    subi r20, r20, 0x4324
    rlwimi r24, r8, 5, 0, 26
    rlwimi r18, r9, 30, 0, 1
    and r22, r23, r7
    and r21, r9, r19
    add r9, r20, r24
    or r7, r8, r18
    add r0, r9, r0
    or r20, r22, r21
    add r0, r20, r0
    and r9, r19, r7
    rlwinm r20, r0, 5, 27, 31
    and r7, r8, r18
    rlwimi r20, r0, 5, 0, 26
    subi r5, r5, 0x4324
    add r5, r5, r20
    stw r6, 0x3c(r31)
    or r7, r9, r7
    add r23, r5, r23
    stw r4, 0x0(r31)
    add r23, r7, r23
    stw r11, 0x4(r31)
    stw r10, 0x8(r31)
    lwz r5, 0x34(r1)
    xor r7, r4, r15
    rlwinm r20, r8, 30, 2, 31
    rlwinm r22, r23, 5, 27, 31
    xor r7, r5, r7
    lwz r5, 0x30(r1)
    rlwimi r20, r8, 30, 0, 1
    rlwinm r25, r23, 30, 2, 31
    xor r5, r5, r7
    xor r7, r11, r14
    rlwinm r9, r5, 1, 31, 31
    or r21, r0, r20
    rlwimi r9, r5, 1, 0, 30
    lwz r5, 0x38(r1)
    subis r8, r9, 0x70e4
    rlwimi r22, r23, 5, 0, 26
    xor r5, r5, r7
    xor r7, r10, r12
    xor r3, r3, r5
    subi r8, r8, 0x4324
    add r22, r8, r22
    rlwinm r5, r0, 30, 2, 31
    rlwinm r8, r3, 1, 31, 31
    and r26, r18, r21
    rlwimi r8, r3, 1, 0, 30
    lwz r3, 0x3c(r1)
    and r24, r0, r20
    rlwimi r5, r0, 30, 0, 1
    xor r7, r3, r7
    lwz r3, 0x34(r1)
    subis r21, r8, 0x70e4
    or r24, r26, r24
    xor r27, r3, r7
    lwz r3, 0x44(r1)
    rlwinm r7, r27, 1, 31, 31
    add r19, r22, r19
    rlwimi r7, r27, 1, 0, 30
    xor r28, r9, r3
    lwz r27, 0x40(r1)
    add r19, r24, r19
    rlwinm r26, r19, 5, 27, 31
    or r0, r23, r5
    xor r28, r27, r28
    lwz r27, 0x38(r1)
    subis r3, r7, 0x70e4
    rlwimi r25, r23, 30, 0, 1
    xor r27, r27, r28
    rlwimi r26, r19, 5, 0, 26
    rlwinm r22, r27, 1, 31, 31
    subi r21, r21, 0x4324
    stw r22, 0x48(r1)
    rlwimi r22, r27, 1, 0, 30
    add r26, r21, r26
    and r27, r20, r0
    stw r22, 0x48(r1)
    and r0, r23, r5
    or r24, r19, r25
    rlwinm r22, r19, 30, 2, 31
    lwz r21, 0x48(r1)
    rlwimi r22, r19, 30, 0, 1
    add r18, r26, r18
    or r0, r27, r0
    add r18, r0, r18
    subis r21, r21, 0x70e4
    rlwinm r26, r18, 5, 27, 31
    subi r3, r3, 0x4324
    rlwimi r26, r18, 5, 0, 26
    and r23, r5, r24
    and r19, r19, r25
    or r0, r18, r22
    add r3, r3, r26
    subi r21, r21, 0x4324
    add r20, r3, r20
    or r19, r23, r19
    add r20, r19, r20
    and r3, r25, r0
    rlwinm r19, r20, 5, 27, 31
    and r0, r18, r22
    rlwimi r19, r20, 5, 0, 26
    stw r9, 0xc(r31)
    or r3, r3, r0
    add r0, r21, r19
    stw r8, 0x10(r31)
    add r5, r0, r5
    lwz r0, 0x48(r1)
    stw r7, 0x14(r31)
    add r5, r3, r5
    stw r0, 0x18(r31)
    xor r0, r8, r6
    rlwinm r23, r18, 30, 2, 31
    xor r3, r17, r0
    lwz r0, 0x3c(r1)
    rlwimi r23, r18, 30, 0, 1
    rlwinm r24, r5, 5, 27, 31
    xor r0, r0, r3
    xor r3, r7, r4
    xor r18, r16, r3
    lwz r3, 0x40(r1)
    rlwinm r19, r0, 1, 31, 31
    rlwimi r24, r5, 5, 0, 26
    xor r26, r3, r18
    lwz r3, 0x48(r1)
    rlwimi r19, r0, 1, 0, 30
    or r0, r20, r23
    xor r3, r3, r11
    rlwinm r18, r26, 1, 31, 31
    subis r21, r19, 0x70e4
    and r0, r22, r0
    xor r3, r15, r3
    rlwimi r18, r26, 1, 0, 30
    xor r29, r17, r3
    subi r21, r21, 0x4324
    add r24, r21, r24
    xor r17, r19, r10
    and r28, r20, r23
    rlwinm r3, r29, 1, 31, 31
    xor r17, r14, r17
    rlwinm r21, r20, 30, 2, 31
    rlwimi r21, r20, 30, 0, 1
    stw r3, 0x4c(r1)
    subis r26, r18, 0x70e4
    xor r16, r16, r17
    lwz r20, 0x4c(r1)
    rlwimi r20, r29, 1, 0, 30
    rlwinm r3, r5, 30, 2, 31
    or r0, r0, r28
    add r25, r24, r25
    stw r20, 0x4c(r1)
    add r25, r0, r25
    rlwinm r0, r16, 1, 31, 31
    stw r0, 0x50(r1)
    rlwimi r0, r16, 1, 0, 30
    rlwinm r28, r25, 5, 27, 31
    subis r20, r20, 0x70e4
    rlwimi r3, r5, 30, 0, 1
    rlwinm r24, r25, 30, 2, 31
    or r27, r5, r21
    rlwimi r28, r25, 5, 0, 26
    subi r26, r26, 0x4324
    stw r0, 0x50(r1)
    add r0, r26, r28
    and r16, r5, r21
    lwz r5, 0x50(r1)
    and r26, r23, r27
    add r22, r0, r22
    or r17, r25, r3
    or r0, r26, r16
    subis r5, r5, 0x70e4
    add r22, r0, r22
    and r26, r21, r17
    rlwinm r16, r22, 5, 27, 31
    and r17, r25, r3
    rlwimi r24, r25, 30, 0, 1
    subi r20, r20, 0x4324
    rlwimi r16, r22, 5, 0, 26
    or r17, r26, r17
    add r16, r20, r16
    or r0, r22, r24
    add r23, r16, r23
    subi r5, r5, 0x4324
    add r23, r17, r23
    and r16, r3, r0
    rlwinm r17, r23, 5, 27, 31
    and r0, r22, r24
    rlwimi r17, r23, 5, 0, 26
    stw r19, 0x1c(r31)
    or r16, r16, r0
    add r0, r5, r17
    stw r18, 0x20(r31)
    add r21, r0, r21
    lwz r0, 0x4c(r1)
    add r21, r16, r21
    stw r0, 0x24(r31)
    lwz r0, 0x50(r1)
    stw r0, 0x28(r31)
    xor r5, r18, r9
    rlwinm r25, r21, 5, 27, 31
    xor r5, r12, r5
    rlwinm r0, r22, 30, 2, 31
    xor r16, r15, r5
    lwz r5, 0x4c(r1)
    rlwimi r0, r22, 30, 0, 1
    rlwinm r22, r23, 30, 2, 31
    xor r15, r5, r8
    lwz r5, 0x44(r1)
    rlwinm r17, r16, 1, 31, 31
    or r20, r23, r0
    rlwimi r17, r16, 1, 0, 30
    xor r5, r5, r15
    subis r16, r17, 0x70e4
    rlwimi r25, r21, 5, 0, 26
    subi r16, r16, 0x4324
    xor r5, r14, r5
    add r25, r16, r25
    and r27, r24, r20
    rlwinm r16, r5, 1, 31, 31
    and r26, r23, r0
    rlwimi r16, r5, 1, 0, 30
    lwz r5, 0x50(r1)
    rlwinm r20, r21, 30, 2, 31
    rlwimi r22, r23, 30, 0, 1
    xor r14, r5, r7
    subis r5, r16, 0x359d
    xor r14, r6, r14
    rlwimi r20, r21, 30, 0, 1
    xor r28, r12, r14
    lwz r12, 0x48(r1)
    rlwinm r15, r28, 1, 31, 31
    xor r14, r21, r22
    xor r12, r17, r12
    add r3, r25, r3
    xor r23, r4, r12
    lwz r12, 0x44(r1)
    or r26, r27, r26
    rlwimi r15, r28, 1, 0, 30
    xor r23, r12, r23
    subi r5, r5, 0x3e2a
    rlwinm r21, r23, 1, 31, 31
    add r3, r26, r3
    stw r21, 0x54(r1)
    rlwinm r27, r3, 5, 27, 31
    subis r12, r15, 0x359d
    rlwinm r21, r3, 30, 2, 31
    lwz r25, 0x54(r1)
    rlwimi r25, r23, 1, 0, 30
    rlwimi r27, r3, 5, 0, 26
    subi r12, r12, 0x3e2a
    stw r25, 0x54(r1)
    add r25, r5, r27
    add r24, r25, r24
    xor r26, r3, r20
    lwz r5, 0x54(r1)
    rlwimi r21, r3, 30, 0, 1
    stw r17, 0x2c(r31)
    subis r23, r5, 0x359d
    xor r5, r0, r14
    add r24, r5, r24
    xor r14, r22, r26
    rlwinm r25, r24, 5, 27, 31
    subi r23, r23, 0x3e2a
    rlwimi r25, r24, 5, 0, 26
    xor r3, r24, r21
    add r12, r12, r25
    rlwinm r5, r24, 30, 2, 31
    add r0, r12, r0
    stw r16, 0x30(r31)
    add r0, r14, r0
    xor r12, r20, r3
    rlwinm r14, r0, 5, 27, 31
    stw r15, 0x34(r31)
    rlwimi r14, r0, 5, 0, 26
    rlwimi r5, r24, 30, 0, 1
    add r3, r23, r14
    add r22, r3, r22
    lwz r3, 0x54(r1)
    add r22, r12, r22
    stw r3, 0x38(r31)
    xor r3, r16, r19
    rlwinm r23, r22, 5, 27, 31
    xor r12, r11, r3
    xor r12, r6, r12
    xor r3, r15, r18
    rlwinm r14, r12, 1, 31, 31
    rlwimi r23, r22, 5, 0, 26
    xor r3, r10, r3
    xor r6, r0, r5
    rlwimi r14, r12, 1, 0, 30
    xor r24, r4, r3
    xor r4, r21, r6
    subis r3, r14, 0x359d
    lwz r6, 0x54(r1)
    subi r3, r3, 0x3e2a
    rlwinm r12, r24, 1, 31, 31
    add r3, r3, r23
    stw r14, 0x3c(r31)
    add r20, r3, r20
    rlwimi r12, r24, 1, 0, 30
    add r20, r4, r20
    lwz r4, 0x4c(r1)
    rlwinm r3, r0, 30, 2, 31
    stw r12, 0x0(r31)
    xor r23, r6, r4
    subis r4, r12, 0x359d
    rlwimi r3, r0, 30, 0, 1
    rlwinm r6, r20, 5, 27, 31
    xor r23, r9, r23
    rlwinm r0, r22, 30, 2, 31
    xor r24, r11, r23
    lwz r23, 0x50(r1)
    rlwinm r11, r24, 1, 31, 31
    rlwimi r0, r22, 30, 0, 1
    rlwimi r11, r24, 1, 0, 30
    xor r26, r22, r3
    xor r23, r14, r23
    xor r25, r20, r0
    subis r24, r11, 0x359d
    rlwinm r22, r20, 30, 2, 31
    xor r23, r8, r23
    rlwimi r6, r20, 5, 0, 26
    xor r27, r10, r23
    subi r4, r4, 0x3e2a
    add r4, r4, r6
    xor r23, r12, r17
    rlwinm r10, r27, 1, 31, 31
    xor r6, r5, r26
    add r21, r4, r21
    subi r24, r24, 0x3e2a
    add r21, r6, r21
    rlwimi r22, r20, 30, 0, 1
    rlwinm r26, r21, 5, 27, 31
    rlwimi r10, r27, 1, 0, 30
    rlwimi r26, r21, 5, 0, 26
    xor r23, r7, r23
    xor r27, r9, r23
    rlwinm r20, r21, 30, 2, 31
    add r24, r24, r26
    subis r23, r10, 0x359d
    rlwinm r9, r27, 1, 31, 31
    xor r25, r3, r25
    rlwimi r9, r27, 1, 0, 30
    add r5, r24, r5
    xor r6, r21, r22
    rlwimi r20, r21, 30, 0, 1
    add r5, r25, r5
    subis r4, r9, 0x359d
    rlwinm r21, r5, 5, 27, 31
    xor r24, r0, r6
    xor r6, r5, r20
    subi r23, r23, 0x3e2a
    rlwimi r21, r5, 5, 0, 26
    subi r4, r4, 0x3e2a
    add r21, r23, r21
    xor r6, r22, r6
    add r3, r21, r3
    stw r11, 0x4(r31)
    add r3, r24, r3
    rlwinm r21, r3, 5, 27, 31
    stw r10, 0x8(r31)
    rlwimi r21, r3, 5, 0, 26
    add r4, r4, r21
    stw r9, 0xc(r31)
    add r0, r4, r0
    add r0, r6, r0
    lwz r6, 0x48(r1)
    xor r21, r11, r16
    rlwinm r4, r5, 30, 2, 31
    rlwinm r23, r0, 5, 27, 31
    xor r6, r6, r21
    xor r21, r8, r6
    rlwimi r4, r5, 30, 0, 1
    xor r6, r10, r15
    rlwimi r23, r0, 5, 0, 26
    rlwinm r8, r21, 1, 31, 31
    rlwimi r8, r21, 1, 0, 30
    xor r5, r19, r6
    xor r24, r7, r5
    stw r8, 0x10(r31)
    subis r21, r8, 0x359d
    xor r5, r3, r4
    subi r21, r21, 0x3e2a
    rlwinm r7, r24, 1, 31, 31
    xor r6, r20, r5
    add r5, r21, r23
    rlwimi r7, r24, 1, 0, 30
    add r22, r5, r22
    rlwinm r23, r3, 30, 2, 31
    rlwimi r23, r3, 30, 0, 1
    lwz r5, 0x54(r1)
    add r22, r6, r22
    rlwinm r21, r0, 30, 2, 31
    xor r26, r0, r23
    xor r5, r9, r5
    rlwimi r21, r0, 30, 0, 1
    subis r24, r7, 0x359d
    xor r0, r22, r21
    xor r6, r18, r5
    stw r0, 0x58(r1)
    rlwinm r27, r22, 5, 27, 31
    lwz r5, 0x48(r1)
    xor r28, r7, r12
    lwz r0, 0x50(r1)
    rlwimi r27, r22, 5, 0, 26
    xor r25, r5, r6
    subi r24, r24, 0x3e2a
    xor r0, r0, r28
    xor r26, r4, r26
    xor r0, r18, r0
    add r18, r24, r27
    rlwinm r6, r25, 1, 31, 31
    lwz r3, 0x4c(r1)
    rlwimi r6, r25, 1, 0, 30
    add r20, r18, r20
    subis r25, r6, 0x359d
    xor r5, r8, r14
    add r20, r26, r20
    rlwinm r24, r0, 1, 31, 31
    rlwimi r24, r0, 1, 0, 30
    xor r3, r3, r5
    xor r3, r19, r3
    rlwinm r18, r20, 5, 27, 31
    rlwinm r5, r3, 1, 31, 31
    rlwinm r19, r22, 30, 2, 31
    rlwimi r5, r3, 1, 0, 30
    rlwinm r0, r20, 30, 2, 31
    rlwimi r19, r22, 30, 0, 1
    subis r22, r24, 0x359d
    subis r3, r5, 0x359d
    rlwimi r18, r20, 5, 0, 26
    subi r25, r25, 0x3e2a
    xor r26, r20, r19
    add r25, r25, r18
    lwz r18, 0x58(r1)
    add r4, r25, r4
    rlwimi r0, r20, 30, 0, 1
    xor r18, r23, r18
    subi r3, r3, 0x3e2a
    add r4, r18, r4
    xor r25, r21, r26
    rlwinm r20, r4, 5, 27, 31
    subi r22, r22, 0x3e2a
    rlwimi r20, r4, 5, 0, 26
    xor r18, r4, r0
    add r3, r3, r20
    stw r7, 0x14(r31)
    add r23, r3, r23
    xor r18, r19, r18
    add r23, r25, r23
    stw r6, 0x18(r31)
    rlwinm r3, r23, 5, 27, 31
    rlwimi r3, r23, 5, 0, 26
    stw r5, 0x1c(r31)
    add r3, r22, r3
    add r21, r3, r21
    stw r24, 0x20(r31)
    add r21, r18, r21
    xor r3, r6, r11
    rlwinm r18, r4, 30, 2, 31
    xor r22, r17, r3
    lwz r3, 0x4c(r1)
    xor r10, r5, r10
    rlwimi r18, r4, 30, 0, 1
    xor r22, r3, r22
    rlwinm r20, r21, 5, 27, 31
    rlwinm r3, r22, 1, 31, 31
    xor r9, r24, r9
    rlwimi r3, r22, 1, 0, 30
    lwz r4, 0x50(r1)
    subis r25, r3, 0x359d
    xor r10, r16, r10
    xor r27, r4, r10
    xor r4, r23, r18
    rlwinm r22, r27, 1, 31, 31
    rlwimi r20, r21, 5, 0, 26
    subi r25, r25, 0x3e2a
    xor r26, r0, r4
    add r10, r25, r20
    rlwimi r22, r27, 1, 0, 30
    xor r9, r15, r9
    rlwinm r4, r23, 30, 2, 31
    add r19, r10, r19
    subis r20, r22, 0x359d
    xor r25, r17, r9
    rlwimi r4, r23, 30, 0, 1
    add r19, r26, r19
    xor r9, r3, r8
    rlwinm r10, r25, 1, 31, 31
    xor r17, r21, r4
    rlwimi r10, r25, 1, 0, 30
    xor r25, r22, r7
    rlwinm r24, r19, 5, 27, 31
    lwz r8, 0x54(r1)
    xor r25, r14, r25
    rlwinm r23, r21, 30, 2, 31
    xor r8, r8, r9
    subis r9, r10, 0x359d
    rlwimi r23, r21, 30, 0, 1
    rlwinm r21, r19, 30, 2, 31
    xor r26, r16, r8
    rlwimi r24, r19, 5, 0, 26
    xor r16, r19, r23
    subi r20, r20, 0x3e2a
    xor r25, r15, r25
    rlwinm r8, r26, 1, 31, 31
    add r15, r20, r24
    xor r17, r18, r17
    rlwimi r8, r26, 1, 0, 30
    rlwinm r20, r25, 1, 31, 31
    add r0, r15, r0
    subi r9, r9, 0x3e2a
    add r0, r17, r0
    rlwimi r20, r25, 1, 0, 30
    rlwinm r24, r0, 5, 27, 31
    subis r7, r8, 0x359d
    rlwimi r24, r0, 5, 0, 26
    rlwimi r21, r19, 30, 0, 1
    add r9, r9, r24
    rlwinm r15, r0, 30, 2, 31
    subis r17, r20, 0x359d
    xor r16, r4, r16
    add r18, r9, r18
    xor r19, r0, r21
    add r18, r16, r18
    rlwimi r15, r0, 30, 0, 1
    rlwinm r16, r18, 5, 27, 31
    subi r7, r7, 0x3e2a
    rlwimi r16, r18, 5, 0, 26
    xor r9, r23, r19
    add r7, r7, r16
    xor r0, r18, r15
    add r4, r7, r4
    stw r3, 0x24(r31)
    add r4, r9, r4
    xor r3, r21, r0
    rlwinm r0, r4, 5, 27, 31
    subi r17, r17, 0x3e2a
    rlwimi r0, r4, 5, 0, 26
    stw r22, 0x28(r31)
    add r0, r17, r0
    add r23, r0, r23
    stw r10, 0x2c(r31)
    add r23, r3, r23
    stw r8, 0x30(r31)
    stw r20, 0x34(r31)
    xor r0, r10, r6
    rlwinm r3, r18, 30, 2, 31
    xor r6, r12, r0
    lwz r0, 0x54(r1)
    rlwinm r9, r23, 5, 27, 31
    rlwimi r3, r18, 30, 0, 1
    xor r6, r0, r6
    xor r0, r8, r5
    rlwinm r7, r6, 1, 31, 31
    rlwimi r9, r23, 5, 0, 26
    rlwimi r7, r6, 1, 0, 30
    xor r0, r11, r0
    subis r5, r7, 0x359d
    stw r7, 0x38(r31)
    xor r10, r14, r0
    xor r0, r4, r3
    subi r5, r5, 0x3e2a
    add r5, r5, r9
    rlwinm r6, r10, 1, 31, 31
    xor r8, r15, r0
    rlwinm r0, r4, 30, 2, 31
    rlwimi r6, r10, 1, 0, 30
    add r21, r5, r21
    add r21, r8, r21
    rlwimi r0, r4, 30, 0, 1
    subis r5, r6, 0x359d
    rlwinm r9, r23, 30, 2, 31
    rlwinm r8, r21, 5, 27, 31
    xor r7, r23, r0
    rlwimi r8, r21, 5, 0, 26
    subi r5, r5, 0x3e2a
    add r4, r5, r8
    stw r6, 0x3c(r31)
    add r15, r4, r15
    xor r5, r3, r7
    lwz r4, 0xc(r1)
    add r15, r5, r15
    rlwimi r9, r23, 30, 0, 1
    add r5, r4, r15
    lwz r4, 0x8(r1)
    stw r5, 0x0(r4)
    lwz r4, 0x4(r4)
    add r5, r4, r21
    lwz r4, 0x8(r1)
    stw r5, 0x4(r4)
    lwz r4, 0x8(r4)
    add r5, r4, r9
    lwz r4, 0x8(r1)
    stw r5, 0x8(r4)
    lwz r4, 0xc(r4)
    add r0, r4, r0
    lwz r4, 0x8(r1)
    stw r0, 0xc(r4)
    lwz r0, 0x10(r4)
    add r0, r0, r3
    stw r0, 0x10(r4)
    lmw r14, 0x68(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
