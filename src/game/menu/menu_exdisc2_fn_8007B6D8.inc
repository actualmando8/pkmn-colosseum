    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r25, 0x24(r1)
    mr r31, r3
    li r0, 0x0
    lbz r30, 0x340(r31)
    addi r4, r1, 0x9
    stb r0, 0x346(r31)
    mr r3, r30
    bl fn_8025F484
    cmpwi r3, 0x0
    beq @L_8007B714
    li r3, 0x0
    b @L_8007C228
@L_8007B714:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    bne @L_8007BC18
    lbz r0, 0x9(r1)
    cmplwi r0, 0x28
    bne @L_8007BC18
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_8007B754
    li r3, 0x0
    b @L_8007C228
@L_8007B754:
    lwz r4, 0x10(r1)
    lwz r0, 0x350(r31)
    lbz r3, 0x9(r1)
    cmplw r4, r0
    bne @L_8007B770
    li r4, 0x1
    b @L_8007B7E0
@L_8007B770:
    rlwinm r0, r3, 0, 26, 27
    cmplwi r0, 0x0
    beq @L_8007B784
    li r4, 0x0
    b @L_8007B7E0
@L_8007B784:
    lwz r0, 0x34c(r31)
    xor r5, r0, r4
    subis r0, r5, 0x2000
    cmplwi r0, 0x0
    bne @L_8007B7A4
    li r0, 0x100
    sth r0, 0x348(r31)
    b @L_8007B7DC
@L_8007B7A4:
    subis r0, r5, 0x20
    cmplwi r0, 0x0
    bne @L_8007B7BC
    li r0, 0x200
    sth r0, 0x348(r31)
    b @L_8007B7DC
@L_8007B7BC:
    lis r3, 0xdfe0
    li r4, 0x0
    subi r0, r3, 0x2021
    sth r4, 0x348(r31)
    and r0, r5, r0
    cmplwi r0, 0x0
    beq @L_8007B7DC
    b @L_8007B7E0
@L_8007B7DC:
    li r4, 0x2
@L_8007B7E0:
    cmplwi r4, 0x1
    beq @L_8007B7F0
    li r3, 0x0
    b @L_8007C228
@L_8007B7F0:
    lbz r0, 0x343(r31)
    cmplwi r0, 0x0
    beq @L_8007B804
    li r3, 0x3
    b @L_8007C228
@L_8007B804:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    bne @L_8007B824
    lbz r0, 0x9(r1)
    cmplwi r0, 0x20
    beq @L_8007B82C
@L_8007B824:
    li r3, 0x0
    b @L_8007C228
@L_8007B82C:
    mr r3, r30
    addi r4, r31, 0x350
    addi r5, r1, 0x9
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_8007B84C
    li r3, 0x0
    b @L_8007C228
@L_8007B84C:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    bne @L_8007B86C
    lbz r0, 0x9(r1)
    cmplwi r0, 0x30
    beq @L_8007B874
@L_8007B86C:
    li r3, 0x0
    b @L_8007C228
@L_8007B874:
    lwz r6, 0x380(r31)
    mr r3, r30
    lwz r0, 0x384(r31)
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    subf r29, r6, r0
    rlwinm r6, r29, 0, 0, 7
    rlwinm r0, r29, 0, 8, 15
    rlwinm r7, r29, 0, 16, 23
    rlwinm r8, r29, 0, 24, 31
    rlwinm r6, r6, 8, 24, 31
    rlwinm r0, r0, 24, 8, 31
    rlwinm r7, r7, 8, 0, 23
    or r0, r6, r0
    rlwinm r6, r8, 24, 0, 7
    or r0, r7, r0
    or r0, r6, r0
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_8007B8D0
    li r3, 0x0
    b @L_8007C228
@L_8007B8D0:
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    bne @L_8007B8F8
    lwz r3, 0xc(r1)
    lwz r0, 0x10(r1)
    cmplw r3, r0
    beq @L_8007B900
@L_8007B8F8:
    li r3, 0x0
    b @L_8007C228
