    nofralloc
    stwu r1, -0x8d0(r1)
    mflr r0
    stw r0, 0x8d4(r1)
    stmw r14, 0x888(r1)
    stw r3, 0x8(r1)
    mr r15, r4
    mr r17, r5
    stw r6, 0xc(r1)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r24, r3, 0, 16, 31
    mr r3, r24
    bl fn_8020E204
    mr r18, r3
    bl fn_8020E1D4
    mr r19, r3
    mr r3, r18
    bl fn_8020E1BC
    mr r16, r3
    mr r3, r18
    bl fn_8020E1A4
    mr r18, r3
    rlwinm r3, r19, 0, 24, 31
    rlwinm r0, r18, 0, 24, 31
    mr r4, r15
    mullw r0, r3, r0
    mr r5, r24
    li r3, 0xb
    rlwinm r0, r0, 1, 0, 30
    rlwinm r23, r0, 0, 24, 31
    bl fn_801F02AC
    li r4, 0x0
    mr r20, r3
    bl fn_801F981C
    rlwinm r0, r19, 0, 24, 31
    li r4, 0x1
    cmplwi r0, 0x2
    stw r3, 0x28(r1)
    stb r4, 0x24(r1)
    bne @L_8008A0AC
    mr r4, r20
    mr r5, r24
    li r3, 0x7
    bl fn_801F02AC
    li r4, 0x0
    mr r19, r3
    bl fn_801F981C
    stw r3, 0x2c(r1)
    mr r3, r19
    bl fn_801FCDB4
    stb r3, 0x25(r1)
    mr r4, r20
    mr r5, r24
    li r3, 0x9
    bl fn_801F02AC
    li r4, 0x0
    mr r19, r3
    bl fn_801F981C
    stw r3, 0x30(r1)
    mr r3, r19
    bl fn_801FCDB4
    stb r3, 0x26(r1)
    mr r4, r20
    mr r5, r24
    li r3, 0xa
    bl fn_801F02AC
    li r4, 0x0
    mr r19, r3
    bl fn_801F981C
    stw r3, 0x34(r1)
    mr r3, r19
    bl fn_801FCDB4
    stb r3, 0x27(r1)
    b @L_8008A134
@L_8008A0AC:
    rlwinm r0, r18, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8008A110
    mr r3, r20
    li r4, 0x1
    bl fn_801F981C
    stw r3, 0x2c(r1)
    mr r4, r20
    mr r5, r24
    li r3, 0x9
    bl fn_801F02AC
    li r4, 0x0
    mr r19, r3
    bl fn_801F981C
    stw r3, 0x30(r1)
    mr r3, r19
    li r4, 0x1
    bl fn_801F981C
    li r0, 0x2
    li r4, 0x1
    stw r3, 0x34(r1)
    stb r4, 0x25(r1)
    stb r0, 0x26(r1)
    stb r0, 0x27(r1)
    b @L_8008A134
@L_8008A110:
    mr r4, r20
    mr r5, r24
    li r3, 0x9
    bl fn_801F02AC
    li r4, 0x0
    bl fn_801F981C
    li r0, 0x1
    stw r3, 0x2c(r1)
    stb r0, 0x25(r1)
@L_8008A134:
    mr r3, r15
    rlwinm r4, r17, 0, 16, 31
    bl fn_801F981C
    rlwinm r0, r18, 0, 24, 31
    mr r22, r3
    cmplwi r0, 0x2
    bne @L_8008A158
    addi r0, r17, 0x1
    b @L_8008A15C
@L_8008A158:
    li r0, 0x0
@L_8008A15C:
    rlwinm r0, r0, 0, 24, 31
    lbz r4, 0x3b(r1)
    rlwimi r4, r0, 0, 25, 31
    lwz r0, 0xc(r1)
    addi r25, r1, 0x38
    stb r4, 0x3b(r1)
    rlwinm r3, r0, 0, 24, 31
    rlwinm r0, r4, 0, 24, 31
    rlwimi r0, r3, 7, 24, 24
    stb r23, 0x3f(r1)
    mr r27, r25
    rlwinm r26, r16, 0, 24, 31
    stb r0, 0x3b(r1)
    li r19, 0x0
    li r18, 0x0
    li r21, 0x0
    li r28, 0x8
    b @L_8008A364
