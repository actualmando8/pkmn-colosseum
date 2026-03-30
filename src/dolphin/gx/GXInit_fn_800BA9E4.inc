    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x60(r1)
    stmw r24, 0x40(r1)
    addi r27, r4, 0x0
    addi r28, r5, 0x0
    addi r31, r3, 0x0
    addi r29, r6, 0x0
    addi r30, r7, 0x0
    addi r24, r8, 0x0
    addi r25, r9, 0x0
    addi r26, r10, 0x0
    li r4, 0x0
    li r5, 0x20
    bl memset
    lwz r4, 0x0(r31)
    rlwinm. r0, r26, 0, 24, 31
    rlwinm r3, r25, 2, 0, 29
    rlwinm r4, r4, 0, 0, 29
    or r4, r4, r24
    stw r4, 0x0(r31)
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 0, 30, 27
    or r0, r0, r3
    stw r0, 0x0(r31)
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 0, 28, 26
    ori r0, r0, 0x10
    stw r0, 0x0(r31)
    beq @L_800BAAFC
    lbz r3, 0x1f(r31)
    subi r0, r30, 0x8
    cmplwi r0, 0x2
    ori r0, r3, 0x1
    stb r0, 0x1f(r31)
    bgt @L_800BAA88
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 0, 27, 23
    ori r0, r0, 0xa0
    stw r0, 0x0(r31)
    b @L_800BAA98
@L_800BAA88:
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 0, 27, 23
    ori r0, r0, 0xc0
    stw r0, 0x0(r31)
@L_800BAA98:
    rlwinm r3, r28, 0, 16, 31
    rlwinm r0, r29, 0, 16, 31
    cmplw r3, r0
    ble @L_800BAAB4
    cntlzw r0, r3
    subfic r0, r0, 0x1f
    b @L_800BAABC
@L_800BAAB4:
    cntlzw r0, r0
    subfic r0, r0, 0x1f
@L_800BAABC:
    stw r0, 0x3c(r1)
    lis r0, 0x4330
    lwz r3, 0x4(r31)
    stw r0, 0x38(r1)
    lfd f1, -29520(r2)
    rlwinm r3, r3, 0, 24, 15
    lfd f0, 0x38(r1)
    lfs f2, -29528(r2)
    fsubs f0, f0, f1
    fmuls f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    rlwimi r3, r0, 8, 16, 23
    stw r3, 0x4(r31)
    b @L_800BAB0C
@L_800BAAFC:
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 0, 27, 23
    ori r0, r0, 0x80
    stw r0, 0x0(r31)
@L_800BAB0C:
    stw r30, 0x14(r31)
    rlwinm r3, r29, 0, 16, 31
    rlwinm r7, r30, 0, 28, 31
    lwz r5, 0x8(r31)
    rlwinm r4, r28, 0, 16, 31
    subi r0, r3, 0x1
    rlwinm r6, r5, 0, 0, 21
    subi r5, r4, 0x1
    or r5, r6, r5
    stw r5, 0x8(r31)
    rlwinm r5, r0, 10, 0, 21
    rlwinm r0, r27, 27, 7, 31
    lwz r6, 0x8(r31)
    cmplwi r7, 0xe
    rlwinm r6, r6, 0, 22, 11
    or r5, r6, r5
    stw r5, 0x8(r31)
    lwz r5, 0x8(r31)
    rlwinm r5, r5, 0, 12, 7
    rlwimi r5, r30, 20, 8, 11
    stw r5, 0x8(r31)
    lwz r5, 0xc(r31)
    rlwinm r5, r5, 0, 0, 10
    or r0, r5, r0
    stw r0, 0xc(r31)
    bgt @L_800BABF0
    lis r5, -32719
    addi r5, r5, 13648
    rlwinm r0, r7, 2, 0, 29
    lwzx r0, r5, r0
    mtctr r0
    bctr
    li r0, 0x1
    stb r0, 0x1e(r31)
    li r0, 0x3
    li r7, 0x3
    b @L_800BAC00
    li r0, 0x2
    stb r0, 0x1e(r31)
    li r0, 0x3
    li r7, 0x2
    b @L_800BAC00
    li r0, 0x2
    stb r0, 0x1e(r31)
    li r0, 0x2
    li r7, 0x2
    b @L_800BAC00
    li r0, 0x3
    stb r0, 0x1e(r31)
    li r0, 0x2
    li r7, 0x2
    b @L_800BAC00
    li r0, 0x0
    stb r0, 0x1e(r31)
    li r0, 0x3
    li r7, 0x3
    b @L_800BAC00
@L_800BABF0:
    li r0, 0x2
    stb r0, 0x1e(r31)
    li r0, 0x2
    li r7, 0x2
@L_800BAC00:
    rlwinm r8, r0, 0, 16, 31
    li r6, 0x1
    slw r5, r6, r8
    rlwinm r7, r7, 0, 16, 31
    subi r0, r5, 0x1
    slw r5, r6, r7
    add r4, r4, r0
    subi r0, r5, 0x1
    sraw r4, r4, r8
    add r0, r3, r0
    sraw r0, r0, r7
    mullw r0, r4, r0
    rlwinm r0, r0, 0, 17, 31
    sth r0, 0x1c(r31)
    lbz r0, 0x1f(r31)
    ori r0, r0, 0x2
    stb r0, 0x1f(r31)
    lwz r0, 0x64(r1)
    lmw r24, 0x40(r1)
    addi r1, r1, 0x60
    mtlr r0
    blr
