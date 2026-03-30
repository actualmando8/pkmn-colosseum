    nofralloc
    stwu r1, -0x160(r1)
    mflr r0
    stw r0, 0x164(r1)
    stmw r23, 0x13c(r1)
    mr r24, r3
    mr r25, r4
    mr r26, r5
    mr r27, r6
    mr r23, r7
    mr r28, r8
    mr r29, r10
    addi r30, r1, 0x8
    rlwinm r3, r9, 0, 16, 31
    bl fn_8005D858
    mr r31, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0x78
    bl memset
    lfs f0, lbl_8047CDEC(r13)
    li r0, 0x7
    stb r0, 0xc(r1)
    stw r23, 0x6c(r1)
    stfs f0, 0x70(r1)
    stfs f0, 0x74(r1)
    lbz r0, 0x0(r31)
    rlwinm r0, r0, 28, 30, 31
    cmplwi r0, 0x1
    bne @L_801041F0
    lbz r0, 0xd(r1)
    ori r0, r0, 0x1
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0xd(r1)
    lwz r0, 0x10(r31)
    stw r0, 0x60(r1)
    b @L_80104210
@L_801041F0:
    cmplwi r0, 0x2
    bne @L_80104210
    lbz r0, 0xd(r1)
    ori r0, r0, 0x2
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0xd(r1)
    lwz r0, 0x10(r31)
    stw r0, 0x10(r1)
@L_80104210:
    lbz r5, 0x5(r31)
    lis r3, 0x8081
    rlwinm r0, r29, 0, 31, 31
    lbz r4, 0x6f(r1)
    extsb r5, r5
    subi r3, r3, 0x7f7f
    add r5, r24, r5
    cmplwi r0, 0x0
    extsh r0, r5
    sth r0, 0x58(r1)
    lbz r0, 0x6(r31)
    extsb r0, r0
    sth r26, 0x5c(r1)
    add r0, r25, r0
    extsh r0, r0
    sth r27, 0x5e(r1)
    sth r0, 0x5a(r1)
    lha r0, 0x8(r31)
    sth r0, 0x64(r1)
    lha r0, 0xa(r31)
    sth r0, 0x66(r1)
    lha r0, 0xc(r31)
    sth r0, 0x68(r1)
    lha r0, 0xe(r31)
    sth r0, 0x6a(r1)
    lbz r0, 0x7(r31)
    mullw r0, r0, r4
    mulhw r3, r3, r0
    add r0, r3, r0
    srawi r0, r0, 7
    rlwinm r3, r0, 1, 31, 31
    add r0, r0, r3
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x6f(r1)
    beq @L_801042AC
    extsh r0, r26
    neg r0, r0
    extsh r0, r0
    sth r0, 0x5c(r1)
@L_801042AC:
    rlwinm r0, r29, 0, 30, 30
    cmplwi r0, 0x0
    beq @L_801042C8
    lha r0, 0x5e(r1)
    neg r0, r0
    extsh r0, r0
    sth r0, 0x5e(r1)
@L_801042C8:
    cmplwi r28, 0x0
    bne @L_801042F8
    addi r28, r1, 0x80
    li r4, 0x0
    mr r3, r28
    li r5, 0xb4
    bl memset
    addi r3, r1, 0x104
    addi r4, r1, 0x106
    bl fn_800FE6AC
    li r0, -0x1
    stw r0, 0x108(r1)
@L_801042F8:
    mr r3, r28
    mr r4, r30
    bl fn_80108C14
    lmw r23, 0x13c(r1)
    lwz r0, 0x164(r1)
    mtlr r0
    addi r1, r1, 0x160
    blr