@L_8008A1A4:
    mr r3, r15
    rlwinm r4, r21, 0, 16, 31
    bl fn_801F986C
    mr r29, r3
    cmplwi r29, 0x0
    beq @L_8008A36C
    bl fn_80205BE8
    mr r17, r3
    bl fn_8011E7C0
    rlwinm r0, r3, 0, 16, 31
    mr r3, r15
    slw r0, r0, r28
    mr r4, r29
    or r19, r19, r0
    bl fn_801F8C00
    rlwinm r0, r3, 0, 24, 31
    mr r3, r17
    slw r0, r0, r28
    or r18, r18, r0
    bl fn_8011E8F4
    mr r16, r3
    mr r3, r17
    bl fn_8011F188
    rlwinm r0, r3, 0, 16, 31
    mr r3, r17
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x24(r27)
    bl fn_8008C5D4
    rlwinm r20, r3, 0, 16, 31
    mr r3, r29
    rlwinm r0, r20, 0, 24, 31
    stb r0, 0x26(r27)
    bl fn_802042E0
    rlwinm r5, r3, 0, 16, 31
    lbz r0, 0x27(r27)
    neg r4, r5
    mr r3, r17
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 7, 24, 24
    stb r0, 0x27(r27)
    bl fn_8011E808
    rlwinm r4, r3, 0, 24, 31
    rlwinm r7, r16, 0, 24, 31
    subfic r3, r4, 0xff
    lbz r6, 0x27(r27)
    subi r0, r4, 0xff
    rlwinm r5, r7, 0, 28, 31
    or r0, r3, r0
    rlwinm r3, r7, 0, 24, 27
    rlwinm r0, r0, 1, 31, 31
    neg r4, r5
    rlwimi r6, r0, 6, 25, 25
    neg r0, r3
    stb r6, 0x27(r27)
    or r4, r4, r5
    rlwinm r5, r4, 1, 31, 31
    or r3, r0, r3
    lbz r4, 0x27(r27)
    rlwimi r4, r5, 4, 27, 27
    srawi r0, r20, 8
    rlwinm r5, r3, 1, 31, 31
    stb r4, 0x27(r27)
    rlwinm r4, r0, 0, 24, 31
    mr r3, r29
    lbz r0, 0x27(r27)
    rlwimi r0, r5, 5, 26, 26
    stb r0, 0x27(r27)
    lbz r0, 0x27(r27)
    rlwimi r0, r4, 0, 28, 31
    stb r0, 0x27(r27)
    bl fn_802042E0
    rlwinm r0, r3, 0, 16, 31
    mr r31, r27
    rlwinm r3, r0, 8, 0, 23
    mr r30, r27
    srawi r0, r0, 8
    mr r29, r27
    or r0, r3, r0
    li r20, 0x0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x28(r27)
@L_8008A2F8:
    mr r3, r17
    rlwinm r4, r20, 0, 16, 31
    bl fn_8011F228
    mr r0, r3
    mr r3, r17
    mr r16, r0
    rlwinm r4, r20, 0, 16, 31
    rlwinm r0, r16, 0, 16, 31
    rlwinm r5, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r5, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x2c(r31)
    bl fn_8011F1F0
    stb r3, 0x34(r30)
    mr r4, r16
    addi r3, r29, 0x38
    bl fn_80083ECC
    addi r31, r31, 0x2
    addi r30, r30, 0x1
    addi r29, r29, 0x50
    addi r20, r20, 0x1
    cmpwi r20, 0x4
    blt @L_8008A2F8
    addi r28, r28, 0x4
    addi r27, r27, 0x154
    addi r21, r21, 0x1
@L_8008A364:
    cmpw r21, r26
    blt @L_8008A1A4
