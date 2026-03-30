    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r25, 0x44(r1)
    mr r30, r3
    mr r31, r4
    bl fn_80124A60
    lbz r3, 0x13(r31)
    rlwinm r0, r3, 31, 31, 31
    cmplwi r0, 0x0
    bne @L_8008BC14
    rlwinm r0, r3, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8008C5C0
@L_8008BC14:
    addi r3, r31, 0x20
    li r0, 0x2
    mtctr r0
@L_8008BC20:
    lwz r4, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x0(r3)
    xor r0, r4, r0
    xor r0, r5, r0
    stw r0, 0x0(r3)
    lwz r4, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x4(r3)
    xor r0, r4, r0
    xor r0, r5, r0
    stw r0, 0x4(r3)
    lwz r4, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x8(r3)
    xor r0, r4, r0
    xor r0, r5, r0
    stw r0, 0x8(r3)
    lwz r4, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0xc(r3)
    xor r0, r4, r0
    xor r0, r5, r0
    stw r0, 0xc(r3)
    lwz r4, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x10(r3)
    xor r0, r4, r0
    xor r0, r5, r0
    stw r0, 0x10(r3)
    lwz r4, 0x0(r31)
    lwz r0, 0x4(r31)
    lwz r5, 0x14(r3)
    xor r0, r4, r0
    xor r0, r5, r0
    stw r0, 0x14(r3)
    addi r3, r3, 0x18
    bdnz @L_8008BC20
    lwz r5, 0x0(r31)
    rlwinm r0, r5, 0, 16, 23
    rlwinm r4, r5, 0, 8, 15
    rlwinm r3, r5, 24, 0, 7
    rlwinm r5, r5, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r4, r4, 24, 8, 31
    or r0, r3, r0
    or r0, r4, r0
    or r25, r5, r0
    rlwinm r0, r25, 0, 31, 31
    cmplwi r0, 0x0
    beq @L_8008BD24
    addi r26, r31, 0x38
    addi r3, r1, 0x14
    mr r4, r26
    li r5, 0xc
    bl memcpy
    addi r27, r31, 0x44
    mr r3, r26
    mr r4, r27
    li r5, 0xc
    bl memcpy
    mr r3, r27
    addi r4, r1, 0x14
    li r5, 0xc
    bl memcpy
@L_8008BD24:
    lis r3, 0xaaab
    rlwinm r4, r25, 31, 1, 31
    subi r0, r3, 0x5555
    mulhwu r0, r0, r4
    rlwinm r0, r0, 31, 1, 31
    mulli r0, r0, 0x3
    subf r26, r0, r4
    cmplwi r26, 0x0
    beq @L_8008BD84
    addi r27, r31, 0x2c
    addi r3, r1, 0x14
    mr r4, r27
    li r5, 0xc
    bl memcpy
    mulli r26, r26, 0xc
    mr r3, r27
    addi r4, r31, 0x38
    mr r5, r26
    bl fn_800C8174
    add r3, r31, r26
    addi r4, r1, 0x14
    addi r3, r3, 0x2c
    li r5, 0xc
    bl memcpy
@L_8008BD84:
    lis r3, 0xaaab
    subi r0, r3, 0x5555
    mulhwu r0, r0, r25
    rlwinm r0, r0, 30, 2, 31
    rlwinm r25, r0, 0, 30, 31
    cmplwi r25, 0x0
    beq @L_8008BDD8
    addi r3, r1, 0x14
    addi r4, r31, 0x20
    li r5, 0xc
    bl memcpy
    mulli r25, r25, 0xc
    addi r3, r31, 0x20
    addi r4, r31, 0x2c
    mr r5, r25
    bl fn_800C8174
    add r3, r31, r25
    addi r4, r1, 0x14
    addi r3, r3, 0x20
    li r5, 0xc
    bl memcpy
