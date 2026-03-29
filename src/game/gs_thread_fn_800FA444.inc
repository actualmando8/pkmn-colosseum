    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r25, 0x34(r1)
    mr r31, r3
    cmplwi r31, 0x0
    li r28, 0x0
    li r27, 0x0
    li r26, 0x0
    bne @L_800FA474
    li r3, 0x0
    b @L_800FAA84
@L_800FA474:
    bne @L_800FA480
    li r25, 0x0
    b @L_800FA510
@L_800FA480:
    lwz r3, lbl_80478B08(r13)
    srwi r4, r31, 20
    clrlwi r5, r31, 12
    lwz r9, 0x8(r3)
    b @L_800FA504
@L_800FA494:
    lhz r0, 0x0(r9)
    cmplw r0, r4
    bne @L_800FA500
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FA4F8
@L_800FA4B0:
    add r0, r7, r6
    srwi r3, r0, 1
    slwi r10, r3, 3
    lwzx r0, r8, r10
    cmplw r0, r5
    bne @L_800FA4E8
    addi r0, r1, 0x8
    cmplwi r0, 0x0
    beq @L_800FA4D8
    stw r9, 0x8(r1)
@L_800FA4D8:
    add r3, r8, r10
    lwz r0, 0x4(r3)
    add r25, r9, r0
    b @L_800FA510
@L_800FA4E8:
    bge @L_800FA4F4
    addi r7, r3, 0x1
    b @L_800FA4F8
@L_800FA4F4:
    mr r6, r3
@L_800FA4F8:
    cmplw r7, r6
    blt @L_800FA4B0
@L_800FA500:
    lwz r9, 0x8(r9)
@L_800FA504:
    cmplwi r9, 0x0
    bne @L_800FA494
    li r25, 0x0
@L_800FA510:
    cmplwi r25, 0x0
    bne @L_800FA520
    li r3, 0x0
    b @L_800FAA84
@L_800FA520:
    lis r3, lbl_80401E48@ha
    li r4, 0x0
    addi r3, r3, lbl_80401E48@l
    li r5, 0x68
    mr r29, r3
    bl memset
    lis r3, lbl_80401E48@ha
    lfs f0, lbl_8047CD08(r2)
    addi r4, r3, lbl_80401E48@l
    li r5, 0x1
    li r0, -0x1
    stb r5, 0x0(r4)
    lwz r3, 0x8(r1)
    li r6, 0x0
    stfs f0, 0x60(r4)
    lwz r7, lbl_80478B08(r13)
    stfs f0, 0x64(r4)
    stw r0, 0x24(r4)
    stw r25, 0x28(r4)
    stw r25, 0x2c(r4)
    stw r25, 0x30(r4)
    lbz r0, 0x3(r3)
    sth r0, 0x20(r4)
    stw r31, 0x1c(r4)
    stb r5, 0x1(r4)
    lhz r3, 0x4(r7)
    clrlwi r4, r0, 16
    mtctr r3
    cmpwi r3, 0x0
    ble @L_800FA62C
@L_800FA598:
    lwz r0, 0x24(r7)
    add r3, r0, r6
    lhz r0, 0x0(r3)
    cmplw r0, r4
    bne @L_800FA624
    lbz r0, 0x2(r3)
    cmplwi r4, 0x0
    stb r0, 0x22(r29)
    lbz r3, 0x3(r3)
    stb r3, 0x23(r29)
    bne @L_800FA5D0
    li r0, 0xb
    stb r0, 0x42(r29)
    b @L_800FA62C
@L_800FA5D0:
    cmplwi r4, 0x1
    beq @L_800FA5DC
    bne @L_800FA5E8
@L_800FA5DC:
    li r0, 0x6
    stb r0, 0x42(r29)
    b @L_800FA62C