@L_8008A36C:
    rlwinm r20, r21, 0, 24, 31
    rlwinm r0, r18, 0, 16, 23
    rlwinm r3, r20, 2, 0, 29
    rlwinm r4, r18, 0, 8, 15
    addi r3, r3, 0x8
    li r5, 0x1
    slw r5, r5, r3
    rlwinm r3, r18, 24, 0, 7
    neg r5, r5
    rlwinm r0, r0, 8, 0, 23
    or r19, r19, r5
    rlwinm r4, r4, 24, 8, 31
    rlwinm r5, r19, 0, 16, 23
    or r0, r3, r0
    rlwinm r3, r19, 0, 8, 15
    rlwinm r6, r19, 24, 0, 7
    rlwinm r5, r5, 8, 0, 23
    rlwinm r8, r19, 8, 24, 31
    rlwinm r7, r3, 24, 8, 31
    rlwinm r3, r18, 8, 24, 31
    or r5, r6, r5
    or r0, r4, r0
    or r5, r7, r5
    lwz r4, 0x38(r1)
    or r5, r8, r5
    or r3, r3, r0
    lwz r0, 0x3c(r1)
    rlwimi r4, r5, 8, 0, 23
    rlwimi r0, r3, 8, 0, 23
    stw r4, 0x38(r1)
    mr r3, r22
    li r26, 0x0
    stw r0, 0x3c(r1)
    bl fn_80205B8C
    mr r0, r3
    mr r3, r22
    mr r21, r0
    addi r4, r1, 0x20
    addi r5, r1, 0x1c
    bl fn_801FDB78
    lwz r9, 0x20(r1)
    mr r3, r21
    lwz r10, 0x1c(r1)
    rlwinm r4, r9, 0, 16, 23
    rlwinm r8, r9, 0, 8, 15
    rlwinm r0, r10, 0, 16, 23
    rlwinm r5, r10, 0, 8, 15
    rlwinm r7, r9, 24, 0, 7
    rlwinm r6, r4, 8, 0, 23
    rlwinm r4, r10, 24, 0, 7
    rlwinm r0, r0, 8, 0, 23
    rlwinm r8, r8, 24, 8, 31
    or r6, r7, r6
    rlwinm r5, r5, 24, 8, 31
    or r0, r4, r0
    rlwinm r7, r9, 8, 24, 31
    or r4, r8, r6
    or r6, r7, r4
    rlwinm r4, r10, 8, 24, 31
    or r0, r5, r0
    stw r6, 0x40(r1)
    or r0, r4, r0
    stw r0, 0x44(r1)
    bl fn_8011F5C8
    rlwinm r4, r3, 0, 16, 31
    rlwinm r3, r4, 8, 0, 23
    srawi r0, r4, 8
    cmplwi r4, 0x181
    or r0, r3, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x48(r1)
    bne @L_8008A528
    mr r3, r22
    li r4, 0x0
    bl fn_801FD614
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xa
    bne @L_8008A4AC
    li r4, 0x1
    b @L_8008A4D0
@L_8008A4AC:
    cmplwi r0, 0xb
    bne @L_8008A4BC
    li r4, 0x2
    b @L_8008A4D0
@L_8008A4BC:
    cmplwi r0, 0xf
    bne @L_8008A4CC
    li r4, 0x3
    b @L_8008A4D0
@L_8008A4CC:
    li r4, 0x0
@L_8008A4D0:
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xa
    bne @L_8008A4E4
    li r0, 0x1
    b @L_8008A508
@L_8008A4E4:
    cmplwi r0, 0xb
    bne @L_8008A4F4
    li r0, 0x2
    b @L_8008A508
@L_8008A4F4:
    cmplwi r0, 0xf
    bne @L_8008A504
    li r0, 0x3
    b @L_8008A508
@L_8008A504:
    li r0, 0x0
@L_8008A508:
    rlwinm r3, r0, 0, 16, 31
    rlwinm r0, r4, 0, 16, 31
    rlwinm r3, r3, 8, 0, 23
    srawi r0, r0, 8
    or r0, r3, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x4a(r1)
    b @L_8008A530
@L_8008A528:
    li r0, 0x0
    sth r0, 0x4a(r1)
@L_8008A530:
    lwz r0, 0xc(r1)
    cmpwi r0, 0x0
    beq @L_8008A548
    li r3, 0x0
    li r6, 0x0
    b @L_8008A5E0
@L_8008A548:
    bl fn_801F6B48
    mr r4, r22
    addi r5, r1, 0x18
    bl fn_801F2020
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8008A5A8
    lwz r3, 0x18(r1)
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x17
    bne @L_8008A580
    li r0, 0x2
    b @L_8008A5A4
@L_8008A580:
    cmplwi r0, 0x2a
    bne @L_8008A590
    li r0, 0x3
    b @L_8008A5A4
@L_8008A590:
    cmplwi r0, 0x47
    bne @L_8008A5A0
    li r0, 0x4
    b @L_8008A5A4
@L_8008A5A0:
    li r0, 0x0
