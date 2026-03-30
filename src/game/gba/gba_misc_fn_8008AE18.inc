    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r26, 0x28(r1)
    mr r28, r3
    mr r31, r4
    mr r3, r4
    li r29, 0x0
    li r4, 0x0
    li r5, 0x64
    bl memset
    mr r3, r28
    bl fn_80123FBC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008BBC8
    mr r3, r28
    bl fn_8011F5B0
    rlwinm r0, r3, 0, 16, 23
    rlwinm r5, r3, 0, 8, 15
    rlwinm r4, r3, 24, 0, 7
    rlwinm r6, r3, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r5, r5, 24, 8, 31
    or r0, r4, r0
    mr r3, r28
    or r0, r5, r0
    or r0, r6, r0
    stw r0, 0x0(r31)
    bl fn_8011F520
    rlwinm r0, r3, 0, 16, 23
    rlwinm r5, r3, 0, 8, 15
    rlwinm r4, r3, 24, 0, 7
    rlwinm r6, r3, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r5, r5, 24, 8, 31
    or r0, r4, r0
    mr r3, r28
    or r0, r5, r0
    or r0, r6, r0
    stw r0, 0x4(r31)
    bl fn_8011F598
    mr r30, r3
    bl fn_80135AB8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0xb
    bgt @L_8008AF64
    lis r3, -32721
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -5152
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lhz r0, 0x10(r1)
    li r3, 0x1
    rlwimi r0, r3, 7, 21, 24
    sth r0, 0x10(r1)
    b @L_8008AF74
    lhz r0, 0x10(r1)
    li r3, 0x2
    rlwimi r0, r3, 7, 21, 24
    sth r0, 0x10(r1)
    b @L_8008AF74
    lhz r0, 0x10(r1)
    li r3, 0x3
    rlwimi r0, r3, 7, 21, 24
    sth r0, 0x10(r1)
    b @L_8008AF74
    lhz r0, 0x10(r1)
    li r3, 0x4
    rlwimi r0, r3, 7, 21, 24
    sth r0, 0x10(r1)
    b @L_8008AF74
    lhz r0, 0x10(r1)
    li r3, 0x5
    rlwimi r0, r3, 7, 21, 24
    sth r0, 0x10(r1)
    b @L_8008AF74
    lhz r0, 0x10(r1)
    li r3, 0xf
    rlwimi r0, r3, 7, 21, 24
    sth r0, 0x10(r1)
    b @L_8008AF74
@L_8008AF64:
    lhz r0, 0x10(r1)
    li r3, 0x0
    rlwimi r0, r3, 7, 21, 24
    sth r0, 0x10(r1)
@L_8008AF74:
    mr r3, r30
    bl fn_80135A70
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x9
    bgt @L_8008B000
    lis r3, -32721
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -5192
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r0, 0x1
    stb r0, 0x12(r31)
    b @L_8008B008
    li r0, 0x2
    stb r0, 0x12(r31)
    b @L_8008B008
    li r0, 0x5
    stb r0, 0x12(r31)
    b @L_8008B008
    li r0, 0x3
    stb r0, 0x12(r31)
    b @L_8008B008
    li r0, 0x4
    stb r0, 0x12(r31)
    b @L_8008B008
    li r0, 0x7
    stb r0, 0x12(r31)
    b @L_8008B008
    li r0, 0x2
    stb r0, 0x12(r31)
    b @L_8008B008
    li r0, 0x2
    stb r0, 0x12(r31)
    b @L_8008B008
@L_8008B000:
    li r0, 0x0
    stb r0, 0x12(r31)