@L_8007B900:
    li r0, 0x1
    li r26, 0x0
    stb r0, 0x346(r31)
    lis r3, -32760
    addi r28, r3, -15812
    b @L_8007BA30
@L_8007B918:
    li r25, 0x0
@L_8007B91C:
    lbz r0, 0x342(r31)
    cmplwi r0, 0x0
    beq @L_8007B938
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007B938:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_8007B95C
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007B95C:
    lbz r3, 0x9(r1)
    rlwinm r0, r3, 0, 26, 27
    cmpwi r0, 0x30
    beq @L_8007B998
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8007B988
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
@L_8007B988:
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007B998:
    rlwinm r0, r3, 0, 30, 30
    cmpwi r0, 0x0
    beq @L_8007B9F8
    addi r25, r25, 0x1
    cmplwi r25, 0x9
    ble @L_8007B9C0
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007B9C0:
    mr r3, r31
    bl OSCreateAlarm
    bl OSDisableInterrupts
    mr r27, r3
    mr r3, r31
    mr r7, r28
    li r6, 0x10
    li r5, 0x0
    bl OSSetAlarm
    addi r3, r31, 0x28
    bl fn_800A221C
    mr r3, r27
    bl OSRestoreInterrupts
    b @L_8007B91C
@L_8007B9F8:
    lwz r6, 0x380(r31)
    mr r3, r30
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    lwzx r0, r6, r26
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_8007BA2C
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BA2C:
    addi r26, r26, 0x4
@L_8007BA30:
    cmplw r26, r29
    blt @L_8007B918
    li r0, 0x2
    stb r0, 0x346(r31)
    lis r3, -32760
    addi r29, r3, -15812
@L_8007BA48:
    lbz r0, 0x342(r31)
    cmplwi r0, 0x0
    beq @L_8007BA64
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BA64:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    bne @L_8007BAC0
    lbz r0, 0x9(r1)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x0
    bne @L_8007BAC0
    mr r3, r31
    bl OSCreateAlarm
    bl OSDisableInterrupts
    mr r27, r3
    mr r3, r31
    mr r7, r29
    li r6, 0x10
    li r5, 0x0
    bl OSSetAlarm
    addi r3, r31, 0x28
    bl fn_800A221C
    mr r3, r27
    bl OSRestoreInterrupts
    b @L_8007BA48
@L_8007BAC0:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F484
    cmpwi r3, 0x0
    beq @L_8007BAE4
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BAE4:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    bne @L_8007BB04
    lbz r0, 0x9(r1)
    cmplwi r0, 0x8
    beq @L_8007BB14
@L_8007BB04:
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BB14:
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_8007BB3C
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BB3C:
    lwz r4, 0x10(r1)
    lwz r0, 0x350(r31)
    lbz r3, 0x9(r1)
    cmplw r4, r0
    bne @L_8007BB58
    li r4, 0x1
    b @L_8007BBC8
@L_8007BB58:
    rlwinm r0, r3, 0, 26, 27
    cmplwi r0, 0x0
    beq @L_8007BB6C
    li r4, 0x0
    b @L_8007BBC8
@L_8007BB6C:
    lwz r0, 0x34c(r31)
    xor r5, r0, r4
    subis r0, r5, 0x2000
    cmplwi r0, 0x0
    bne @L_8007BB8C
    li r0, 0x100
    sth r0, 0x348(r31)
    b @L_8007BBC4
@L_8007BB8C:
    subis r0, r5, 0x20
    cmplwi r0, 0x0
    bne @L_8007BBA4
    li r0, 0x200
    sth r0, 0x348(r31)
    b @L_8007BBC4
@L_8007BBA4:
    lis r3, 0xdfe0
    li r4, 0x0
    subi r0, r3, 0x2021
    sth r4, 0x348(r31)
    and r0, r5, r0
    cmplwi r0, 0x0
    beq @L_8007BBC4
    b @L_8007BBC8
@L_8007BBC4:
    li r4, 0x2