@L_8008BDD8:
    lwz r25, 0x44(r31)
    mr r3, r30
    lwz r0, 0x48(r31)
    lwz r4, 0x4c(r31)
    rlwinm r11, r25, 0, 16, 23
    lwz r5, 0x0(r31)
    rlwinm r9, r0, 0, 16, 23
    rlwinm r8, r4, 0, 16, 23
    rlwinm r26, r25, 0, 8, 15
    rlwinm r29, r0, 0, 8, 15
    rlwinm r10, r4, 0, 8, 15
    rlwinm r6, r5, 0, 16, 23
    rlwinm r7, r5, 0, 8, 15
    rlwinm r27, r25, 24, 0, 7
    rlwinm r28, r11, 8, 0, 23
    rlwinm r12, r0, 24, 0, 7
    rlwinm r11, r9, 8, 0, 23
    rlwinm r9, r4, 24, 0, 7
    rlwinm r8, r8, 8, 0, 23
    rlwinm r26, r26, 24, 8, 31
    or r28, r27, r28
    rlwinm r29, r29, 24, 8, 31
    or r11, r12, r11
    rlwinm r10, r10, 24, 8, 31
    or r8, r9, r8
    rlwinm r12, r25, 8, 24, 31
    or r9, r26, r28
    or r28, r12, r9
    rlwinm r12, r0, 8, 24, 31
    or r11, r29, r11
    rlwinm r9, r4, 8, 24, 31
    or r8, r10, r8
    rlwinm r4, r5, 24, 0, 7
    rlwinm r0, r6, 8, 0, 23
    or r10, r12, r11
    or r8, r9, r8
    rlwinm r6, r7, 24, 8, 31
    or r0, r4, r0
    stw r28, 0x10(r1)
    rlwinm r4, r5, 8, 24, 31
    or r0, r6, r0
    stw r10, 0xc(r1)
    or r4, r4, r0
    stw r8, 0x8(r1)
    bl fn_8011DFE0
    lwz r6, 0x4(r31)
    mr r3, r30
    rlwinm r0, r6, 0, 16, 23
    rlwinm r5, r6, 0, 8, 15
    rlwinm r4, r6, 24, 0, 7
    rlwinm r6, r6, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r5, r5, 24, 8, 31
    or r0, r4, r0
    or r0, r5, r0
    or r4, r6, r0
    bl fn_8011DF90
    lhz r0, 0x10(r1)
    rlwinm r0, r0, 25, 28, 31
    cmplwi r0, 0xf
    bgt @L_8008BF14
    lis r3, -32721
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -5072
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r25, 0x8
    b @L_8008BF18
    li r25, 0x9
    b @L_8008BF18
    li r25, 0xa
    b @L_8008BF18
    li r25, 0x1
    b @L_8008BF18
    li r25, 0x2
    b @L_8008BF18
    li r25, 0xb
    b @L_8008BF18
@L_8008BF14:
    li r25, 0x0
@L_8008BF18:
    lbz r0, 0x12(r31)
    cmplwi r0, 0x7
    bgt @L_8008BF84
    lis r3, -32721
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, -5104
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r26, 0x1
    li r27, 0x1
    b @L_8008BF8C
    li r26, 0x2
    li r27, 0x2
    b @L_8008BF8C
    li r26, 0x3
    li r27, 0x4
    b @L_8008BF8C
    li r26, 0x3
    li r27, 0x5
    b @L_8008BF8C
    li r26, 0x3
    li r27, 0x3
    b @L_8008BF8C
    li r26, 0x3
    li r27, 0x6
    b @L_8008BF8C
@L_8008BF84:
    li r26, 0x0
    li r27, 0x0