@L_8008A5A4:
    rlwinm r3, r0, 0, 24, 31
@L_8008A5A8:
    subi r6, r23, 0x1
    addi r4, r1, 0x28
    rlwinm r0, r6, 2, 0, 29
    lwz r5, 0x18(r1)
    add r4, r4, r0
    mtctr r6
    cmpwi r6, 0x0
    ble @L_8008A5E0
@L_8008A5C8:
    lwz r0, 0x0(r4)
    cmplw r5, r0
    beq @L_8008A5E0
    subi r4, r4, 0x4
    subi r6, r6, 0x1
    bdnz @L_8008A5C8
@L_8008A5E0:
    lbz r0, 0x5b(r1)
    rlwimi r0, r3, 0, 28, 31
    rlwinm r4, r6, 0, 24, 31
    mr r3, r22
    stb r0, 0x5b(r1)
    rlwinm r0, r0, 0, 24, 31
    rlwimi r0, r4, 4, 24, 27
    li r4, 0x0
    stb r0, 0x5b(r1)
    bl fn_801FF1BC
    lbz r0, 0x5a(r1)
    rlwimi r0, r3, 0, 28, 31
    li r19, 0x0
    mr r16, r25
    stb r0, 0x5a(r1)
    mr r17, r25
    mr r18, r19
    lis r3, -32759
    addi r15, r3, -22116
@L_8008A62C:
    li r0, 0x0
    mr r3, r21
    stb r0, -25000(r13)
    rlwinm r4, r19, 0, 16, 31
    bl fn_8011F228
    mr r0, r3
    mr r3, r21
    mr r27, r0
    rlwinm r4, r19, 0, 16, 31
    rlwinm r0, r27, 0, 16, 31
    rlwinm r5, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r5, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x14(r16)
    bl fn_8011F1F0
    stb r3, 0x1c(r17)
    mr r3, r22
    mr r4, r27
    mr r5, r24
    mr r6, r15
    li r7, 0x1
    li r8, 0x0
    li r9, -0x1
    bl fn_8022B2CC
    lbz r0, -25000(r13)
    cmplwi r0, 0x0
    beq @L_8008A6AC
    li r0, 0x8
    slw r0, r0, r18
    or r0, r26, r0
    rlwinm r26, r0, 0, 16, 31
@L_8008A6AC:
    mr r3, r22
    rlwinm r4, r19, 0, 16, 31
    addi r6, r1, 0x10
    li r5, 0x1
    bl fn_801FFEC8
    rlwinm r3, r3, 0, 24, 31
    slw r0, r3, r18
    or r0, r26, r0
    cmplwi r3, 0x5
    rlwinm r26, r0, 0, 16, 31
    bne @L_8008A6DC
    lhz r14, 0x10(r1)
@L_8008A6DC:
    addi r16, r16, 0x2
    addi r17, r17, 0x1
    addi r18, r18, 0x4
    addi r19, r19, 0x1
    cmpwi r19, 0x4
    blt @L_8008A62C
    srawi r0, r26, 8
    rlwinm r3, r26, 8, 0, 23
    or r0, r3, r0
    rlwinm r3, r14, 8, 0, 23
    rlwinm r4, r0, 0, 16, 31
    srawi r0, r14, 8
    sth r4, 0x58(r1)
    or r0, r3, r0
    li r4, 0x3
    rlwinm r0, r0, 0, 16, 31
    lhz r3, 0x52(r1)
    cmplw r3, r0
    beq @L_8008A74C
    li r4, 0x2
    lhz r3, 0x50(r1)
    cmplw r3, r0
    beq @L_8008A74C
    li r4, 0x1
    lhz r3, 0x4e(r1)
    cmplw r3, r0
    beq @L_8008A74C
    li r4, 0x0
@L_8008A74C:
    rlwinm r3, r4, 0, 24, 31
    lbz r0, 0x5a(r1)
    rlwimi r0, r3, 4, 24, 27
    addi r14, r1, 0x28
    stb r0, 0x5a(r1)
    addi r15, r1, 0x24
    li r16, 0x0
    b @L_8008A898
@L_8008A76C:
    addi r3, r25, 0x81c
    li r4, 0x0
    li r5, 0xc
    bl memset
    lwz r3, 0x0(r14)
    cmplw r3, r22
    bne @L_8008A794
    li r0, 0xff
    stb r0, 0x81c(r25)
    b @L_8008A888