@L_8007BBC8:
    cmplwi r4, 0x2
    beq @L_8007BBE0
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BBE0:
    mr r3, r30
    addi r4, r31, 0x34c
    addi r5, r1, 0x9
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_8007BC08
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BC08:
    li r0, 0x3
    li r3, 0x1
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BC18:
    cmpwi r3, 0x0
    bne @L_8007C224
    lbz r0, 0x9(r1)
    cmplwi r0, 0x8
    bne @L_8007C224
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_8007BC4C
    li r3, 0x0
    b @L_8007C228
@L_8007BC4C:
    lwz r4, 0x10(r1)
    lwz r0, 0x350(r31)
    lbz r3, 0x9(r1)
    cmplw r4, r0
    bne @L_8007BC68
    li r4, 0x1
    b @L_8007BCD8
@L_8007BC68:
    rlwinm r0, r3, 0, 26, 27
    cmplwi r0, 0x0
    beq @L_8007BC7C
    li r4, 0x0
    b @L_8007BCD8
@L_8007BC7C:
    lwz r0, 0x34c(r31)
    xor r5, r0, r4
    subis r0, r5, 0x2000
    cmplwi r0, 0x0
    bne @L_8007BC9C
    li r0, 0x100
    sth r0, 0x348(r31)
    b @L_8007BCD4
@L_8007BC9C:
    subis r0, r5, 0x20
    cmplwi r0, 0x0
    bne @L_8007BCB4
    li r0, 0x200
    sth r0, 0x348(r31)
    b @L_8007BCD4
@L_8007BCB4:
    lis r3, 0xdfe0
    li r4, 0x0
    subi r0, r3, 0x2021
    sth r4, 0x348(r31)
    and r0, r5, r0
    cmplwi r0, 0x0
    beq @L_8007BCD4
    b @L_8007BCD8
@L_8007BCD4:
    li r4, 0x2
@L_8007BCD8:
    cmplwi r4, 0x2
    beq @L_8007BCE8
    li r3, 0x0
    b @L_8007C228
@L_8007BCE8:
    lbz r0, 0x344(r31)
    cmplwi r0, 0x0
    beq @L_8007BD00
    lwz r0, 0x34c(r31)
    stw r0, 0xc(r1)
    b @L_8007BD18
@L_8007BD00:
    li r0, 0x4
    stb r0, 0x346(r31)
    lwz r0, 0x34c(r31)
    oris r0, r0, 0x2020
    ori r0, r0, 0x2020
    stw r0, 0xc(r1)
@L_8007BD18:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    bne @L_8007BD38
    lbz r0, 0x9(r1)
    cmplwi r0, 0x0
    beq @L_8007BD48
@L_8007BD38:
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BD48:
    mr r3, r30
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    bl fn_8025F648
    cmpwi r3, 0x0
    beq @L_8007BD70
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BD70:
    lbz r0, 0x344(r31)
    cmplwi r0, 0x0
    beq @L_8007BDA4
    li r0, 0x0
    stb r0, 0x346(r31)
    lhz r0, 0x348(r31)
    rlwinm r0, r0, 0, 31, 31
    cmpwi r0, 0x0
    beq @L_8007BD9C
    li r3, 0x4
    b @L_8007C228
@L_8007BD9C:
    li r3, 0x2
    b @L_8007C228
@L_8007BDA4:
    lis r3, -32760
    addi r29, r3, -15812
@L_8007BDAC:
    lbz r0, 0x342(r31)
    cmplwi r0, 0x0
    beq @L_8007BDC8
    li r0, 0x6
    li r3, 0x6
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BDC8:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    bne @L_8007C19C
    lbz r3, 0x9(r1)
    rlwinm r0, r3, 0, 26, 27
    cmpwi r0, 0x30
    beq @L_8007C19C
    cmpwi r0, 0x0
    beq @L_8007C19C
    andi. r0, r3, 0x32
    cmpwi r0, 0x20
    bne @L_8007C164
    bl OSGetTick
    stw r3, 0x358(r31)
    mr r3, r30
    addi r4, r1, 0xc
    addi r5, r1, 0x9
    lwz r0, 0x358(r31)
    stw r0, 0xc(r1)
    bl fn_8025F648
    cmpwi r3, 0x0
    bne @L_8007BE38
    lbz r0, 0x9(r1)
    rlwinm r0, r0, 0, 26, 27
    cmpwi r0, 0x20
    beq @L_8007BE48