@L_8008B008:
    lbz r0, 0x13(r31)
    li r3, 0x1
    rlwimi r0, r3, 1, 30, 30
    mr r3, r28
    stb r0, 0x13(r31)
    bl fn_8011E8DC
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x13(r31)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 2, 29, 29
    stb r0, 0x13(r31)
    bl fn_8011E838
    lbz r0, 0x13(r31)
    rlwimi r0, r3, 3, 24, 28
    mr r3, r28
    stb r0, 0x13(r31)
    bl fn_8011E850
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x13(r31)
    neg r4, r5
    mr r3, r30
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 0, 31, 31
    stb r0, 0x13(r31)
    bl fn_80135A70
    rlwinm r27, r3, 0, 24, 31
    mr r3, r28
    bl fn_8011F4D8
    mr r4, r3
    mr r5, r27
    addi r3, r31, 0x8
    bl fn_800F9AEC
    mr r5, r3
    cmpwi r5, 0xa
    bge @L_8008B0C4
    add r4, r31, r5
    li r0, 0xff
    addi r3, r5, 0x9
    stb r0, 0x8(r4)
    add r3, r31, r3
    subfic r5, r5, 0x9
    li r4, 0x0
    bl memset
@L_8008B0C4:
    mr r3, r30
    bl fn_80135A70
    rlwinm r30, r3, 0, 24, 31
    mr r3, r28
    bl fn_8011F508
    mr r4, r3
    mr r5, r30
    addi r3, r31, 0x14
    bl fn_800F9AEC
    mr r5, r3
    cmpwi r5, 0x7
    bge @L_8008B114
    add r4, r31, r5
    li r0, 0xff
    addi r3, r5, 0x15
    stb r0, 0x14(r4)
    add r3, r31, r3
    subfic r5, r5, 0x6
    li r4, 0x0
    bl memset
@L_8008B114:
    mr r3, r28
    bl fn_8011E820
    stb r3, 0x1b(r31)
    mr r3, r28
    bl fn_8011E7F0
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x1e(r31)
    bl fn_8011F5C8
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x20(r31)
    bl fn_8011F1A0
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x22(r31)
    bl fn_8011F4C0
    rlwinm r0, r3, 0, 16, 23
    rlwinm r5, r3, 0, 8, 15
    rlwinm r4, r3, 24, 0, 7
    rlwinm r6, r3, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r5, r5, 24, 8, 31
    or r0, r4, r0
    mr r3, r28
    or r0, r5, r0
    or r0, r6, r0
    stw r0, 0x24(r31)
    bl fn_8011EE58
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    stb r0, 0x29(r31)
    bl fn_8011E7D8
    sth r3, 0x2a(r31)
    li r26, 0x0
    mr r27, r31
    li r30, 0x0
    stb r26, 0x28(r31)