@L_8008A794:
    bl fn_80205B8C
    mr r18, r3
    bl fn_8011F598
    mr r17, r3
    bl fn_80135A70
    rlwinm r19, r3, 0, 24, 31
    mr r3, r18
    bl fn_8011F4F0
    mr r4, r3
    mr r5, r19
    addi r3, r25, 0x81c
    bl fn_800F9AEC
    cmpwi r3, 0xa
    bge @L_8008A7D8
    addi r0, r3, 0x81c
    li r3, 0xff
    stbx r3, r25, r0
@L_8008A7D8:
    mr r3, r18
    bl fn_8011F5C8
    mr r19, r3
    mr r3, r18
    bl fn_801231A4
    rlwinm r5, r19, 0, 16, 31
    lbz r6, 0x826(r25)
    rlwimi r6, r3, 6, 24, 25
    lbz r4, 0x0(r15)
    subfic r0, r5, 0x20
    stb r6, 0x826(r25)
    cntlzw r3, r0
    subfic r0, r5, 0x1d
    rlwinm r3, r3, 27, 5, 31
    lbz r5, 0x826(r25)
    rlwinm r3, r3, 0, 24, 31
    cntlzw r0, r0
    rlwimi r5, r3, 5, 26, 26
    mr r3, r17
    stb r5, 0x826(r25)
    rlwinm r0, r0, 27, 5, 31
    rlwinm r5, r0, 0, 24, 31
    lbz r0, 0x826(r25)
    rlwimi r0, r5, 4, 27, 27
    stb r0, 0x826(r25)
    lbz r0, 0x826(r25)
    rlwimi r0, r4, 0, 29, 31
    stb r0, 0x826(r25)
    bl fn_80135A70
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8008A86C
    lbz r0, 0x826(r25)
    li r3, 0x1
    rlwimi r0, r3, 3, 28, 28
    stb r0, 0x826(r25)
    b @L_8008A87C
@L_8008A86C:
    lbz r0, 0x826(r25)
    li r3, 0x0
    rlwimi r0, r3, 3, 28, 28
    stb r0, 0x826(r25)
@L_8008A87C:
    mr r3, r18
    bl fn_8011F4A8
    stb r3, 0x827(r25)
@L_8008A888:
    addi r25, r25, 0xc
    addi r14, r14, 0x4
    addi r15, r15, 0x1
    addi r16, r16, 0x1
@L_8008A898:
    cmpw r16, r23
    blt @L_8008A76C
    mulli r3, r20, 0x154
    addi r0, r1, 0x38
    addi r4, r1, 0x854
    mulli r14, r23, 0xc
    addi r3, r3, 0x24
    mr r5, r14
    add r3, r0, r3
    bl fn_800C8174
    mulli r0, r20, 0x154
    lwz r3, 0x8(r1)
    addi r4, r1, 0x38
    subi r15, r3, 0x1
    add r5, r0, r14
    mr r3, r15
    addi r5, r5, 0x24
    bl fn_80072D58
@L_8008A8E0:
    bl fn_801EF634
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x1
    bne @L_8008A900
    mr r3, r15
    bl fn_80072A00
    lis r3, 0x5
    b @L_8008A988
@L_8008A900:
    li r3, 0x0
    bl fn_801F1700
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8008A934
    bl fn_80265924
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8008A934
    mr r3, r15
    bl fn_80072A00
    lis r3, 0x4
    b @L_8008A988
@L_8008A934:
    mr r3, r15
    addi r4, r1, 0x14
    bl fn_80072C74
    cmpwi r3, 0x0
    ble @L_8008A950
    oris r3, r3, 0x5
    b @L_8008A988
@L_8008A950:
    bne @L_8008A980
    lwz r5, 0x14(r1)
    rlwinm r0, r5, 0, 16, 23
    rlwinm r4, r5, 0, 8, 15
    rlwinm r3, r5, 24, 0, 7
    rlwinm r5, r5, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r4, r4, 24, 8, 31
    or r0, r3, r0
    or r0, r4, r0
    or r3, r5, r0
    b @L_8008A988
@L_8008A980:
    bl fn_800F0308
    b @L_8008A8E0
@L_8008A988:
    lmw r14, 0x888(r1)
    lwz r0, 0x8d4(r1)
    mtlr r0
    addi r1, r1, 0x8d0
    blr