@L_8007BE38:
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BE48:
    li r28, 0x0
    lis r3, -32760
    addi r29, r3, -15812
@L_8007BE54:
    li r26, 0x0
@L_8007BE58:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    beq @L_8007BE7C
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BE7C:
    lbz r3, 0x9(r1)
    rlwinm r0, r3, 0, 26, 27
    cmpwi r0, 0x20
    beq @L_8007BE9C
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BE9C:
    andi. r0, r3, 0xa
    cmpwi r0, 0x8
    beq @L_8007BEFC
    addi r26, r26, 0x1
    cmplwi r26, 0x9
    ble @L_8007BEC4
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BEC4:
    mr r3, r31
    bl OSCreateAlarm
    bl OSDisableInterrupts
    mr r27, r3
    mr r3, r31
    mr r7, r29
    li r6, 0x10
    li r5, 0x0
    bl OSSetAlarm
    addi r3, r31, 0x28
    bl fn_800A221C
    mr r3, r27
    bl OSRestoreInterrupts
    b @L_8007BE58
@L_8007BEFC:
    mr r3, r30
    addi r4, r1, 0x10
    addi r5, r1, 0x9
    bl fn_8025F584
    cmpwi r3, 0x0
    beq @L_8007BF24
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007BF24:
    lwz r4, 0x10(r1)
    addi r0, r28, 0x2388
    lwz r3, 0x358(r31)
    addi r28, r28, 0x4
    xor r3, r4, r3
    stwx r3, r31, r0
    cmplwi r28, 0x1040
    blt @L_8007BE54
    lwz r6, 0x33c4(r31)
    li r3, 0x0
    rlwinm r4, r6, 0, 0, 7
    rlwinm r0, r6, 0, 8, 15
    rlwinm r5, r6, 0, 16, 23
    rlwinm r6, r6, 0, 24, 31
    rlwinm r4, r4, 8, 24, 31
    rlwinm r0, r0, 24, 8, 31
    rlwinm r5, r5, 8, 0, 23
    rlwinm r6, r6, 24, 0, 7
    or r0, r4, r0
    or r0, r5, r0
    or r9, r6, r0
    lis r4, lbl_803FAEF8@ha
    li r0, 0x80
    addi r5, r1, 0x8
    addi r4, r4, lbl_803FAEF8@l
    mtctr r0
@L_8007BF8C:
    rlwinm r0, r3, 0, 24, 31
    lwz r8, 0x354(r31)
    stb r0, 0x8(r1)
    li r7, 0x0
@L_8007BF9C:
    mr r0, r7
    addi r7, r7, 0x1
    lbzx r0, r5, r0
    cmplwi r7, 0x1
    rlwinm r6, r8, 24, 8, 31
    xor r0, r8, r0
    rlwinm r0, r0, 0, 24, 31
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r4, r0
    xor r8, r6, r0
    bne @L_8007BF9C
    cmplw r9, r8
    beq @L_8007C020
    addi r3, r3, 0x1
    rlwinm r0, r3, 0, 24, 31
    lwz r8, 0x354(r31)
    stb r0, 0x8(r1)
    li r7, 0x0
@L_8007BFE4:
    mr r0, r7
    addi r7, r7, 0x1
    lbzx r0, r5, r0
    cmplwi r7, 0x1
    rlwinm r6, r8, 24, 8, 31
    xor r0, r8, r0
    rlwinm r0, r0, 0, 24, 31
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r4, r0
    xor r8, r6, r0
    bne @L_8007BFE4
    cmplw r9, r8
    beq @L_8007C020
    addi r3, r3, 0x1
    bdnz @L_8007BF8C
@L_8007C020:
    cmplwi r3, 0x100
    bne @L_8007C038
    li r0, 0x7
    li r3, 0x7
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007C038:
    lis r3, 0xaa48
    li r6, 0x0
    subi r7, r3, 0x7bde
    lis r3, lbl_803FAEF8@ha
    addi r4, r1, 0x8
    addi r3, r3, lbl_803FAEF8@l