@L_8008B1DC:
    mr r3, r28
    rlwinm r4, r30, 0, 16, 31
    bl fn_8011F228
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r5, r0, 8, 0, 23
    rlwinm r4, r30, 0, 16, 31
    srawi r0, r0, 8
    or r0, r5, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x2c(r27)
    bl fn_8011F1B8
    rlwinm r3, r3, 0, 24, 31
    lbz r0, 0x28(r31)
    slw r4, r3, r26
    mr r3, r28
    or r0, r0, r4
    rlwinm r4, r30, 0, 16, 31
    rlwinm r0, r0, 0, 24, 31
    stb r0, 0x28(r31)
    bl fn_8011F1F0
    addi r0, r30, 0x34
    addi r27, r27, 0x2
    stbx r3, r31, r0
    addi r26, r26, 0x2
    addi r30, r30, 0x1
    cmpwi r30, 0x4
    blt @L_8008B1DC
    mr r3, r28
    bl fn_8011F054
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    stb r0, 0x38(r31)
    bl fn_8011F028
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    stb r0, 0x39(r31)
    bl fn_8011EFFC
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    stb r0, 0x3a(r31)
    bl fn_8011EF78
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    stb r0, 0x3b(r31)
    bl fn_8011EFD0
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    stb r0, 0x3c(r31)
    bl fn_8011EFA4
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    stb r0, 0x3d(r31)
    bl fn_8011ECEC
    stb r3, 0x3e(r31)
    mr r3, r28
    bl fn_8011ECC0
    stb r3, 0x3f(r31)
    mr r3, r28
    bl fn_8011EC94
    stb r3, 0x40(r31)
    mr r3, r28
    bl fn_8011EC68
    stb r3, 0x41(r31)
    mr r3, r28
    bl fn_8011EC3C
    stb r3, 0x42(r31)
    mr r3, r28
    bl fn_8011EB48
    stb r3, 0x43(r31)
    mr r3, r28
    bl fn_8011E8F4
    stb r3, 0x13(r1)
    mr r3, r28
    bl fn_8011F580
    rlwinm r0, r3, 0, 24, 31
    mr r3, r28
    stb r0, 0x12(r1)
    bl fn_8011F568
    lbz r0, 0x11(r1)
    rlwimi r0, r3, 0, 25, 31
    mr r3, r28
    stb r0, 0x11(r1)
    bl fn_8011F550
    lbz r0, 0x10(r1)
    rlwimi r0, r3, 3, 25, 28
    mr r3, r28
    stb r0, 0x10(r1)
    bl fn_8011F538
    lbz r0, 0x10(r1)
    rlwimi r0, r3, 7, 24, 24
    mr r3, r28
    stb r0, 0x10(r1)
    bl fn_8011EF4C
    rlwinm r3, r3, 0, 24, 31
    lbz r0, 0xf(r1)
    rlwimi r0, r3, 0, 27, 31
    mr r3, r28
    stb r0, 0xf(r1)
    bl fn_8011EF20
    lhz r0, 0xe(r1)
    rlwimi r0, r3, 5, 22, 26
    mr r3, r28
    sth r0, 0xe(r1)
    bl fn_8011EEF4
    rlwinm r3, r3, 0, 24, 31
    lbz r0, 0xe(r1)
    rlwimi r0, r3, 2, 25, 29
    mr r3, r28
    stb r0, 0xe(r1)
    bl fn_8011EE70
    rlwinm r3, r3, 0, 16, 31
    lwz r0, 0xc(r1)
    rlwimi r0, r3, 15, 12, 16
    mr r3, r28
    stw r0, 0xc(r1)
    bl fn_8011EEC8
    lhz r0, 0xc(r1)
    rlwimi r0, r3, 4, 23, 27
    mr r3, r28
    sth r0, 0xc(r1)
    bl fn_8011EE9C
    rlwinm r3, r3, 0, 24, 31
    lbz r0, 0xc(r1)
    rlwimi r0, r3, 1, 26, 30
    mr r3, r28
    stb r0, 0xc(r1)
    bl fn_8011E8DC
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0xc(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 6, 25, 25
    stb r0, 0xc(r1)
    bl fn_8011E868
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0xc(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 7, 24, 24
    stb r0, 0xc(r1)
    bl fn_8011EC10
    lbz r0, 0xb(r1)
    rlwimi r0, r3, 0, 29, 31
    mr r3, r28
    stb r0, 0xb(r1)
    bl fn_8011EBE4
    lbz r0, 0xb(r1)
    rlwimi r0, r3, 3, 26, 28
    mr r3, r28
    stb r0, 0xb(r1)
    bl fn_8011EBB8
    rlwinm r3, r3, 0, 24, 31
    lhz r0, 0xa(r1)
    rlwimi r0, r3, 6, 23, 25
    mr r3, r28
    sth r0, 0xa(r1)
    bl fn_8011EB8C
    lbz r0, 0xa(r1)
    rlwimi r0, r3, 1, 28, 30
    mr r3, r28
    stb r0, 0xa(r1)
    bl fn_8011EB60
    lbz r0, 0xa(r1)
    rlwimi r0, r3, 4, 25, 27
    mr r3, r28
    stb r0, 0xa(r1)
    bl fn_8011EB1C
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0xa(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 7, 24, 24
    stb r0, 0xa(r1)
    bl fn_8011EAF0
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x9(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 0, 31, 31
    stb r0, 0x9(r1)
    bl fn_8011EAC4
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x9(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 1, 30, 30
    stb r0, 0x9(r1)
    bl fn_8011EA98
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x9(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 2, 29, 29
    stb r0, 0x9(r1)
    bl fn_8011EA6C
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x9(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 3, 28, 28
    stb r0, 0x9(r1)
    bl fn_8011EA40
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x9(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 4, 27, 27
    stb r0, 0x9(r1)
    bl fn_8011EA14
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x9(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 5, 26, 26
    stb r0, 0x9(r1)
    bl fn_8011E9E8
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x9(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 6, 25, 25
    stb r0, 0x9(r1)
    bl fn_8011E9BC
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x9(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 7, 24, 24
    stb r0, 0x9(r1)
    bl fn_8011E990
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x8(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 0, 31, 31
    stb r0, 0x8(r1)
    bl fn_8011E964
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x8(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 1, 30, 30
    stb r0, 0x8(r1)
    bl fn_8011E938
    rlwinm r5, r3, 0, 24, 31
    lbz r0, 0x8(r1)
    neg r4, r5
    mr r3, r28
    or r4, r4, r5
    rlwinm r4, r4, 1, 31, 31
    rlwimi r0, r4, 2, 29, 29
    stb r0, 0x8(r1)
    bl fn_8011E90C
    lbz r0, 0x8(r1)
    rlwimi r0, r3, 3, 25, 28
    mr r3, r28
    stb r0, 0x8(r1)
    bl fn_8011E7A4
    lbz r0, 0x8(r1)
    rlwimi r0, r3, 7, 24, 24
    mr r3, r28
    li r27, 0x0
    stb r0, 0x8(r1)
    li r4, 0x4
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008B698
    mr r3, r28
    li r4, 0x4
    bl fn_80121984
    extsh r0, r3
    rlwinm r0, r0, 8, 0, 23
    ori r0, r0, 0x80
    rlwinm r27, r0, 0, 16, 31
    b @L_8008B754
@L_8008B698:
    mr r3, r28
    li r4, 0x5
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008B6BC
    ori r0, r27, 0x40
    rlwinm r27, r0, 0, 16, 31
    b @L_8008B754
@L_8008B6BC:
    mr r3, r28
    li r4, 0x7
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008B6E0
    ori r0, r27, 0x20
    rlwinm r27, r0, 0, 16, 31
    b @L_8008B754
@L_8008B6E0:
    mr r3, r28
    li r4, 0x6
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008B704
    ori r0, r27, 0x10
    rlwinm r27, r0, 0, 16, 31
    b @L_8008B754
@L_8008B704:
    mr r3, r28
    li r4, 0x3
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008B728
    ori r0, r27, 0x8
    rlwinm r27, r0, 0, 16, 31
    b @L_8008B754
@L_8008B728:
    mr r3, r28
    li r4, 0x8
    bl fn_80121ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8008B754
    mr r3, r28
    li r4, 0x8
    bl fn_8012189C
    extsb r0, r3
    rlwinm r27, r0, 0, 16, 31
@L_8008B754:
    mr r3, r28
    rlwinm r27, r27, 0, 16, 31
    bl fn_8011F45C
    rlwinm r0, r27, 0, 16, 23
    rlwinm r5, r27, 0, 8, 15
    rlwinm r4, r27, 24, 0, 7
    rlwinm r6, r3, 0, 0, 19
    rlwinm r0, r0, 8, 0, 23
    rlwinm r3, r5, 24, 8, 31
    or r0, r4, r0
    rlwinm r4, r27, 8, 24, 31
    or r0, r3, r0
    mr r3, r28
    or r0, r4, r0
    or r0, r0, r6
    stw r0, 0x50(r31)
    bl fn_8011F4A8
    stb r3, 0x54(r31)
    mr r3, r28
    bl fn_8011E808
    stb r3, 0x55(r31)
    mr r3, r28
    bl fn_8011F15C
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x58(r31)
    bl fn_8011F188
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x56(r31)
    bl fn_8011F130
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x5a(r31)
    bl fn_8011F104
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x5c(r31)
    bl fn_8011F080
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x5e(r31)
    bl fn_8011F0D8
    rlwinm r0, r3, 0, 16, 31
    mr r3, r28
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x60(r31)
    bl fn_8011F0AC
    lwz r11, 0x10(r1)
    rlwinm r0, r3, 0, 16, 31
    lwz r12, 0xc(r1)
    addi r4, r31, 0x20
    rlwinm r7, r11, 0, 16, 23
    rlwinm r5, r0, 8, 0, 23
    srawi r3, r0, 8
    rlwinm r10, r11, 0, 8, 15
    lwz r0, 0x8(r1)
    or r3, r5, r3
    rlwinm r27, r3, 0, 16, 31
    rlwinm r5, r12, 0, 16, 23
    rlwinm r8, r12, 0, 8, 15
    rlwinm r3, r0, 0, 16, 23
    rlwinm r6, r0, 0, 8, 15
    rlwinm r9, r11, 24, 0, 7
    rlwinm r7, r7, 8, 0, 23
    rlwinm r10, r10, 24, 8, 31
    or r9, r9, r7
    sth r27, 0x62(r31)
    rlwinm r11, r11, 8, 24, 31
    rlwinm r7, r12, 24, 0, 7
    or r9, r10, r9
    rlwinm r5, r5, 8, 0, 23
    or r9, r11, r9
    rlwinm r8, r8, 24, 8, 31
    or r7, r7, r5
    stw r9, 0x44(r31)
    rlwinm r9, r12, 8, 24, 31
    rlwinm r5, r0, 24, 0, 7
    or r7, r8, r7
    rlwinm r3, r3, 8, 0, 23
    or r7, r9, r7
    rlwinm r6, r6, 24, 8, 31
    or r5, r5, r3
    stw r7, 0x48(r31)
    rlwinm r7, r0, 8, 24, 31
    mr r3, r4
    or r0, r6, r5
    or r0, r7, r0
    stw r0, 0x4c(r31)
    li r0, 0x3
    mtctr r0
@L_8008B91C:
    lhz r5, 0x0(r3)
    lhz r7, 0x2(r3)
    srawi r0, r5, 8
    rlwinm r5, r5, 8, 0, 23
    lhz r8, 0x4(r3)
    or r6, r5, r0
    srawi r0, r7, 8
    rlwinm r5, r7, 8, 0, 23
    lhz r7, 0x6(r3)
    rlwinm r11, r6, 0, 16, 31
    or r10, r5, r0
    srawi r0, r8, 8
    rlwinm r5, r8, 8, 0, 23
    lhz r8, 0x8(r3)
    or r6, r5, r0
    srawi r0, r7, 8
    rlwinm r5, r7, 8, 0, 23
    lhz r7, 0xa(r3)
    or r9, r5, r0
    srawi r0, r8, 8
    rlwinm r5, r8, 8, 0, 23
    lhz r26, 0xc(r3)
    lhz r12, 0xe(r3)
    or r8, r5, r0
    srawi r0, r7, 8
    rlwinm r5, r7, 8, 0, 23
    or r7, r5, r0
    srawi r0, r26, 8
    rlwinm r5, r26, 8, 0, 23
    add r29, r29, r11
    rlwinm r11, r10, 0, 16, 31
    rlwinm r10, r6, 0, 16, 31
    add r29, r29, r11
    or r6, r5, r0
    add r29, r29, r10
    rlwinm r0, r9, 0, 16, 31
    add r29, r29, r0
    rlwinm r0, r8, 0, 16, 31
    add r29, r29, r0
    rlwinm r7, r7, 0, 16, 31
    rlwinm r5, r12, 8, 0, 23
    srawi r0, r12, 8
    or r0, r5, r0
    add r29, r29, r7
    rlwinm r5, r6, 0, 16, 31
    addi r3, r3, 0x10
    add r29, r29, r5
    rlwinm r0, r0, 0, 16, 31
    add r29, r29, r0
    bdnz @L_8008B91C
    rlwinm r0, r29, 0, 16, 31
    rlwinm r3, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r3, r0
    rlwinm r0, r0, 0, 16, 31
    sth r0, 0x1c(r31)
    li r0, 0x2
    mtctr r0
@L_8008BA04:
    lwz r3, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x0(r4)
    xor r0, r3, r0
    xor r0, r5, r0
    stw r0, 0x0(r4)
    lwz r3, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x4(r4)
    xor r0, r3, r0
    xor r0, r5, r0
    stw r0, 0x4(r4)
    lwz r3, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x8(r4)
    xor r0, r3, r0
    xor r0, r5, r0
    stw r0, 0x8(r4)
    lwz r3, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0xc(r4)
    xor r0, r3, r0
    xor r0, r5, r0
    stw r0, 0xc(r4)
    lwz r3, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x10(r4)
    xor r0, r3, r0
    xor r0, r5, r0
    stw r0, 0x10(r4)
    lwz r3, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x14(r4)
    xor r0, r3, r0
    xor r0, r5, r0
    stw r0, 0x14(r4)
    addi r4, r4, 0x18
    bdnz @L_8008BA04
    lwz r6, 0x0(r31)
    lis r3, 0xaaab
    subi r3, r3, 0x5555
    rlwinm r0, r6, 0, 16, 23
    rlwinm r5, r6, 0, 8, 15
    rlwinm r4, r6, 24, 0, 7
    rlwinm r6, r6, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r5, r5, 24, 8, 31
    or r0, r4, r0
    or r0, r5, r0
    or r27, r6, r0
    mulhwu r0, r3, r27
    rlwinm r0, r0, 28, 4, 31
    mulli r0, r0, 0x18
    subf r27, r0, r27
    mulhwu r0, r3, r27
    rlwinm r0, r0, 30, 2, 31
    cmplwi r0, 0x0
    beq @L_8008BB24
    mulli r26, r0, 0xc
    addi r3, r1, 0x14
    li r5, 0xc
    add r4, r31, r26
    addi r4, r4, 0x20
    bl memcpy
    mr r5, r26
    addi r3, r31, 0x2c
    addi r4, r31, 0x20
    bl fn_800C8174
    addi r3, r31, 0x20
    addi r4, r1, 0x14
    li r5, 0xc
    bl memcpy
@L_8008BB24:
    lis r3, 0xaaab
    subi r0, r3, 0x5555
    mulhwu r0, r0, r27
    rlwinm r0, r0, 30, 2, 31
    mulli r0, r0, 0x6
    subf r27, r0, r27
    rlwinm r0, r27, 31, 1, 31
    cmplwi r0, 0x0
    beq @L_8008BB84
    mulli r26, r0, 0xc
    addi r3, r1, 0x14
    li r5, 0xc
    add r4, r31, r26
    addi r4, r4, 0x2c
    bl memcpy
    addi r28, r31, 0x2c
    mr r5, r26
    mr r4, r28
    addi r3, r31, 0x38
    bl fn_800C8174
    mr r3, r28
    addi r4, r1, 0x14
    li r5, 0xc
    bl memcpy
@L_8008BB84:
    rlwinm r0, r27, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8008BBC8
    addi r26, r31, 0x44
    addi r3, r1, 0x14
    mr r4, r26
    li r5, 0xc
    bl memcpy
    addi r27, r31, 0x38
    mr r3, r26
    mr r4, r27
    li r5, 0xc
    bl memcpy
    mr r3, r27
    addi r4, r1, 0x14
    li r5, 0xc
    bl memcpy
@L_8008BBC8:
    lmw r26, 0x28(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
