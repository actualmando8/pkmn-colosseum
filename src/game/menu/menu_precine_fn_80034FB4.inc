    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    stfd f31, 0x80(r1)
    psq_st f31, 0x88(r1), 0, 0
    stfd f30, 0x70(r1)
    psq_st f30, 0x78(r1), 0, 0
    stfd f29, 0x60(r1)
    psq_st f29, 0x68(r1), 0, 0
    stfd f28, 0x50(r1)
    psq_st f28, 0x58(r1), 0, 0
    stfd f27, 0x40(r1)
    psq_st f27, 0x48(r1), 0, 0
    stmw r24, 0x20(r1)
    lis r3, lbl_803A3278@ha
    li r0, 0x1
    addi r31, r3, lbl_803A3278@l
    stb r0, lbl_8047A438(r13)
    addi r30, r31, 0xbc
    addi r29, r31, 0x10
    addi r28, r30, 0xb00
    b @L_80035BD4
@L_8003500C:
    cmplwi r0, 0xe
    bgt @L_80035BD4
    lis r3, jumptable_802E50A4@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_802E50A4@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lbz r0, lbl_8047A454(r13)
    cmplwi r0, 0x1
    bne @L_80035044
    lfs f1, lbl_8047BA00(r2)
    li r3, 0x2
    bl fn_801C41C8
@L_80035044:
    li r3, 0xa4
    li r4, 0x1
    bl fn_8010264C
    li r0, 0x1
    mr r27, r3
    stb r0, lbl_8047A454(r13)
    li r3, 0x1
    bl fn_801C40F0
    cmpwi r27, 0x1
    beq @L_80035098
    bge @L_80035080
    cmpwi r27, -0x1
    beq @L_800350A4
    bge @L_8003508C
    b @L_800350BC
@L_80035080:
    cmpwi r27, 0x3
    bge @L_800350BC
    b @L_800350B0
@L_8003508C:
    li r0, 0x2
    stw r0, lbl_8047A458(r13)
    b @L_800350C4
@L_80035098:
    li r0, 0x3
    stw r0, lbl_8047A458(r13)
    b @L_800350C4
@L_800350A4:
    li r0, 0x0
    stw r0, lbl_8047A458(r13)
    b @L_800350C4
@L_800350B0:
    li r0, 0x0
    stw r0, lbl_8047A458(r13)
    b @L_800350C4
@L_800350BC:
    li r0, 0x0
    stw r0, lbl_8047A458(r13)
