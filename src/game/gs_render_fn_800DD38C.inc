    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stw r31, 0xac(r1)
    stw r30, 0xa8(r1)
    mr r30, r3
    bne cr1, @L_800DD3C8
    stfd f1, 0x28(r1)
    stfd f2, 0x30(r1)
    stfd f3, 0x38(r1)
    stfd f4, 0x40(r1)
    stfd f5, 0x48(r1)
    stfd f6, 0x50(r1)
    stfd f7, 0x58(r1)
    stfd f8, 0x60(r1)
    @L_800DD3C8:
    lbz r0, lbl_8047AB11(r13)
    addi r12, r1, 0xb8
    addi r11, r1, 0x8
    lis r31, 0x100
    cmplwi r0, 0x0
    stw r3, 0x8(r1)
    stw r4, 0xc(r1)
    stw r5, 0x10(r1)
    stw r6, 0x14(r1)
    stw r7, 0x18(r1)
    stw r8, 0x1c(r1)
    stw r9, 0x20(r1)
    stw r10, 0x24(r1)
    stw r31, 0x68(r1)
    stw r12, 0x6c(r1)
    stw r11, 0x70(r1)
    beq @L_800DD44C
    bl OSGetTime
    addi r5, r1, 0x74
    bl fn_800A2998
    lwz r4, 0x84(r1)
    lis r3, lbl_80400F30@ha
    lis r5, lbl_802704B4@ha
    lwz r7, 0x80(r1)
    addi r6, r4, 0x1
    lwz r8, 0x7c(r1)
    lwz r9, 0x78(r1)
    addi r3, r3, lbl_80400F30@l
    lwz r10, 0x74(r1)
    addi r5, r5, lbl_802704B4@l
    li r4, 0x14
    crxor 6, 6, 6
    bl fn_800DE09C
    @L_800DD44C:
    lis r3, lbl_80400F44@ha
    mr r5, r30
    addi r3, r3, lbl_80400F44@l
    addi r6, r1, 0x68
    li r4, 0xff
    bl fn_800DE680
    lwz r0, lbl_8047AAFC(r13)
    lis r3, lbl_80400F44@ha
    addi r3, r3, lbl_80400F44@l
    li r4, 0x0
    cmplwi r0, 0x0
    stb r4, 0xff(r3)
    beq @L_800DD934
    lbz r0, lbl_8047AB11(r13)
    cmplwi r0, 0x0
    beq @L_800DD4B0
    bl strlen
    lis r4, lbl_80400F30@ha
    mr r31, r3
    addi r3, r4, lbl_80400F30@l
    bl strlen
    addi r0, r31, 0x1
    add r0, r3, r0
    clrlwi r31, r0, 16
    b @L_800DD4BC
    @L_800DD4B0:
    bl strlen
    addi r0, r3, 0x1
    clrlwi r31, r0, 16
    @L_800DD4BC:
    clrlwi r3, r31, 16
    @L_800DD4C0:
    lwz r9, lbl_8047AB08(r13)
    li r0, 0x1
    lwz r4, lbl_8047AB0C(r13)
    cmplw r9, r4
    blt @L_800DD4D8
    li r0, 0x0
    @L_800DD4D8:
    cmplwi r9, 0x0
    li r7, 0x0
    lwz r10, lbl_8047AB00(r13)
    mr r6, r7
    ble @L_800DD588
    cmplwi r9, 0x8
    subi r5, r9, 0x8
    ble @L_800DD55C
    addi r4, r5, 0x7
    mr r8, r10
    srwi r4, r4, 3
    mtctr r4
    cmplwi r5, 0x0
    ble @L_800DD55C
    @L_800DD510:
    lhz r5, 0x0(r8)
    addi r6, r6, 0x8
    lhz r4, 0x2(r8)
    add r7, r7, r5
    lhz r5, 0x4(r8)
    add r7, r7, r4
    lhz r4, 0x6(r8)
    add r7, r7, r5
    lhz r5, 0x8(r8)
    add r7, r7, r4
    lhz r4, 0xa(r8)
    add r7, r7, r5
    lhz r5, 0xc(r8)
    add r7, r7, r4
    lhz r4, 0xe(r8)
    add r7, r7, r5
    addi r8, r8, 0x10
    add r7, r7, r4
    bdnz @L_800DD510
    @L_800DD55C:
    slwi r5, r6, 1
    subf r4, r6, r9
    add r5, r10, r5
    mtctr r4
    cmplw r6, r9
    bge @L_800DD588
    @L_800DD574:
    lhz r4, 0x0(r5)
    addi r5, r5, 0x2
    addi r6, r6, 0x1
    add r7, r7, r4
    bdnz @L_800DD574
    @L_800DD588:
    lwz r5, lbl_8047AB04(r13)
    add r4, r3, r7
    cmplw r4, r5
    blt @L_800DD59C
    li r0, 0x0
    @L_800DD59C:
    clrlwi. r4, r0, 24
    bne @L_800DD7FC
    lhz r4, 0x0(r10)
    lwz r8, lbl_8047AAFC(r13)
    subf r6, r4, r5
    add r7, r8, r4
    subf. r5, r8, r7
    subf r4, r8, r7
    neg r5, r4
    beq @L_800DD5C8
    mr r5, r4
    @L_800DD5C8:
    cmplwi r5, 0x4
    blt @L_800DD6E0
    srwi. r4, r6, 2
    clrlwi r6, r6, 30
    mr r5, r4
    beq @L_800DD658
    srwi. r4, r4, 3
    mtctr r4
    beq @L_800DD640
    @L_800DD5EC:
    lwz r4, 0x0(r7)
    stw r4, 0x0(r8)
    lwz r4, 0x4(r7)
    stw r4, 0x4(r8)
    lwz r4, 0x8(r7)
    stw r4, 0x8(r8)
    lwz r4, 0xc(r7)
    stw r4, 0xc(r8)
    lwz r4, 0x10(r7)
    stw r4, 0x10(r8)
    lwz r4, 0x14(r7)
    stw r4, 0x14(r8)
    lwz r4, 0x18(r7)
    stw r4, 0x18(r8)
    lwz r4, 0x1c(r7)
    addi r7, r7, 0x20
    stw r4, 0x1c(r8)
    addi r8, r8, 0x20
    bdnz @L_800DD5EC
    andi. r5, r5, 0x7
    beq @L_800DD658
    @L_800DD640:
    mtctr r5
    @L_800DD644:
    lwz r4, 0x0(r7)
    addi r7, r7, 0x4
    stw r4, 0x0(r8)
    addi r8, r8, 0x4
    bdnz @L_800DD644
    @L_800DD658:
    cmplwi r6, 0x0
    mr r5, r6
    beq @L_800DD764
    srwi. r4, r6, 3
    mtctr r4
    beq @L_800DD6C4
    @L_800DD670:
    lbz r4, 0x0(r7)
    stb r4, 0x0(r8)
    lbz r4, 0x1(r7)
    stb r4, 0x1(r8)
    lbz r4, 0x2(r7)
    stb r4, 0x2(r8)
    lbz r4, 0x3(r7)
    stb r4, 0x3(r8)
    lbz r4, 0x4(r7)
    stb r4, 0x4(r8)
    lbz r4, 0x5(r7)
    stb r4, 0x5(r8)
    lbz r4, 0x6(r7)
    stb r4, 0x6(r8)
    lbz r4, 0x7(r7)
    addi r7, r7, 0x8
    stb r4, 0x7(r8)
    addi r8, r8, 0x8
    bdnz @L_800DD670
    andi. r5, r5, 0x7
    beq @L_800DD764
    @L_800DD6C4:
    mtctr r5
    @L_800DD6C8:
    lbz r4, 0x0(r7)
    addi r7, r7, 0x1
    stb r4, 0x0(r8)
    addi r8, r8, 0x1
    bdnz @L_800DD6C8
    b @L_800DD764
    @L_800DD6E0:
    cmplwi r6, 0x0
    mr r5, r6
    beq @L_800DD764
    srwi. r4, r6, 3
    mtctr r4
    beq @L_800DD74C
    @L_800DD6F8:
    lbz r4, 0x0(r7)
    stb r4, 0x0(r8)
    lbz r4, 0x1(r7)
    stb r4, 0x1(r8)
    lbz r4, 0x2(r7)
    stb r4, 0x2(r8)
    lbz r4, 0x3(r7)
    stb r4, 0x3(r8)
    lbz r4, 0x4(r7)
    stb r4, 0x4(r8)
    lbz r4, 0x5(r7)
    stb r4, 0x5(r8)
    lbz r4, 0x6(r7)
    stb r4, 0x6(r8)
    lbz r4, 0x7(r7)
    addi r7, r7, 0x8
    stb r4, 0x7(r8)
    addi r8, r8, 0x8
    bdnz @L_800DD6F8
    andi. r5, r5, 0x7
    beq @L_800DD764
    @L_800DD74C:
    mtctr r5
    @L_800DD750:
    lbz r4, 0x0(r7)
    addi r7, r7, 0x1
    stb r4, 0x0(r8)
    addi r8, r8, 0x1
    bdnz @L_800DD750
    @L_800DD764:
    lwz r4, lbl_8047AB08(r13)
    lwz r7, lbl_8047AB00(r13)
    subic. r6, r4, 0x1
    addi r5, r7, 0x2
    beq @L_800DD7F0
    srwi. r4, r6, 3
    mtctr r4
    beq @L_800DD7D8
    @L_800DD784:
    lhz r4, 0x0(r5)
    sth r4, 0x0(r7)
    lhz r4, 0x2(r5)
    sth r4, 0x2(r7)
    lhz r4, 0x4(r5)
    sth r4, 0x4(r7)
    lhz r4, 0x6(r5)
    sth r4, 0x6(r7)
    lhz r4, 0x8(r5)
    sth r4, 0x8(r7)
    lhz r4, 0xa(r5)
    sth r4, 0xa(r7)
    lhz r4, 0xc(r5)
    sth r4, 0xc(r7)
    lhz r4, 0xe(r5)
    addi r5, r5, 0x10
    sth r4, 0xe(r7)
    addi r7, r7, 0x10
    bdnz @L_800DD784
    andi. r6, r6, 0x7
    beq @L_800DD7F0
    @L_800DD7D8:
    mtctr r6
    @L_800DD7DC:
    lhz r4, 0x0(r5)
    addi r5, r5, 0x2
    sth r4, 0x0(r7)
    addi r7, r7, 0x2
    bdnz @L_800DD7DC
    @L_800DD7F0:
    lwz r4, lbl_8047AB08(r13)
    subi r4, r4, 0x1
    stw r4, lbl_8047AB08(r13)
    @L_800DD7FC:
    clrlwi. r0, r0, 24
    beq @L_800DD4C0
    lwz r8, lbl_8047AB08(r13)
    li r5, 0x0
    lwz r6, lbl_8047AB00(r13)
    mr r4, r5
    cmplwi r8, 0x0
    ble @L_800DD8B4
    cmplwi r8, 0x8
    subi r3, r8, 0x8
    ble @L_800DD88C
    addi r0, r3, 0x7
    mr r7, r6
    srwi r0, r0, 3
    mtctr r0
    cmplwi r3, 0x0
    ble @L_800DD88C
    @L_800DD840:
    lhz r3, 0x0(r7)
    addi r4, r4, 0x8
    lhz r0, 0x2(r7)
    add r5, r5, r3
    lhz r3, 0x4(r7)
    add r5, r5, r0
    lhz r0, 0x6(r7)
    add r5, r5, r3
    lhz r3, 0x8(r7)
    add r5, r5, r0
    lhz r0, 0xa(r7)
    add r5, r5, r3
    lhz r3, 0xc(r7)
    add r5, r5, r0
    lhz r0, 0xe(r7)
    add r5, r5, r3
    addi r7, r7, 0x10
    add r5, r5, r0
    bdnz @L_800DD840
    @L_800DD88C:
    slwi r3, r4, 1
    subf r0, r4, r8
    add r3, r6, r3
    mtctr r0
    cmplw r4, r8
    bge @L_800DD8B4
    @L_800DD8A4:
    lhz r0, 0x0(r3)
    addi r3, r3, 0x2
    add r5, r5, r0
    bdnz @L_800DD8A4
    @L_800DD8B4:
    lbz r0, lbl_8047AB11(r13)
    lwz r3, lbl_8047AAFC(r13)
    cmplwi r0, 0x0
    add r30, r3, r5
    beq @L_800DD8F8
    lis r3, lbl_80400F30@ha
    addi r3, r3, lbl_80400F30@l
    bl strlen
    lis r4, lbl_80400F30@ha
    mr r5, r3
    addi r4, r4, lbl_80400F30@l
    mr r3, r30
    bl memcpy
    lis r3, lbl_80400F30@ha
    addi r3, r3, lbl_80400F30@l
    bl strlen
    add r30, r30, r3
    @L_800DD8F8:
    lis r3, lbl_80400F44@ha
    addi r3, r3, lbl_80400F44@l
    bl strlen
    mr r5, r3
    lis r4, lbl_80400F44@ha
    addi r4, r4, lbl_80400F44@l
    mr r3, r30
    addi r5, r5, 0x1
    bl memcpy
    lwz r4, lbl_8047AB08(r13)
    lwz r5, lbl_8047AB00(r13)
    addi r3, r4, 0x1
    slwi r0, r4, 1
    stw r3, lbl_8047AB08(r13)
    sthx r31, r5, r0
    @L_800DD934:
    lbz r0, lbl_8047AB11(r13)
    cmplwi r0, 0x0
    beq @L_800DD94C
    lis r3, lbl_80400F30@ha
    addi r3, r3, lbl_80400F30@l
    bl strlen
    @L_800DD94C:
    lis r3, lbl_80400F44@ha
    addi r3, r3, lbl_80400F44@l
    bl strlen
    lwz r0, 0xb4(r1)
    lwz r31, 0xac(r1)
    lwz r30, 0xa8(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