@L_8007C050:
    mr r0, r6
    addi r6, r6, 0x1
    lbzx r0, r4, r0
    cmplwi r6, 0x1
    rlwinm r5, r7, 24, 8, 31
    xor r0, r7, r0
    rlwinm r0, r0, 0, 24, 31
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
    xor r7, r5, r0
    bne @L_8007C050
    rlwinm r3, r7, 0, 0, 7
    rlwinm r0, r7, 0, 8, 15
    rlwinm r4, r7, 0, 16, 23
    rlwinm r5, r7, 0, 24, 31
    rlwinm r3, r3, 8, 24, 31
    rlwinm r0, r0, 24, 8, 31
    rlwinm r4, r4, 8, 0, 23
    rlwinm r5, r5, 24, 0, 7
    or r0, r3, r0
    li r6, 0x0
    or r0, r4, r0
    or r4, r5, r0
    li r0, 0x40f
    mtctr r0
@L_8007C0B4:
    addi r3, r6, 0x2388
    addi r6, r6, 0x4
    lwzx r0, r31, r3
    xor r0, r0, r4
    stwx r0, r31, r3
    bdnz @L_8007C0B4
    lwz r5, 0x33c0(r31)
    li r6, 0x0
    lwz r7, 0x354(r31)
    rlwinm r3, r5, 0, 0, 7
    rlwinm r0, r5, 0, 8, 15
    rlwinm r4, r5, 0, 16, 23
    rlwinm r5, r5, 0, 24, 31
    rlwinm r3, r3, 8, 24, 31
    rlwinm r0, r0, 24, 8, 31
    rlwinm r4, r4, 8, 0, 23
    rlwinm r5, r5, 24, 0, 7
    or r0, r3, r0
    or r0, r4, r0
    or r5, r5, r0
    lis r3, lbl_803FAEF8@ha
    addi r3, r3, lbl_803FAEF8@l
@L_8007C10C:
    mr r4, r6
    addi r6, r6, 0x1
    addi r0, r4, 0x2388
    rlwinm r4, r7, 24, 8, 31
    lbzx r0, r31, r0
    cmplwi r6, 0x1038
    xor r0, r7, r0
    rlwinm r0, r0, 0, 24, 31
    rlwinm r0, r0, 2, 0, 29
    lwzx r0, r3, r0
    xor r7, r4, r0
    bne @L_8007C10C
    cmplw r7, r5
    beq @L_8007C154
    li r0, 0x7
    li r3, 0x7
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007C154:
    li r0, 0x5
    li r3, 0x5
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007C164:
    mr r3, r31
    bl OSCreateAlarm
    bl OSDisableInterrupts
    mr r27, r3
    mr r3, r31
    mr r7, r29
    li r6, 0x10
    li r5, 0x0
    bl OSSetAlarm
    addi r3, r31, 0x28
    bl fn_800A221C
    mr r3, r27
    bl OSRestoreInterrupts
    b @L_8007BDAC
@L_8007C19C:
    mr r3, r30
    addi r4, r1, 0x9
    bl fn_8025F3F4
    cmpwi r3, 0x0
    bne @L_8007C1C0
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007C1C0:
    lbz r3, 0x9(r1)
    rlwinm r0, r3, 0, 26, 27
    cmpwi r0, 0x0
    bne @L_8007C1E0
    li r0, 0x6
    li r3, 0x6
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007C1E0:
    cmpwi r0, 0x30
    bne @L_8007C214
    rlwinm r0, r3, 0, 28, 28
    cmpwi r0, 0x0
    beq @L_8007C204
    li r0, 0x7
    li r3, 0x7
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007C204:
    li r0, 0x8
    li r3, 0x8
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007C214:
    li r0, 0x9
    li r3, 0x9
    stb r0, 0x346(r31)
    b @L_8007C228
@L_8007C224:
    li r3, 0x0
@L_8007C228:
    lmw r25, 0x24(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
