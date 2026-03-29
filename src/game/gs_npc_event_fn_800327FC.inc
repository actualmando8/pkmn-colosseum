    nofralloc
    stwu r1, -0x2c0(r1)
    mflr r0
    stw r0, 0x2c4(r1)
    stfd f31, 0x2b0(r1)
    psq_st f31, 0x2b8(r1), 0, 0
    stfd f30, 0x2a0(r1)
    psq_st f30, 0x2a8(r1), 0, 0
    stfd f29, 0x290(r1)
    psq_st f29, 0x298(r1), 0, 0
    stfd f28, 0x280(r1)
    psq_st f28, 0x288(r1), 0, 0
    stfd f27, 0x270(r1)
    psq_st f27, 0x278(r1), 0, 0
    stmw r14, 0x228(r1)
    lbz r0, lbl_8047A44A(r13)
    cmplwi r0, 0x0
    beq @L_80032BB8
    lwz r4, lbl_8047A430(r13)
    li r14, 0x0
    li r0, 0xa
    mtctr r0
    @L_80032850:
    addi r3, r4, 0x41e4
    cmplwi r3, 0x0
    beq @L_80032894
    lhz r0, 0x6(r3)
    cmplwi r0, 0x0
    bne @L_80032874
    lhz r0, 0x8(r3)
    cmplwi r0, 0x0
    beq @L_80032894
    @L_80032874:
    subfic r3, r14, 0xa
    bl fn_800E0C04
    add r3, r14, r3
    lwz r0, lbl_8047A430(r13)
    mulli r3, r3, 0xca
    addi r3, r3, 0x41e4
    add r3, r0, r3
    b @L_800328A0
    @L_80032894:
    addi r4, r4, 0xca
    addi r14, r14, 0x1
    bdnz @L_80032850
    @L_800328A0:
    li r0, 0x19
    addi r5, r1, 0xe0
    subi r4, r3, 0x4
    mtctr r0
    @L_800328B0:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_800328B0
    lhz r0, 0x4(r4)
    addi r3, r1, 0x14
    addi r4, r1, 0xea
    sth r0, 0x4(r5)
    bl fn_800F9E70
    lwz r15, 0x150(r1)
    li r0, 0x0
    lwz r3, 0x148(r1)
    stw r15, 0x220(r1)
    lwz r15, 0x154(r1)
    stw r3, 0x7e(r1)
    lwz r3, 0x220(r1)
    stw r15, 0x1c0(r1)
    lhz r15, 0x158(r1)
    stw r3, 0x86(r1)
    lwz r3, 0x1c0(r1)
    stw r15, 0x1c4(r1)
    lwz r15, 0x15a(r1)
    stw r3, 0x8a(r1)
    lwz r3, 0x1c4(r1)
    stw r15, 0x1c8(r1)
    lwz r15, 0x15e(r1)
    sth r3, 0x8e(r1)
    lwz r3, 0x1c8(r1)
    stw r15, 0x1cc(r1)
    lwz r15, 0x162(r1)
    stw r3, 0x90(r1)
    lwz r3, 0x1cc(r1)
    stw r15, 0x1d0(r1)
    lwz r15, 0x166(r1)
    stw r3, 0x94(r1)
    lwz r3, 0x1d0(r1)
    stw r15, 0x1d4(r1)
    lwz r15, 0x16a(r1)
    stw r3, 0x98(r1)
    lwz r3, 0x1d4(r1)
    stw r15, 0x1d8(r1)
    lwz r15, 0x16e(r1)
    stw r3, 0x9c(r1)
    lwz r3, 0x1d8(r1)
    stw r15, 0x1dc(r1)
    lwz r15, 0x172(r1)
    stw r3, 0xa0(r1)
    lwz r3, 0x1dc(r1)
    stw r15, 0x1e0(r1)
    lwz r15, 0x176(r1)
    stw r3, 0xa4(r1)
    lwz r3, 0x1e0(r1)
    stw r15, 0x1e4(r1)
    lwz r15, 0x17a(r1)
    stw r3, 0xa8(r1)
    lwz r3, 0x1e4(r1)
    stw r15, 0x1e8(r1)
    lwz r15, 0x17e(r1)
    stw r3, 0xac(r1)
    lwz r3, 0x1e8(r1)
    stw r15, 0x1ec(r1)
    lhz r15, 0x182(r1)
    stw r3, 0xb0(r1)
    lwz r3, 0x1ec(r1)
    stw r15, 0x1f0(r1)
    lwz r15, 0x184(r1)
    stw r3, 0xb4(r1)
    lwz r3, 0x1f0(r1)
    stw r15, 0x1f4(r1)
    lwz r15, 0x188(r1)
    sth r3, 0xb8(r1)
    lwz r3, 0x1f4(r1)
    stw r15, 0x1f8(r1)
    lwz r15, 0x18c(r1)
    stw r3, 0xba(r1)
    lwz r3, 0x1f8(r1)
    stw r15, 0x1fc(r1)
    lwz r15, 0x190(r1)
    stw r3, 0xbe(r1)
    lwz r3, 0x1fc(r1)
    stw r15, 0x200(r1)
    lwz r15, 0x194(r1)
    stw r3, 0xc2(r1)
    lwz r3, 0x200(r1)
    stw r15, 0x204(r1)
    lwz r15, 0x198(r1)
    stw r3, 0xc6(r1)
    lwz r3, 0x204(r1)
    stw r15, 0x208(r1)
    lwz r15, 0x19c(r1)
    stw r3, 0xca(r1)
    lwz r3, 0x208(r1)
    stw r15, 0x20c(r1)
    lwz r15, 0x1a0(r1)
    stw r3, 0xce(r1)
    lwz r3, 0x20c(r1)
    stw r15, 0x210(r1)
    lwz r15, 0x1a4(r1)
    stw r3, 0xd2(r1)
    lwz r3, 0x210(r1)
    stw r15, 0x214(r1)
    lwz r15, 0x1a8(r1)
    stw r3, 0xd6(r1)
    lwz r3, 0x214(r1)
    stw r15, 0x218(r1)
    lhz r15, 0x1ac(r1)
    stw r3, 0xda(r1)
    lwz r3, 0x218(r1)
    lbz r16, 0xf6(r1)
    lhz r17, 0xf8(r1)
    lhz r18, 0xfa(r1)
    lhz r19, 0xfc(r1)
    lhz r20, 0xfe(r1)
    lbz r21, 0x100(r1)
    lhz r22, 0x102(r1)
    lbz r23, 0x104(r1)
    lwz r24, 0x106(r1)
    lwz r25, 0x10a(r1)
    lwz r26, 0x10e(r1)
    lwz r27, 0x112(r1)
    lwz r28, 0x116(r1)
    lwz r29, 0x11a(r1)
    lwz r30, 0x11e(r1)
    lwz r31, 0x122(r1)
    lwz r12, 0x126(r1)
    lwz r11, 0x12a(r1)
    lhz r10, 0x12e(r1)
    lwz r9, 0x130(r1)
    lwz r8, 0x134(r1)
    lwz r7, 0x138(r1)
    lwz r6, 0x13c(r1)
    lwz r5, 0x140(r1)
    lwz r4, 0x144(r1)
    lwz r14, 0x14c(r1)
    stw r3, 0xde(r1)
    mr r3, r15
    stw r15, 0x21c(r1)
    stb r16, 0x20(r1)
    stb r0, 0x21(r1)
    stb r0, 0x22(r1)
    stb r0, 0x23(r1)
    stb r0, 0x24(r1)
    sth r17, 0x26(r1)
    sth r18, 0x28(r1)
    sth r19, 0x2a(r1)
    sth r20, 0x2c(r1)
    stw r21, 0x30(r1)
    sth r22, 0x34(r1)
    sth r0, 0x36(r1)
    stb r23, 0x38(r1)
    stw r24, 0x3c(r1)
    stw r25, 0x40(r1)
    stw r26, 0x44(r1)
    stw r27, 0x48(r1)
    stw r28, 0x4c(r1)
    stw r29, 0x50(r1)
    stw r30, 0x54(r1)
    stw r31, 0x58(r1)
    stw r12, 0x5c(r1)
    stw r11, 0x60(r1)
    sth r10, 0x64(r1)
    stw r9, 0x66(r1)
    stw r8, 0x6a(r1)
    stw r7, 0x6e(r1)
    stw r6, 0x72(r1)
    stw r5, 0x76(r1)
    stw r4, 0x7a(r1)
    stw r14, 0x82(r1)
    sth r3, 0xe2(r1)
    lis r3, lbl_803A3278@ha
    stw r0, lbl_8047A43C(r13)
    addi r6, r3, lbl_803A3278@l
    addi r3, r1, 0x14
    stb r0, 0x0(r6)
    addi r5, r1, 0x3c
    li r4, 0x209
    stb r0, 0x1(r6)
    stb r0, 0x2(r6)
    stb r0, 0x3(r6)
    stb r0, 0x4(r6)
    stb r0, 0x5(r6)
    stb r0, 0x6(r6)
    stb r0, 0x7(r6)
    stb r0, 0x8(r6)
    stb r0, 0x9(r6)
    stb r0, 0xa(r6)
    stb r0, 0xb(r6)
    stb r0, 0xc(r6)
    stb r0, 0xd(r6)
    stb r0, 0xe(r6)
    bl fn_80032ED8
    mr r15, r3
    b @L_80032DC8
    @L_80032BB8:
    lis r3, lbl_803A3334@ha
    li r16, 0x0
    addi r3, r3, lbl_803A3334@l
    li r15, 0x1
    lbz r19, 0x24(r3)
    mr r17, r15
    lbz r18, 0x26(r3)
    mr r20, r16
    b @L_80032C2C
    @L_80032BDC:
    mr r5, r20
    li r3, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r14, r3
    bl fn_80123FBC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80032C28
    mr r3, r14
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    clrlwi r3, r3, 24
    clrlwi r0, r17, 24
    cmplw r0, r3
    bge @L_80032C28
    mr r17, r3
    @L_80032C28:
    addi r20, r20, 0x1
    @L_80032C2C:
    clrlwi r0, r20, 16
    cmplwi r0, 0x6
    blt @L_80032BDC
    lis r3, lbl_803A3334@ha
    addi r21, r1, 0x8
    addi r14, r3, lbl_803A3334@l
    clrlwi r22, r17, 24
    mr r20, r14
    li r17, 0x0
    mr r23, r21
    b @L_80032D30
    @L_80032C58:
    lwz r3, lbl_8047A434(r13)
    mr r5, r19
    mr r6, r18
    extsb r4, r17
    bl fn_80082EA4
    lbz r0, 0x5b(r20)
    li r4, 0x0
    mr r5, r4
    extsb r0, r0
    mulli r0, r0, 0x28
    add r6, r14, r0
    b @L_80032CC0
    @L_80032C88:
    clrlwi r3, r5, 16
    addi r0, r3, 0x3b9
    lbzx r0, r6, r0
    extsb r0, r0
    cmpwi r0, 0x0
    blt @L_80032CBC
    mulli r3, r0, 0x2a
    clrlwi r0, r4, 24
    add r3, r14, r3
    lbz r3, 0x517(r3)
    cmplw r0, r3
    bge @L_80032CBC
    mr r4, r3
    @L_80032CBC:
    addi r5, r5, 0x1
    @L_80032CC0:
    clrlwi r0, r5, 16
    cmplwi r0, 0x4
    blt @L_80032C88
    clrlwi r3, r4, 24
    extsb r0, r16
    subf r3, r3, r22
    srawi r4, r3, 31
    slwi r0, r0, 2
    xor r3, r4, r3
    subf r3, r4, r3
    stw r3, 0x0(r21)
    lwz r3, 0x0(r21)
    lwzx r0, r23, r0
    cmpw r3, r0
    bge @L_80032D08
    extsb r16, r17
    li r15, 0x1
    b @L_80032D24
    @L_80032D08:
    bne @L_80032D24
    addi r15, r15, 0x1
    mr r3, r15
    bl fn_800E0C04
    cmplwi r3, 0x0
    bne @L_80032D24
    extsb r16, r17
    @L_80032D24:
    addi r20, r20, 0x1
    addi r21, r21, 0x4
    addi r17, r17, 0x1
    @L_80032D30:
    lbz r0, 0x58(r14)
    extsb r0, r0
    cmpw r17, r0
    blt @L_80032C58
    lis r3, lbl_803A3334@ha
    extsb r0, r16
    addi r6, r3, lbl_803A3334@l
    stb r16, lbl_8047A448(r13)
    add r4, r6, r0
    addi r3, r1, 0x14
    lbz r0, 0x5b(r4)
    li r4, 0x209
    li r5, 0x0
    extsb r0, r0
    mulli r0, r0, 0x28
    add r16, r6, r0
    lwz r15, 0x3ac(r16)
    lwz r14, 0x3b0(r16)
    lwz r12, 0x3b4(r16)
    lwz r11, 0x3b8(r16)
    lwz r10, 0x3bc(r16)
    lwz r9, 0x3c0(r16)
    lwz r8, 0x3c4(r16)
    lwz r7, 0x3c8(r16)
    lwz r6, 0x3cc(r16)
    lwz r0, 0x3d0(r16)
    stw r15, 0x14(r1)
    stw r14, 0x18(r1)
    stw r12, 0x1c(r1)
    stw r11, 0x20(r1)
    stw r10, 0x24(r1)
    stw r9, 0x28(r1)
    stw r8, 0x2c(r1)
    stw r7, 0x30(r1)
    stw r6, 0x34(r1)
    stw r0, 0x38(r1)
    bl fn_80032ED8
    mr r15, r3
    @L_80032DC8:
    li r3, 0xa5
    bl fn_80102620
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80032DE4
    li r3, 0xa5
    bl fn_80102510
    @L_80032DE4:
    lfs f1, lbl_8047BA00(r2)
    li r3, 0x2
    bl fn_801C41C8
    li r3, 0xa5
    li r4, 0x1
    bl fn_8010264C
    li r3, 0x1
    bl fn_801C40F0
    li r0, 0x1
    lfs f27, lbl_8047B9F8(r2)
    stb r0, lbl_8047A449(r13)
    lfd f28, lbl_8047BA08(r2)
    lis r14, 0x4330
    lfd f30, lbl_8047BA10(r2)
    lfs f31, lbl_8047BA18(r2)
    b @L_80032E5C
    @L_80032E24:
    bl fn_800F0308
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    stw r14, 0x1b0(r1)
    stw r0, 0x1b4(r1)
    lfd f0, 0x1b0(r1)
    fsubs f29, f0, f28
    bl fn_800D3088
    stw r3, 0x1bc(r1)
    stw r14, 0x1b8(r1)
    lfd f0, 0x1b8(r1)
    fsubs f0, f0, f30
    fdivs f0, f0, f29
    fadds f27, f27, f0
    @L_80032E5C:
    fcmpo cr0, f27, f31
    blt @L_80032E24
    cmpwi r15, 0x3
    beq @L_80032E88
    bge @L_80032E94
    cmpwi r15, 0x2
    bge @L_80032E7C
    b @L_80032E94
    @L_80032E7C:
    li r0, 0xd
    stw r0, lbl_8047A458(r13)
    b @L_80032E9C
    @L_80032E88:
    li r0, 0xc
    stw r0, lbl_8047A458(r13)
    b @L_80032E9C
    @L_80032E94:
    li r0, 0xc
    stw r0, lbl_8047A458(r13)
    @L_80032E9C:
    psq_l f31, 0x2b8(r1), 0, 0
    lfd f31, 0x2b0(r1)
    psq_l f30, 0x2a8(r1), 0, 0
    lfd f30, 0x2a0(r1)
    psq_l f29, 0x298(r1), 0, 0
    lfd f29, 0x290(r1)
    psq_l f28, 0x288(r1), 0, 0
    lfd f28, 0x280(r1)
    psq_l f27, 0x278(r1), 0, 0
    lfd f27, 0x270(r1)
    lmw r14, 0x228(r1)
    lwz r0, 0x2c4(r1)
    mtlr r0
    addi r1, r1, 0x2c0
    blr