@L_800FA5E8:
    lis r0, 0x4330
    stw r3, 0x14(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r0, 0x10(r1)
    lfd f3, lbl_8047CD20(r2)
    lfd f1, 0x10(r1)
    lfd f0, lbl_8047CD18(r2)
    fsub f1, f1, f2
    fmadd f0, f3, f1, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r0, 0x1c(r1)
    extsb r0, r0
    stb r0, 0x42(r29)
    b @L_800FA62C
@L_800FA624:
    addi r6, r6, 0x8
    bdnz @L_800FA598
@L_800FA62C:
    lwz r3, 0x30(r29)
    lhz r31, 0x0(r3)
    cmplwi r31, 0x0
    bne @L_800FA670
    lbz r3, 0x40(r29)
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800FA650
    b @L_800FA678
@L_800FA650:
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x40(r29)
    slwi r3, r0, 2
    addi r0, r3, 0x34
    lwzx r0, r29, r0
    stw r0, 0x30(r29)
    b @L_800FA62C
@L_800FA670:
    addi r0, r3, 0x2
    stw r0, 0x30(r29)
@L_800FA678:
    clrlwi r3, r31, 16
    cmplwi r3, 0x0
    beq @L_800FAA1C
    cmplwi r3, 0xffff
    bne @L_800FA8B4
    lwz r3, 0x30(r29)
    addi r0, r3, 0x1
    stw r0, 0x30(r29)
    lbz r5, 0x0(r3)
    cmplwi r5, 0x3
    bne @L_800FA6FC
    lbz r4, 0x22(r29)
    lis r3, 0x4330
    extsh r0, r28
    stw r3, 0x18(r1)
    xoris r0, r0, 0x8000
    lfd f1, lbl_8047CD28(r2)
    stw r4, 0x1c(r1)
    lfs f3, 0xc(r29)
    lfd f0, 0x18(r1)
    stw r0, 0x14(r1)
    fsubs f2, f0, f1
    lfd f1, lbl_8047CD10(r2)
    stw r3, 0x10(r1)
    lfd f0, 0x10(r1)
    fadds f2, f3, f2
    fsubs f0, f0, f1
    stfs f2, 0xc(r29)
    fcmpo cr0, f0, f2
    bge @L_800FA6FC
    fctiwz f0, f2
    stfd f0, 0x18(r1)
    lwz r28, 0x1c(r1)
@L_800FA6FC:
    lwz r3, lbl_80478B08(r13)
    lwz r3, 0x28(r3)
    cmplwi r3, 0x0
    beq @L_800FA86C
    lbz r0, 0x1(r29)
    cmplwi r0, 0x0
    bne @L_800FA728
    slwi r0, r5, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 27
    b @L_800FA734
@L_800FA728:
    slwi r0, r5, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 28
@L_800FA734:
    cmplwi r0, 0x0
    beq @L_800FA86C
    slwi r0, r5, 3
    add r25, r3, r0
    lwz r12, 0x4(r25)
    cmplwi r12, 0x0
    beq @L_800FA86C
    mr r3, r29
    mtctr r12
    bctrl
    lbz r0, 0x0(r25)
    extrwi r0, r0, 2, 24
    cmplwi r0, 0x0
    beq @L_800FA86C
    cmplwi r3, 0x0
    beq @L_800FA86C
    cmpwi r0, 0x2
    beq @L_800FA794
    bge @L_800FA824
    cmpwi r0, 0x1
    bge @L_800FA78C
    b @L_800FA824
@L_800FA78C:
    mr r30, r3
    b @L_800FA824
@L_800FA794:
    cmplwi r3, 0x0
    bne @L_800FA7A4
    li r30, 0x0
    b @L_800FA824
@L_800FA7A4:
    lwz r4, lbl_80478B08(r13)
    srwi r5, r3, 20
    clrlwi r3, r3, 12
    lwz r9, 0x8(r4)
    b @L_800FA818
@L_800FA7B8:
    lhz r0, 0x0(r9)
    cmplw r0, r5
    bne @L_800FA814
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FA80C
@L_800FA7D4:
    add r0, r7, r6
    srwi r4, r0, 1
    slwi r0, r4, 3
    add r10, r8, r0
    lwz r0, 0x0(r10)
    cmplw r0, r3
    bne @L_800FA7FC
    lwz r0, 0x4(r10)
    add r30, r9, r0
    b @L_800FA824
@L_800FA7FC:
    bge @L_800FA808
    addi r7, r4, 0x1
    b @L_800FA80C
@L_800FA808:
    mr r6, r4
@L_800FA80C:
    cmplw r7, r6
    blt @L_800FA7D4
@L_800FA814:
    lwz r9, 0x8(r9)
@L_800FA818:
    cmplwi r9, 0x0
    bne @L_800FA7B8
    li r30, 0x0
@L_800FA824:
    lbz r4, 0x40(r29)
    extsb r3, r4
    cmpwi r3, 0x3
    blt @L_800FA850
    lis r3, lbl_80271700@ha
    lis r4, lbl_80315678@ha
    addi r3, r3, lbl_80271700@l
    addi r4, r4, lbl_80315678@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800FA86C
@L_800FA850:
    addi r0, r4, 0x1
    slwi r3, r3, 2
    stb r0, 0x40(r29)
    addi r0, r3, 0x34
    lwz r3, 0x30(r29)
    stwx r3, r29, r0
    stw r30, 0x30(r29)
@L_800FA86C:
    clrlwi r0, r26, 24
    cmplwi r0, 0x0
    beq @L_800FA9A8
    lfs f2, 0xc(r29)
    lfs f0, 0x4(r29)
    fcmpu cr0, f2, f0
    bne @L_800FA9A8
    lbz r3, 0x22(r29)
    lis r0, 0x4330
    stw r0, 0x18(r1)
    xoris r0, r3, 0x8000
    lfd f1, lbl_8047CD10(r2)
    stw r0, 0x1c(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f1
    fadds f0, f2, f0
    stfs f0, 0xc(r29)
    b @L_800FA9A8
@L_800FA8B4:
    lbz r0, 0x4b(r29)
    cmplwi r0, 0x2
    beq @L_800FA62C
    cmplwi r3, 0x20
    bne @L_800FA8FC
    lbz r3, 0x22(r29)
    lis r0, 0x4330
    stw r0, 0x18(r1)
    srwi r0, r3, 1
    lfd f2, lbl_8047CD10(r2)
    xoris r0, r0, 0x8000
    lfs f0, 0x60(r29)
    stw r0, 0x1c(r1)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    stfs f0, 0x14(r29)
    b @L_800FA968
@L_800FA8FC:
    mr r3, r29
    mr r4, r31
    li r5, 0x0
    bl fn_800FDF1C
    cmplwi r3, 0x0
    bne @L_800FA940
    lbz r3, 0x22(r29)
    lis r0, 0x4330
    stw r0, 0x18(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r3, 0x1c(r1)
    lfs f0, 0x60(r29)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    stfs f0, 0x14(r29)
    b @L_800FA968
@L_800FA940:
    lbz r3, 0x2(r3)
    lis r0, 0x4330
    stw r0, 0x18(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r3, 0x1c(r1)
    lfs f0, 0x60(r29)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    stfs f0, 0x14(r29)
@L_800FA968:
    lbz r0, 0x41(r29)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_800FA998
    clrlwi r0, r31, 16
    cmplwi r0, 0x300c
    bne @L_800FA988
    li r26, 0x1
@L_800FA988:
    clrlwi r0, r31, 16
    cmplwi r0, 0x300d
    bne @L_800FA998
    li r26, 0x0
@L_800FA998:
    lfs f1, 0xc(r29)
    lfs f0, 0x14(r29)
    fadds f0, f1, f0
    stfs f0, 0xc(r29)
@L_800FA9A8:
    extsh r3, r28
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x18(r1)
    lfd f1, lbl_8047CD10(r2)
    stw r3, 0x1c(r1)
    lfs f2, 0xc(r29)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f0, f2
    bge @L_800FA9E0
    fctiwz f0, f2
    stfd f0, 0x10(r1)
    lwz r28, 0x14(r1)
@L_800FA9E0:
    extsh r3, r27
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x20(r1)
    lfd f1, lbl_8047CD10(r2)
    stw r3, 0x24(r1)
    lfs f2, 0x10(r29)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fcmpo cr0, f0, f2
    bge @L_800FA62C
    fctiwz f0, f2
    stfd f0, 0x20(r1)
    lwz r27, 0x24(r1)
    b @L_800FA62C
@L_800FAA1C:
    lbz r0, 0x23(r29)
    lis r4, 0x4330
    extsh r5, r27
    extsh r3, r28
    stw r0, 0x1c(r1)
    xoris r5, r5, 0x8000
    subi r0, r3, 0x1
    lfd f1, lbl_8047CD28(r2)
    stw r4, 0x18(r1)
    slwi r3, r0, 16
    lfd f4, lbl_8047CD10(r2)
    lfd f0, 0x18(r1)
    stw r5, 0x24(r1)
    fsubs f2, f0, f1
    lfs f1, 0x64(r29)
    stw r4, 0x20(r1)
    lfs f0, lbl_8047CD08(r2)
    lfd f3, 0x20(r1)
    fmadds f0, f2, f1, f0
    fsubs f1, f3, f4
    fadds f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r27, 0x14(r1)
    extsh r0, r27
    or r3, r3, r0
@L_800FAA84:
    lmw r25, 0x34(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
