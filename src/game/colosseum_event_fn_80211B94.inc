    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    rlwinm. r0, r5, 0, 24, 31
    stmw r22, 0x18(r1)
    mr r23, r5
    lwz r24, -21008(r13)
    lwz r26, -20980(r13)
    stw r4, -21008(r13)
    lbz r25, -21004(r13)
    bne @L_80211BC8
    li r0, 0x0
    stb r0, -21004(r13)
@L_80211BC8:
    lis r4, -32728
    stw r3, -20980(r13)
    rlwinm r31, r23, 0, 24, 31
    addi r30, r4, -24564
@L_80211BD8:
    lwz r3, -21008(r13)
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r12, r30, r0
    mtctr r12
    bctrl
    cmplwi r31, 0x0
    beq @L_80211DC8
    lbz r0, -21004(r13)
    cmplwi r0, 0x1
    bne @L_80211CF8
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, 284
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lwz r27, -20980(r13)
    lis r4, -32712
    lbz r28, -21004(r13)
    li r0, 0x0
    lis r3, -32728
    lwz r29, -21008(r13)
    addi r4, r4, -30824
    stb r0, -21004(r13)
    addi r22, r3, -24564
    stw r4, -21008(r13)
    stw r27, -20980(r13)
@L_80211C48:
    lwz r3, -21008(r13)
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 2, 0, 29
    lwzx r12, r22, r0
    mtctr r12
    bctrl
    lbz r0, -21004(r13)
    cmplwi r0, 0x1
    beq @L_80211C74
    cmplwi r0, 0x2
    bne @L_80211C48
@L_80211C74:
    li r0, 0x1
    lis r3, -32733
    stw r27, -20980(r13)
    addi r4, r3, -7348
    addi r5, r1, 0x9
    li r3, 0x0
    stb r28, -21004(r13)
    li r6, 0x0
    stw r29, -21008(r13)
    stb r0, 0x9(r1)
    bl fn_801F37B0
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, -7688
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, 136
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r0, 0x0
    lis r3, -32733
    stb r0, 0x8(r1)
    addi r4, r3, -5220
    addi r5, r1, 0x8
    li r3, 0x0
    li r6, 0x0
    bl fn_801F37B0
    li r0, 0x2
    stb r0, -21004(r13)
@L_80211CF8:
    lbz r0, -21004(r13)
    cmplwi r0, 0x2
    bne @L_80211BD8
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r27, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    lis r4, -32735
    mr r22, r3
    addi r4, r4, 13988
    li r3, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lis r4, 0xf1e9
    lis r3, -32712
    addi r3, r3, -24744
    lwz r5, -21000(r13)
    subi r4, r4, 0x6d51
    li r0, 0x0
    addis r8, r3, 0x1
    addi r9, r13, -31400
    and r5, r5, r4
    stb r0, 0x3(r9)
    mr r3, r27
    li r4, 0x0
    stw r5, -21000(r13)
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    stb r0, -20987(r13)
    stb r0, 0x4(r9)
    stb r0, 0x6002(r8)
    stb r0, 0x60a1(r8)
    bl fn_801254B4
    mr r3, r27
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    mr r3, r22
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    b @L_80211DDC
@L_80211DC8:
    lbz r0, -21004(r13)
    cmplwi r0, 0x1
    beq @L_80211DDC
    cmplwi r0, 0x2
    bne @L_80211BD8
@L_80211DDC:
    rlwinm. r0, r23, 0, 24, 31
    beq @L_80211DF8
    lwz r0, -21000(r13)
    rlwinm r0, r0, 0, 23, 21
    stw r0, -21000(r13)
    rlwinm r0, r0, 0, 13, 11
    stw r0, -21000(r13)
@L_80211DF8:
    stw r26, -20980(r13)
    stb r25, -21004(r13)
    stw r24, -21008(r13)
    lmw r22, 0x18(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