@L_8008BF8C:
    mr r3, r30
    bl fn_8011F598
    mr r4, r25
    mr r6, r26
    mr r7, r27
    li r5, 0x3
    bl fn_801353C0
    rlwinm r25, r27, 0, 24, 31
    addi r3, r1, 0x20
    mr r6, r25
    addi r4, r31, 0x8
    li r5, 0xa
    bl fn_800F9C04
    mr r3, r30
    addi r4, r1, 0x20
    bl fn_8011DEE4
    lbz r0, 0x13(r31)
    mr r3, r30
    rlwinm r4, r0, 29, 27, 31
    bl fn_8011D4E4
    lbz r0, 0x13(r31)
    mr r3, r30
    rlwinm r4, r0, 0, 31, 31
    bl fn_8011D4F4
    mr r3, r30
    bl fn_8011F508
    mr r6, r25
    addi r4, r31, 0x14
    li r5, 0x7
    bl fn_800F9C04
    lbz r4, 0x1b(r31)
    mr r3, r30
    bl fn_8011D4D4
    lhz r0, 0x1e(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011D4B4
    lhz r0, 0x20(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011DFF0
    lhz r0, 0x22(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011DCB4
    lwz r6, 0x24(r31)
    mr r3, r30
    rlwinm r0, r6, 0, 16, 23
    rlwinm r5, r6, 0, 8, 15
    rlwinm r4, r6, 24, 0, 7
    rlwinm r6, r6, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r5, r5, 24, 8, 31
    or r0, r4, r0
    or r0, r5, r0
    or r4, r6, r0
    bl fn_8011DE98
    lbz r4, 0x29(r31)
    mr r3, r30
    bl fn_8011D904
    lhz r4, 0x2a(r31)
    mr r3, r30
    bl fn_8011D4A4
    li r27, 0x0
    mr r25, r31
    mr r26, r27
@L_8008C0B8:
    lhz r0, 0x2c(r25)
    mr r3, r30
    rlwinm r4, r27, 0, 16, 31
    rlwinm r5, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r5, r0
    rlwinm r5, r0, 0, 16, 31
    bl fn_8011DDFC
    lbz r0, 0x28(r31)
    mr r3, r30
    rlwinm r4, r27, 0, 16, 31
    sraw r0, r0, r26
    rlwinm r5, r0, 0, 30, 31
    bl fn_8011DCC4
    addi r0, r27, 0x34
    mr r3, r30
    lbzx r5, r31, r0
    rlwinm r4, r27, 0, 16, 31
    bl fn_8011DD80
    addi r25, r25, 0x2
    addi r26, r26, 0x2
    addi r27, r27, 0x1
    cmpwi r27, 0x4
    blt @L_8008C0B8
    lbz r4, 0x38(r31)
    mr r3, r30
    bl fn_8011DB60
    lbz r4, 0x39(r31)
    mr r3, r30
    bl fn_8011DB2C
    lbz r4, 0x3a(r31)
    mr r3, r30
    bl fn_8011DAF8
    lbz r4, 0x3b(r31)
    mr r3, r30
    bl fn_8011DA5C
    lbz r4, 0x3c(r31)
    mr r3, r30
    bl fn_8011DAC4
    lbz r4, 0x3d(r31)
    mr r3, r30
    bl fn_8011DA90
    lbz r4, 0x3e(r31)
    mr r3, r30
    bl fn_8011D8B4
    lbz r4, 0x3f(r31)
    mr r3, r30
    bl fn_8011D890
    lbz r4, 0x40(r31)
    mr r3, r30
    bl fn_8011D86C
    lbz r4, 0x41(r31)
    mr r3, r30
    bl fn_8011D848
    lbz r4, 0x42(r31)
    mr r3, r30
    bl fn_8011D824
    lbz r4, 0x43(r31)
    mr r3, r30
    bl fn_8011D760
    lbz r4, 0x13(r1)
    mr r3, r30
    bl fn_8011D57C
    lbz r4, 0x12(r1)
    mr r3, r30
    bl fn_8011DFD0
    lbz r0, 0x11(r1)
    mr r3, r30
    rlwinm r4, r0, 0, 25, 31
    bl fn_8011DFC0
    lbz r0, 0x10(r1)
    mr r3, r30
    rlwinm r4, r0, 29, 28, 31
    bl fn_8011DFB0
    lbz r0, 0x10(r1)
    mr r3, r30
    rlwinm r4, r0, 25, 31, 31
    bl fn_8011DFA0
    lbz r0, 0xf(r1)
    mr r3, r30
    rlwinm r4, r0, 0, 27, 31
    bl fn_8011DA28
    lhz r0, 0xe(r1)
    mr r3, r30
    rlwinm r4, r0, 27, 27, 31
    bl fn_8011D9F4
    lbz r0, 0xe(r1)
    mr r3, r30
    rlwinm r4, r0, 30, 27, 31
    bl fn_8011D9C0
    lwz r0, 0xc(r1)
    mr r3, r30
    rlwinm r4, r0, 17, 27, 31
    bl fn_8011D924
    lhz r0, 0xc(r1)
    mr r3, r30
    rlwinm r4, r0, 28, 27, 31
    bl fn_8011D98C
    lbz r0, 0xc(r1)
    mr r3, r30
    rlwinm r4, r0, 31, 27, 31
    bl fn_8011D958
    lbz r0, 0xc(r1)
    mr r3, r30
    rlwinm r4, r0, 26, 31, 31
    bl fn_8011D56C
    lbz r0, 0xc(r1)
    mr r3, r30
    rlwinm r4, r0, 25, 31, 31
    bl fn_8011D504
    lbz r0, 0xb(r1)
    mr r3, r30
    rlwinm r4, r0, 0, 29, 31
    bl fn_8011D800
    lbz r0, 0xb(r1)
    mr r3, r30
    rlwinm r4, r0, 29, 29, 31
    bl fn_8011D7DC
    lhz r0, 0xa(r1)
    mr r3, r30
    rlwinm r4, r0, 26, 29, 31
    bl fn_8011D7B8
    lbz r0, 0xa(r1)
    mr r3, r30
    rlwinm r4, r0, 31, 29, 31
    bl fn_8011D794
    lbz r0, 0xa(r1)
    mr r3, r30
    rlwinm r4, r0, 28, 29, 31
    bl fn_8011D770
    lbz r0, 0xa(r1)
    mr r3, r30
    rlwinm r4, r0, 25, 31, 31
    bl fn_8011D73C
    lbz r0, 0x9(r1)
    mr r3, r30
    rlwinm r4, r0, 0, 31, 31
    bl fn_8011D718
    lbz r0, 0x9(r1)
    mr r3, r30
    rlwinm r4, r0, 31, 31, 31
    bl fn_8011D6F4
    lbz r0, 0x9(r1)
    mr r3, r30
    rlwinm r4, r0, 30, 31, 31
    bl fn_8011D6D0
    lbz r0, 0x9(r1)
    mr r3, r30
    rlwinm r4, r0, 29, 31, 31
    bl fn_8011D6AC
    lbz r0, 0x9(r1)
    mr r3, r30
    rlwinm r4, r0, 28, 31, 31
    bl fn_8011D688
    lbz r0, 0x9(r1)
    mr r3, r30
    rlwinm r4, r0, 27, 31, 31
    bl fn_8011D664
    lbz r0, 0x9(r1)
    mr r3, r30
    rlwinm r4, r0, 26, 31, 31
    bl fn_8011D640
    lbz r0, 0x9(r1)
    mr r3, r30
    rlwinm r4, r0, 25, 31, 31
    bl fn_8011D61C
    lbz r0, 0x8(r1)
    mr r3, r30
    rlwinm r4, r0, 0, 31, 31
    bl fn_8011D5F8
    lbz r0, 0x8(r1)
    mr r3, r30
    rlwinm r4, r0, 31, 31, 31
    bl fn_8011D5D4
    lbz r0, 0x8(r1)
    mr r3, r30
    rlwinm r4, r0, 30, 31, 31
    bl fn_8011D5B0
    lbz r0, 0x8(r1)
    mr r3, r30
    rlwinm r4, r0, 29, 28, 31
    bl fn_8011D58C
    lbz r0, 0x8(r1)
    mr r3, r30
    rlwinm r4, r0, 25, 31, 31
    bl fn_8011D480
    lwz r5, 0x50(r31)
    rlwinm r0, r5, 0, 16, 23
    rlwinm r4, r5, 0, 8, 15
    rlwinm r3, r5, 24, 0, 7
    rlwinm r5, r5, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r4, r4, 24, 8, 31
    or r0, r3, r0
    or r0, r4, r0
    or r0, r5, r0
    rlwinm r29, r0, 0, 16, 31
    rlwinm r0, r29, 0, 24, 24
    cmpwi r0, 0x0
    beq @L_8008C404
    mr r3, r30
    li r4, 0x4
    li r5, 0x0
    bl fn_801219F4
    rlwinm r0, r29, 0, 20, 23
    mr r3, r30
    srawi r0, r0, 8
    li r4, 0x4
    extsh r5, r0
    bl fn_8012190C
    b @L_8008C4B0
@L_8008C404:
    rlwinm r0, r29, 0, 25, 25
    cmpwi r0, 0x0
    beq @L_8008C424
    mr r3, r30
    li r4, 0x5
    li r5, 0x0
    bl fn_801219F4
    b @L_8008C4B0
@L_8008C424:
    rlwinm r0, r29, 0, 26, 26
    cmpwi r0, 0x0
    beq @L_8008C444
    mr r3, r30
    li r4, 0x7
    li r5, 0x0
    bl fn_801219F4
    b @L_8008C4B0
@L_8008C444:
    rlwinm r0, r29, 0, 27, 27
    cmpwi r0, 0x0
    beq @L_8008C464
    mr r3, r30
    li r4, 0x6
    li r5, 0x0
    bl fn_801219F4
    b @L_8008C4B0
@L_8008C464:
    rlwinm r0, r29, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8008C484
    mr r3, r30
    li r4, 0x3
    li r5, 0x0
    bl fn_801219F4
    b @L_8008C4B0
@L_8008C484:
    rlwinm r25, r29, 0, 29, 31
    cmpwi r25, 0x0
    beq @L_8008C4B0
    mr r3, r30
    li r4, 0x8
    li r5, 0x0
    bl fn_801219F4
    mr r3, r30
    extsb r5, r25
    li r4, 0x8
    bl fn_8012173C
@L_8008C4B0:
    lwz r6, 0x50(r31)
    mr r3, r30
    rlwinm r0, r6, 0, 16, 23
    rlwinm r5, r6, 0, 8, 15
    rlwinm r4, r6, 24, 0, 7
    rlwinm r6, r6, 8, 24, 31
    rlwinm r0, r0, 8, 0, 23
    rlwinm r5, r5, 24, 8, 31
    or r0, r4, r0
    or r0, r5, r0
    or r0, r6, r0
    rlwinm r4, r0, 0, 0, 19
    bl fn_8011DE38
    lbz r4, 0x54(r31)
    mr r3, r30
    bl fn_8011DE48
    lbz r4, 0x55(r31)
    mr r3, r30
    bl fn_8011D4C4
    lhz r0, 0x58(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011DC48
    lhz r0, 0x56(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011DC6C
    lhz r0, 0x5a(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011DC24
    lhz r0, 0x5c(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011DC00
    lhz r0, 0x5e(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011DB94
    lhz r0, 0x60(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011DBDC
    lhz r0, 0x62(r31)
    mr r3, r30
    rlwinm r4, r0, 8, 0, 23
    srawi r0, r0, 8
    or r0, r4, r0
    rlwinm r4, r0, 0, 16, 31
    bl fn_8011DBB8
@L_8008C5C0:
    lmw r25, 0x44(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