@L_800350C4:
    lwz r0, lbl_8047A458(r13)
    cmpwi r0, 0x0
    bne @L_800350F8
    li r3, 0x3c7
    bl fn_80166A28
    li r3, 0x8
    li r4, 0x3b54
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80035BD4
@L_800350F8:
    lfs f1, lbl_8047BA00(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0xa4
    bl fn_80102510
    b @L_80035BD4
    lfs f1, lbl_8047BA00(r2)
    li r3, 0x2
    bl fn_801C41C8
    bl fn_801046B8
    mr r4, r3
    mr r9, r29
    li r3, 0xa6
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    addi r24, r31, 0xb0
    mr r27, r3
    li r26, 0x0
    mr r25, r24
@L_80035170:
    rlwinm r4, r26, 0, 24, 31
    mr r3, r27
    bl fn_8012AC08
    bl fn_8011F1A0
    sth r3, 0x0(r25)
    addi r25, r25, 0x2
    addi r26, r26, 0x1
    cmpwi r26, 0x6
    blt @L_80035170
    b @L_800351A0
@L_80035198:
    bl fn_80033278
    stw r3, lbl_8047A458(r13)
@L_800351A0:
    lwz r0, lbl_8047A458(r13)
    cmpwi r0, 0x2
    beq @L_80035198
    li r3, 0x0
    bl fn_801D0AFC
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    mr r26, r3
    li r27, 0x0
@L_800351C8:
    rlwinm r4, r27, 0, 24, 31
    mr r3, r26
    bl fn_8012AC08
    lhz r4, 0x0(r24)
    bl fn_8011DCB4
    addi r24, r24, 0x2
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_800351C8
    lbz r0, lbl_8047A439(r13)
    cmplwi r0, 0x0
    beq @L_80035200
    li r0, 0x0
    stw r0, lbl_8047A458(r13)
@L_80035200:
    lwz r0, lbl_8047A458(r13)
    cmpwi r0, 0x0
    beq @L_80035BD4
    li r3, 0x3c7
    bl fn_80166A28
    lfs f1, lbl_8047BA00(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0xa6
    bl fn_80102510
    b @L_80035BD4
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    addi r25, r31, 0xb0
    mr r26, r3
    li r27, 0x0
@L_8003524C:
    rlwinm r4, r27, 0, 24, 31
    mr r3, r26
    bl fn_8012AC08
    bl fn_8011F1A0
    sth r3, 0x0(r25)
    addi r25, r25, 0x2
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_8003524C
    li r0, 0x0
    addi r4, r31, 0x0
    stw r0, lbl_8047A450(r13)
    li r3, 0xa5
    stb r0, lbl_8047A44A(r13)
    stw r0, lbl_8047A43C(r13)
    stb r0, 0x0(r31)
    stb r0, 0x1(r4)
    stb r0, 0x2(r4)
    stb r0, 0x3(r4)
    stb r0, 0x4(r4)
    stb r0, 0x5(r4)
    stb r0, 0x6(r4)
    stb r0, 0x7(r4)
    stb r0, 0x8(r4)
    stb r0, 0x9(r4)
    stb r0, 0xa(r4)
    stb r0, 0xb(r4)
    stb r0, 0xc(r4)
    stb r0, 0xd(r4)
    stb r0, 0xe(r4)
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_800352DC
    li r3, 0xa5
    bl fn_80102510
@L_800352DC:
    lfs f1, lbl_8047BA00(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0xa5
    li r4, 0x1
    bl fn_8010264C
    li r3, 0x1
    bl fn_801C40F0
    li r0, 0x5
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
    lbz r0, lbl_8047A449(r13)
    cmplwi r0, 0x0
    beq @L_8003531C
    li r3, 0x0
    bl fn_801D0AFC
@L_8003531C:
    lwz r3, lbl_8047A450(r13)
    lwz r0, lbl_8047A44C(r13)
    cmplw r3, r0
    ble @L_800353D8
    li r3, 0x4ce
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    lis r3, 0x99
    lwz r8, lbl_8047A450(r13)
    subi r0, r3, 0x6981
    cmplw r8, r0
    bgt @L_80035354
    b @L_80035358
@L_80035354:
    mr r8, r0
@L_80035358:
    stw r8, lbl_8047A44C(r13)
    li r3, 0x8
    lwz r7, lbl_8047A430(r13)
    li r4, 0x3b8d
    li r5, 0x1
    li r6, 0x0
    stw r8, 0x49c8(r7)
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    lfs f27, lbl_8047B9F8(r2)
    lfd f31, lbl_8047BA08(r2)
    lis r27, 0x4330
    lfd f29, lbl_8047BA10(r2)
    lfs f28, lbl_8047B9FC(r2)
    b @L_800353D0
@L_80035398:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r27, 0x10(r1)
    stw r0, 0x14(r1)
    lfd f0, 0x10(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x1c(r1)
    stw r27, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_800353D0:
    fcmpo cr0, f27, f28
    blt @L_80035398
@L_800353D8:
    li r3, 0x8
    li r4, 0x3b54
    li r5, 0x1
    li r6, 0x1
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x0
    addi r5, r31, 0x0
    stw r0, lbl_8047A43C(r13)
    li r3, 0x0
    li r4, 0x2
    stb r0, 0x0(r31)
    stb r0, 0x1(r5)
    stb r0, 0x2(r5)
    stb r0, 0x3(r5)
    stb r0, 0x4(r5)
    stb r0, 0x5(r5)
    stb r0, 0x6(r5)
    stb r0, 0x7(r5)
    stb r0, 0x8(r5)
    stb r0, 0x9(r5)
    stb r0, 0xa(r5)
    stb r0, 0xb(r5)
    stb r0, 0xc(r5)
    stb r0, 0xd(r5)
    stb r0, 0xe(r5)
    bl fn_80129280
    addi r25, r31, 0xb0
    mr r26, r3
    li r27, 0x0
@L_80035454:
    rlwinm r4, r27, 0, 24, 31
    mr r3, r26
    bl fn_8012AC08
    lhz r4, 0x0(r25)
    bl fn_8011DCB4
    addi r25, r25, 0x2
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_80035454
    li r0, 0x0
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
    bl fn_80034280
    cmpwi r3, 0x1
    beq @L_800354B4
    bge @L_800354A0
    cmpwi r3, 0x0
    bge @L_800354A8
    b @L_800354C0
@L_800354A0:
    cmpwi r3, 0x3
    b @L_800354C0
@L_800354A8:
    li r0, 0x4
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
@L_800354B4:
    li r0, 0x6
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
@L_800354C0:
    lwz r0, 0xbc(r31)
    cmpwi r0, 0x1
    beq @L_800354DC
    bge @L_80035BD4
    cmpwi r0, 0x0
    bge @L_800354E8
    b @L_80035BD4
@L_800354DC:
    li r0, 0x7
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
@L_800354E8:
    li r3, 0x0
    li r4, 0xd
    bl fn_80129280
    addi r4, r31, 0xbc
    li r5, 0x0
    bl fn_800836AC
    cmplwi r3, 0x0
    stw r3, lbl_8047A434(r13)
    beq @L_80035608
    lbz r4, 0x58(r30)
    li r27, 0x4
    lbz r5, 0x24(r30)
    subi r0, r4, 0x1
    lbz r6, 0x26(r30)
    extsb r4, r0
    bl fn_80082EA4
    lbz r0, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_80035600
    lwz r0, lbl_8047A43C(r13)
    li r7, 0x0
    cmpwi r0, 0x0
    ble @L_800355A0
    lbz r6, 0x8(r30)
    addi r5, r31, 0x0
    lbz r3, 0x24(r30)
    lbz r8, 0x26(r30)
    extsb r4, r3
    extsb r3, r8
    mtctr r0
    ble @L_800355A0
@L_80035564:
    lbz r0, 0x0(r5)
    cmplw r6, r0
    bne @L_80035598
    lbz r0, 0x1(r5)
    extsb r0, r0
    cmpw r4, r0
    bne @L_80035598
    lbz r0, 0x2(r5)
    extsb r0, r0
    cmpw r3, r0
    bne @L_80035598
    li r7, 0x1
    b @L_800355A0
@L_80035598:
    addi r5, r5, 0x3
    bdnz @L_80035564
@L_800355A0:
    rlwinm r0, r7, 0, 24, 31
    cmplwi r0, 0x0
    bne @L_800355EC
    lwz r3, lbl_8047A43C(r13)
    addi r8, r31, 0x0
    lbz r6, 0x8(r30)
    mulli r5, r3, 0x3
    addi r0, r3, 0x1
    lbz r4, 0x24(r30)
    lbz r3, 0x26(r30)
    cmpwi r0, 0x5
    add r8, r8, r5
    stb r6, 0x0(r8)
    stb r4, 0x1(r8)
    stb r3, 0x2(r8)
    stw r0, lbl_8047A43C(r13)
    blt @L_800355EC
    li r0, 0x4
    stw r0, lbl_8047A43C(r13)
@L_800355EC:
    rlwinm r0, r7, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8003560C
    li r27, 0x3
    b @L_8003560C
@L_80035600:
    li r27, 0x2
    b @L_8003560C
@L_80035608:
    li r27, 0x2
@L_8003560C:
    cmpwi r27, 0x4
    beq @L_80035648
    bge @L_80035628
    cmpwi r27, 0x2
    beq @L_80035630
    bge @L_8003563C
    b @L_80035654
@L_80035628:
    cmpwi r27, 0x6
    b @L_80035654
@L_80035630:
    li r0, 0x8
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
@L_8003563C:
    li r0, 0x9
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
@L_80035648:
    li r0, 0xa
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
@L_80035654:
    li r0, 0x4
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
    li r3, 0x8
    li r4, 0x3b8a
    li r5, 0x0
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0xa9
    li r4, 0x1
    bl fn_8010264C
    mr r27, r3
    li r3, 0xa9
    bl fn_80102510
    li r3, 0x1
    bl fn_801069FC
    cmpwi r27, 0x0
    beq @L_800356CC
    bge @L_800356AC
    cmpwi r27, -0x1
    bge @L_800356C0
    b @L_800356CC
@L_800356AC:
    cmpwi r27, 0x2
    bge @L_800356CC
    li r0, 0x5
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
@L_800356C0:
    li r0, 0x5
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
@L_800356CC:
    lwz r3, lbl_8047A450(r13)
    lwz r0, lbl_8047A44C(r13)
    cmplw r3, r0
    ble @L_80035788
    li r3, 0x4ce
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    lis r3, 0x99
    lwz r8, lbl_8047A450(r13)
    subi r0, r3, 0x6981
    cmplw r8, r0
    bgt @L_80035704
    b @L_80035708
@L_80035704:
    mr r8, r0
@L_80035708:
    stw r8, lbl_8047A44C(r13)
    li r3, 0x8
    lwz r7, lbl_8047A430(r13)
    li r4, 0x3b8d
    li r5, 0x1
    li r6, 0x0
    stw r8, 0x49c8(r7)
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    lfs f27, lbl_8047B9F8(r2)
    lfd f31, lbl_8047BA08(r2)
    lis r27, 0x4330
    lfd f29, lbl_8047BA10(r2)
    lfs f28, lbl_8047B9FC(r2)
    b @L_80035780
@L_80035748:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r27, 0x18(r1)
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f30, f0, f31
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r27, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f29
    fdivs f0, f0, f30
    fadds f27, f27, f0
@L_80035780:
    fcmpo cr0, f27, f28
    blt @L_80035748
@L_80035788:
    lbz r0, lbl_8047A449(r13)
    cmplwi r0, 0x0
    beq @L_8003579C
    li r3, 0x0
    bl fn_801D0AFC
@L_8003579C:
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    addi r25, r31, 0xb0
    mr r26, r3
    li r27, 0x0
@L_800357B4:
    rlwinm r4, r27, 0, 24, 31
    mr r3, r26
    bl fn_8012AC08
    lhz r4, 0x0(r25)
    bl fn_8011DCB4
    addi r25, r25, 0x2
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_800357B4
    li r0, 0x1
    cmpwi r0, 0x0
    stw r0, lbl_8047A458(r13)
    beq @L_80035BD4
    lfs f1, lbl_8047BA00(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0xa5
    bl fn_80102510
    b @L_80035BD4
    lhz r0, 0x0(r28)
    lwz r3, 0xafc(r30)
    cmplwi r0, 0x0
    stw r3, lbl_8047A444(r13)
    beq @L_8003582C
    mr r4, r28
    addi r3, r31, 0x10
    bl fn_800F9E70
    b @L_80035860
@L_8003582C:
    rlwinm r3, r3, 0, 16, 31
    bl fn_801F6738
    cmplwi r3, 0x0
    beq @L_80035854
    bl fn_801F6600
    bl fn_800FA280
    mr r4, r3
    addi r3, r31, 0x10
    bl fn_800F9E70
    b @L_80035860
@L_80035854:
    addi r3, r31, 0x10
    la r4, lbl_8047BA1C(r2)
    bl fn_800F9E70
@L_80035860:
    mr r4, r29
    li r3, 0x4d
    bl fn_80132A38
    li r3, 0x8
    li r4, 0x3b57
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x5
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
    li r3, 0x8
    li r4, 0x3b5a
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x5
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
    li r3, 0x8
    li r4, 0x3b5d
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    li r0, 0x5
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
    addi r3, r1, 0x8
    addi r4, r1, 0x9
    addi r5, r1, 0xa
    bl fn_80034B5C
    lbz r4, 0xa(r1)
    addi r0, r31, 0xbc
    lbz r3, 0x8(r1)
    extsb r5, r4
    lbz r4, 0x9(r1)
    mulli r5, r5, 0x28
    addi r5, r5, 0x3ac
    add r5, r0, r5
    bl fn_80034830
    lwz r4, lbl_8047A450(r13)
    lis r3, 0x99
    subi r0, r3, 0x6981
    addi r4, r4, 0x1
    cmplw r4, r0
    ble @L_80035934
    mr r4, r0
@L_80035934:
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x8
    li r4, 0x3b5e
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    lwz r0, lbl_8047A450(r13)
    cmplwi r0, 0x0
    bne @L_8003596C
    li r3, 0x0
    bl fn_801D0AFC
@L_8003596C:
    li r0, 0xb
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
    bl fn_800327FC
    b @L_80035BD4
    lwz r3, lbl_8047A450(r13)
    li r4, 0x0
    lwz r0, lbl_8047A44C(r13)
    stb r4, lbl_8047A44A(r13)
    cmplw r3, r0
    ble @L_800359EC
    li r3, 0x4ce
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    lis r3, 0x99
    lwz r8, lbl_8047A450(r13)
    subi r0, r3, 0x6981
    cmplw r8, r0
    bgt @L_800359C0
    b @L_800359C4
@L_800359C0:
    mr r8, r0
@L_800359C4:
    stw r8, lbl_8047A44C(r13)
    li r3, 0x8
    lwz r7, lbl_8047A430(r13)
    li r4, 0x3b75
    li r5, 0x1
    li r6, 0x0
    stw r8, 0x49c8(r7)
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_800359EC:
    li r0, 0x0
    addi r4, r31, 0x0
    stw r0, lbl_8047A43C(r13)
    li r3, 0x0
    stb r0, 0x0(r31)
    stb r0, 0x1(r4)
    stb r0, 0x2(r4)
    stb r0, 0x3(r4)
    stb r0, 0x4(r4)
    stb r0, 0x5(r4)
    stb r0, 0x6(r4)
    stb r0, 0x7(r4)
    stb r0, 0x8(r4)
    stb r0, 0x9(r4)
    stb r0, 0xa(r4)
    stb r0, 0xb(r4)
    stb r0, 0xc(r4)
    stb r0, 0xd(r4)
    stb r0, 0xe(r4)
    bl fn_801D0AFC
    li r3, 0x8
    li r4, 0x3bd7
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0xa9
    li r4, 0x1
    bl fn_8010264C
    mr r27, r3
    li r3, 0xa9
    bl fn_80102510
    li r3, 0x1
    bl fn_801069FC
    cmpwi r27, 0x0
    beq @L_80035A94
    bge @L_80035A88
    cmpwi r27, -0x1
    bge @L_80035AAC
    b @L_80035AB4
@L_80035A88:
    cmpwi r27, 0x2
    bge @L_80035AB4
    b @L_80035AA0
@L_80035A94:
    li r0, 0x3
    stw r0, lbl_8047A458(r13)
    b @L_80035BD4
@L_80035AA0:
    li r0, 0x1
    stw r0, lbl_8047A458(r13)
    b @L_80035AB4
@L_80035AAC:
    li r0, 0x1
    stw r0, lbl_8047A458(r13)
@L_80035AB4:
    lfs f27, lbl_8047B9F8(r2)
    lfd f28, lbl_8047BA08(r2)
    lis r27, 0x4330
    lfd f30, lbl_8047BA10(r2)
    lfs f31, lbl_8047B9FC(r2)
    b @L_80035B04
@L_80035ACC:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r27, 0x18(r1)
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x14(r1)
    stw r27, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
@L_80035B04:
    fcmpo cr0, f27, f31
    blt @L_80035ACC
    li r3, 0x0
    li r4, 0x2
    bl fn_80129280
    addi r25, r31, 0xb0
    mr r26, r3
    li r27, 0x0
@L_80035B24:
    rlwinm r4, r27, 0, 24, 31
    mr r3, r26
    bl fn_8012AC08
    lhz r4, 0x0(r25)
    bl fn_8011DCB4
    addi r25, r25, 0x2
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_80035B24
    lwz r0, lbl_8047A458(r13)
    cmpwi r0, 0x0
    beq @L_80035BD4
    lfs f1, lbl_8047BA00(r2)
    li r3, 0x3
    bl fn_801C41C8
    li r3, 0x1
    bl fn_801C40F0
    li r3, 0xa5
    bl fn_80102510
    b @L_80035BD4
    bl fn_8003258C
    b @L_80035BD4
    lwz r0, lbl_8047A450(r13)
    cmplwi r0, 0x1e
    ble @L_80035BA8
    li r3, 0x8
    li r4, 0x3b5c
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
    b @L_80035BC4
@L_80035BA8:
    li r3, 0x8
    li r4, 0x3b5f
    li r5, 0x1
    li r6, 0x0
    bl fn_80106D3C
    li r3, 0x1
    bl fn_801069FC
@L_80035BC4:
    li r3, 0x1
    li r0, 0xb
    stb r3, lbl_8047A44A(r13)
    stw r0, lbl_8047A458(r13)
@L_80035BD4:
    lwz r0, lbl_8047A458(r13)
    cmpwi r0, 0x0
    bgt @L_8003500C
    bl fn_800FF52C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80035BF8
    bl fn_800FF660
    b @L_80035C04
@L_80035BF8:
    li r3, 0x80
    li r4, 0x0
    bl fn_80113828
@L_80035C04:
    li r0, 0x0
    stb r0, lbl_8047A438(r13)
    psq_l f31, 0x88(r1), 0, 0
    lfd f31, 0x80(r1)
    psq_l f30, 0x78(r1), 0, 0
    lfd f30, 0x70(r1)
    psq_l f29, 0x68(r1), 0, 0
    lfd f29, 0x60(r1)
    psq_l f28, 0x58(r1), 0, 0
    lfd f28, 0x50(r1)
    psq_l f27, 0x48(r1), 0, 0
    lfd f27, 0x40(r1)
    lmw r24, 0x20(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
