    nofralloc
    stwu r1, -0x50(r1)
    rlwinm. r0, r3, 0, 24, 31
    stmw r23, 0x2c(r1)
    beq @L_800B9D78
    lbz r0, 0x1(r4)
    lbz r3, 0x7(r4)
    rlwinm r30, r0, 4, 0, 27
    lbz r8, 0x0(r4)
    lbz r0, 0x13(r4)
    rlwinm r25, r3, 4, 0, 27
    lbz r10, 0x6(r4)
    rlwimi r30, r8, 0, 28, 31
    lbz r11, 0x2(r4)
    lbz r7, 0xd(r4)
    rlwimi r25, r10, 0, 28, 31
    rlwinm r27, r11, 8, 0, 23
    lbz r9, 0x8(r4)
    lbz r3, 0xe(r4)
    rlwinm r26, r9, 8, 0, 23
    lbz r28, 0x3(r4)
    rlwimi r27, r30, 0, 24, 31
    lbz r9, 0x10(r4)
    rlwinm r23, r7, 4, 0, 27
    lbz r12, 0xc(r4)
    lbz r7, 0x15(r4)
    rlwinm r24, r3, 8, 0, 23
    rlwimi r23, r12, 0, 28, 31
    lbz r29, 0x12(r4)
    rlwinm r0, r0, 4, 0, 27
    rlwimi r0, r29, 0, 28, 31
    lbz r8, 0x14(r4)
    rlwinm r28, r28, 12, 0, 19
    lbz r31, 0x9(r4)
    rlwimi r28, r27, 0, 20, 31
    lbz r29, 0x4(r4)
    rlwimi r24, r23, 0, 24, 31
    lbz r10, 0xf(r4)
    rlwinm r23, r8, 8, 0, 23
    lbz r12, 0xa(r4)
    rlwimi r26, r25, 0, 24, 31
    lbz r3, 0x16(r4)
    rlwinm r25, r10, 12, 0, 19
    lbz r30, 0x5(r4)
    rlwimi r23, r0, 0, 24, 31
    lbz r0, 0x17(r4)
    rlwinm r27, r31, 12, 0, 19
    lbz r11, 0xb(r4)
    lbz r8, 0x11(r4)
    rlwinm r4, r29, 16, 0, 15
    rlwinm r7, r7, 12, 0, 19
    rlwinm r10, r12, 16, 0, 15
    rlwimi r27, r26, 0, 20, 31
    rlwinm r12, r3, 16, 0, 15
    rlwimi r7, r23, 0, 20, 31
    rlwimi r4, r28, 0, 16, 31
    rlwinm r3, r30, 20, 0, 11
    rlwimi r25, r24, 0, 20, 31
    rlwinm r9, r9, 16, 0, 15
    rlwimi r10, r27, 0, 16, 31
    rlwimi r3, r4, 0, 12, 31
    rlwimi r12, r7, 0, 16, 31
    rlwinm r7, r3, 0, 8, 31
    rlwinm r3, r11, 20, 0, 11
    rlwimi r3, r10, 0, 12, 31
    rlwinm r4, r3, 0, 8, 31
    rlwinm r3, r8, 20, 0, 11
    rlwimi r9, r25, 0, 16, 31
    rlwimi r3, r9, 0, 12, 31
    rlwinm r0, r0, 20, 0, 11
    rlwimi r0, r12, 0, 12, 31
    rlwinm r3, r3, 0, 8, 31
    rlwinm r0, r0, 0, 8, 31
    oris r8, r7, 0x100
    oris r7, r4, 0x200
    oris r9, r3, 0x300
    oris r10, r0, 0x400
    b @L_800B9D98
@L_800B9D78:
    lis r8, 0x166
    lis r7, 0x266
    lis r4, 0x366
    lis r3, 0x466
    addi r8, r8, 0x6666
    addi r7, r7, 0x6666
    addi r9, r4, 0x6666
    addi r10, r3, 0x6666
@L_800B9D98:
    li r4, 0x61
    lis r3, 0xcc01
    stb r4, -0x8000(r3)
    rlwinm. r0, r5, 0, 24, 31
    stw r8, -0x8000(r3)
    stb r4, -0x8000(r3)
    stw r7, -0x8000(r3)
    stb r4, -0x8000(r3)
    stw r9, -0x8000(r3)
    stb r4, -0x8000(r3)
    stw r10, -0x8000(r3)
    beq @L_800B9E2C
    lbz r0, 0x0(r6)
    lbz r3, 0x1(r6)
    oris r5, r0, 0x5300
    lbz r0, 0x4(r6)
    lbz r4, 0x2(r6)
    rlwinm r7, r5, 0, 26, 19
    rlwinm r5, r3, 6, 0, 25
    lbz r3, 0x5(r6)
    or r7, r7, r5
    oris r8, r0, 0x5400
    lbz r5, 0x3(r6)
    lbz r0, 0x6(r6)
    rlwinm r6, r7, 0, 20, 13
    rlwinm r4, r4, 12, 0, 19
    or r6, r6, r4
    rlwinm r4, r8, 0, 26, 19
    rlwinm r3, r3, 6, 0, 25
    or r3, r4, r3
    rlwinm r6, r6, 0, 14, 7
    rlwinm r4, r5, 18, 0, 13
    rlwinm r3, r3, 0, 20, 13
    rlwinm r0, r0, 12, 0, 19
    or r6, r6, r4
    or r7, r3, r0
    b @L_800B9E3C
@L_800B9E2C:
    lis r4, 0x5359
    lis r3, 0x5400
    addi r6, r4, 0x5000
    addi r7, r3, 0x15
@L_800B9E3C:
    li r5, 0x61
    lwz r3, -29632(r13)
    lis r4, 0xcc01
    stb r5, -0x8000(r4)
    li r0, 0x0
    stw r6, -0x8000(r4)
    stb r5, -0x8000(r4)
    stw r7, -0x8000(r4)
    sth r0, 0x2(r3)
    lmw r23, 0x2c(r1)
    addi r1, r1, 0x50
    blr
