    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    li r5, 0x7a
    li r6, 0x0
    stw r0, 0x64(r1)
    li r0, 0x0
    stmw r21, 0x34(r1)
    mr r26, r4
    mr r23, r3
    li r24, 0x0
    li r4, 0x0
    stb r0, 0x8(r1)
    bl fn_8012640C
@L_8025E674:
    cmplwi r26, 0x0
    bne @L_8025E698
    mr r3, r23
    mr r7, r26
    li r4, 0x0
    li r5, 0xc6
    li r6, 0x0
    bl fn_801254B4
    b @L_8025E9A4
@L_8025E698:
    mr r3, r23
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    rlwinm r21, r3, 0, 24, 31
    cmplwi r21, 0x64
    bge @L_8025E9A4
    mr r3, r23
    li r4, 0x0
    li r5, 0x79
    li r6, 0x0
    bl fn_8012640C
    addi r0, r21, 0x1
    mr r25, r3
    mr r3, r23
    rlwinm r4, r0, 0, 24, 31
    bl fn_801229F4
    mr r0, r3
    mr r3, r23
    mr r21, r0
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    extsh r31, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x88
    li r6, 0x0
    bl fn_8012640C
    extsh r30, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x89
    li r6, 0x0
    bl fn_8012640C
    extsh r29, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x8a
    li r6, 0x0
    bl fn_8012640C
    extsh r28, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x8b
    li r6, 0x0
    bl fn_8012640C
    extsh r27, r3
    mr r3, r23
    li r4, 0x0
    li r5, 0x8c
    li r6, 0x0
    bl fn_8012640C
    add r7, r25, r26
    extsh r22, r3
    cmplw r7, r21
    blt @L_8025E988
    mr r3, r23
    mr r4, r21
    subf r26, r21, r7
    li r24, 0x1
    bl fn_8011DE98
    mr r3, r23
    bl fn_8012546C
    mr r3, r23
    bl fn_80123090
    mr r0, r3
    mr r3, r23
    mr r4, r0
    li r5, 0x0
    bl fn_80122370
    mr r3, r23
    bl fn_8011F4A8
    mr r0, r3
    li r3, 0x4ca
    mr r25, r0
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    rlwinm r4, r25, 0, 24, 31
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x44ce
    li r4, 0x1
    li r5, 0x0
    bl fn_801067E8
    mr r3, r23
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0xe(r1)
    mr r3, r23
    li r4, 0x0
    li r5, 0x88
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0x10(r1)
    mr r3, r23
    li r4, 0x0
    li r5, 0x89
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0x12(r1)
    mr r3, r23
    li r4, 0x0
    li r5, 0x8a
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0x16(r1)
    mr r3, r23
    li r4, 0x0
    li r5, 0x8b
    li r6, 0x0
    bl fn_8012640C
    sth r3, 0x18(r1)
    mr r3, r23
    li r4, 0x0
    li r5, 0x8c
    li r6, 0x0
    bl fn_8012640C
    lha r4, 0xe(r1)
    extsh r5, r3
    lha r0, 0x10(r1)
    subf r5, r22, r5
    subf r10, r31, r4
    lha r4, 0x12(r1)
    subf r9, r30, r0
    lha r0, 0x16(r1)
    subf r8, r29, r4
    lha r4, 0x18(r1)
    subf r7, r28, r0
    li r0, 0x1
    subf r6, r27, r4
    sth r3, 0x14(r1)
    addi r3, r1, 0x1c
    li r4, 0x1
    sth r10, 0x1e(r1)
    sth r9, 0x20(r1)
    sth r8, 0x22(r1)
    sth r7, 0x26(r1)
    sth r6, 0x28(r1)
    sth r5, 0x24(r1)
    stb r0, 0x1c(r1)
    bl fn_80105E7C
    li r0, 0x0
    addi r3, r1, 0xc
    stb r0, 0xc(r1)
    li r4, 0x1
    bl fn_80105E7C
    li r3, 0x1
    bl fn_80105C68
    li r0, 0x0
    stb r0, 0x8(r1)
    b @L_8025E968
@L_8025E90C:
    mr r3, r23
    mr r4, r21
    bl fn_80123B5C
    extsb r0, r3
    cmpwi r0, -0x1
    bne @L_8025E95C
    lis r4, -32730
    mr r3, r23
    addi r7, r4, -7248
    addi r5, r1, 0x9
    mr r4, r21
    li r6, 0x0
    li r8, 0x0
    bl fn_802600E4
    cmpwi r3, 0x0
    beq @L_8025E95C
    lbz r4, 0x9(r1)
    mr r3, r23
    mr r5, r21
    bl fn_80123D58
@L_8025E95C:
    lbz r3, 0x8(r1)
    addi r0, r3, 0x1
    stb r0, 0x8(r1)
@L_8025E968:
    mr r3, r23
    mr r4, r25
    addi r5, r1, 0x8
    bl fn_801236F8
    rlwinm. r0, r3, 0, 16, 31
    mr r21, r3
    bne @L_8025E90C
    b @L_8025E674
@L_8025E988:
    mr r3, r23
    li r26, 0x0
    li r4, 0x0
    li r5, 0x79
    li r6, 0x0
    bl fn_801254B4
    b @L_8025E674
@L_8025E9A4:
    mr r3, r24
    lmw r21, 0x34(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
