    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stmw r20, 0x30(r1)
    mr r23, r3
    lwz r7, 0x34(r23)
    li r3, 0x0
    lwz r6, 0x38(r23)
    li r0, 0x1
    lwz r5, 0x3c(r23)
    li r31, 0x0
    lbz r4, 0x45(r23)
    li r30, 0x0
    lwz r25, 0x30(r23)
    li r28, 0x0
    lbz r24, 0x40(r23)
    extsb r21, r4
    lbz r26, 0x1(r23)
    li r27, 0x0
    stw r7, 0x8(r1)
    li r29, 0x0
    stw r3, 0x54(r23)
    stb r3, 0x58(r23)
    stb r3, 0x59(r23)
    stb r3, 0x1(r23)
    stb r0, 0x45(r23)
    stw r6, 0xc(r1)
    stw r5, 0x10(r1)
    stb r0, 0x4b(r23)
    b @L_800FAE14
@L_800FAB10:
    lwz r3, 0x30(r23)
    lhz r4, 0x0(r3)
    cmplwi r4, 0x0
    bne @L_800FAB54
    lbz r3, 0x40(r23)
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800FAB34
    b @L_800FAB5C
@L_800FAB34:
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x40(r23)
    slwi r3, r0, 2
    addi r0, r3, 0x34
    lwzx r0, r23, r0
    stw r0, 0x30(r23)
    b @L_800FAB10
@L_800FAB54:
    addi r0, r3, 0x2
    stw r0, 0x30(r23)
@L_800FAB5C:
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    beq @L_800FAE10
    cmplwi r0, 0xffff
    bne @L_800FAD24
    lwz r4, 0x30(r23)
    addi r0, r4, 0x1
    stw r0, 0x30(r23)
    lwz r3, lbl_80478B08(r13)
    lbz r4, 0x0(r4)
    lwz r3, 0x28(r3)
    cmplwi r3, 0x0
    beq @L_800FACF0
    lbz r0, 0x1(r23)
    cmplwi r0, 0x0
    bne @L_800FABAC
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 27
    b @L_800FABB8
@L_800FABAC:
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 28
@L_800FABB8:
    cmplwi r0, 0x0
    beq @L_800FACF0
    slwi r0, r4, 3
    add r20, r3, r0
    lwz r12, 0x4(r20)
    cmplwi r12, 0x0
    beq @L_800FACF0
    mr r3, r23
    mtctr r12
    bctrl
    lbz r0, 0x0(r20)
    extrwi r0, r0, 2, 24
    cmplwi r0, 0x0
    beq @L_800FACF0
    cmplwi r3, 0x0
    beq @L_800FACF0
    cmpwi r0, 0x2
    beq @L_800FAC18
    bge @L_800FACA8
    cmpwi r0, 0x1
    bge @L_800FAC10
    b @L_800FACA8
@L_800FAC10:
    mr r22, r3
    b @L_800FACA8
@L_800FAC18:
    cmplwi r3, 0x0
    bne @L_800FAC28
    li r22, 0x0
    b @L_800FACA8
@L_800FAC28:
    lwz r4, lbl_80478B08(r13)
    srwi r5, r3, 20
    clrlwi r3, r3, 12
    lwz r9, 0x8(r4)
    b @L_800FAC9C
@L_800FAC3C:
    lhz r0, 0x0(r9)
    cmplw r0, r5
    bne @L_800FAC98
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FAC90
@L_800FAC58:
    add r0, r7, r6
    srwi r4, r0, 1
    slwi r0, r4, 3
    add r10, r8, r0
    lwz r0, 0x0(r10)
    cmplw r0, r3
    bne @L_800FAC80
    lwz r0, 0x4(r10)
    add r22, r9, r0
    b @L_800FACA8
@L_800FAC80:
    bge @L_800FAC8C
    addi r7, r4, 0x1
    b @L_800FAC90
@L_800FAC8C:
    mr r6, r4
@L_800FAC90:
    cmplw r7, r6
    blt @L_800FAC58
@L_800FAC98:
    lwz r9, 0x8(r9)
@L_800FAC9C:
    cmplwi r9, 0x0
    bne @L_800FAC3C
    li r22, 0x0
@L_800FACA8:
    lbz r5, 0x40(r23)
    extsb r3, r5
    cmpwi r3, 0x3
    blt @L_800FACD4
    lis r3, lbl_80271700@ha
    lis r4, lbl_80315678@ha
    addi r3, r3, lbl_80271700@l
    addi r4, r4, lbl_80315678@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800FACF0
@L_800FACD4:
    lwz r4, 0x30(r23)
    addi r0, r5, 0x1
    slwi r3, r3, 2
    stb r0, 0x40(r23)
    addi r0, r3, 0x34
    stwx r4, r23, r0
    stw r22, 0x30(r23)
@L_800FACF0:
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_800FAD14
    lbz r0, 0x4b(r23)
    cmplwi r0, 0x2
    bne @L_800FAB10
    lwz r0, 0x30(r23)
    stw r0, 0x54(r23)
    b @L_800FAE10
@L_800FAD14:
    lbz r0, 0x4b(r23)
    cmplwi r0, 0x0
    beq @L_800FAE10
    b @L_800FAB10
@L_800FAD24:
    mr r3, r23
    li r5, 0x0
    bl fn_800FDF1C
    cmplwi r3, 0x0
    beq @L_800FAD40
    lbz r3, 0x2(r3)
    b @L_800FAD44
@L_800FAD40:
    lbz r3, 0x22(r23)
@L_800FAD44:
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    bne @L_800FADAC
    extsh r0, r3
    lis r3, 0x4330
    xoris r0, r0, 0x8000
    extsh r4, r28
    stw r0, 0x24(r1)
    xoris r0, r4, 0x8000
    lfd f4, lbl_8047CD10(r2)
    addi r31, r31, 0x1
    stw r3, 0x20(r1)
    lfs f1, 0x60(r23)
    lfd f0, 0x20(r1)
    stw r0, 0x1c(r1)
    fsubs f2, f0, f4
    lfs f0, lbl_8047CD30(r2)
    stw r3, 0x18(r1)
    lfd f3, 0x18(r1)
    fmadds f0, f2, f1, f0
    fsubs f1, f3, f4
    fadds f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x28(r1)
    lwz r28, 0x2c(r1)
    b @L_800FAB10
@L_800FADAC:
    extsh r0, r3
    lis r3, 0x4330
    xoris r0, r0, 0x8000
    extsh r4, r27
    stw r0, 0x24(r1)
    xoris r0, r4, 0x8000
    lfd f4, lbl_8047CD10(r2)
    addi r30, r30, 0x1
    stw r3, 0x20(r1)
    lfs f1, 0x60(r23)
    lfd f0, 0x20(r1)
    stw r0, 0x2c(r1)
    fsubs f2, f0, f4
    lfs f3, lbl_8047CD34(r2)
    stw r3, 0x28(r1)
    lfs f0, lbl_8047CD30(r2)
    fmuls f1, f2, f1
    lfd f2, 0x28(r1)
    fsubs f2, f2, f4
    fmadds f0, f3, f1, f0
    fadds f0, f2, f0
    fctiwz f0, f0
    stfd f0, 0x18(r1)
    lwz r27, 0x1c(r1)
    b @L_800FAB10
@L_800FAE10:
    addi r29, r29, 0x1
@L_800FAE14:
    clrlwi r0, r29, 24
    cmplwi r0, 0x2
    blt @L_800FAB10
    extsh r3, r27
    extsh r0, r28
    subf r3, r3, r0
    lis r4, 0x4330
    srwi r0, r3, 31
    lbz r5, 0x22(r23)
    add r0, r0, r3
    lfs f0, 0x60(r23)
    srawi r0, r0, 1
    stw r5, 0x2c(r1)
    xoris r3, r0, 0x8000
    lfd f5, lbl_8047CD28(r2)
    stw r4, 0x28(r1)
    clrlwi r0, r21, 24
    lfd f1, lbl_8047CD10(r2)
    stb r31, 0x5a(r23)
    lfd f2, 0x28(r1)
    stb r30, 0x5b(r23)
    fsubs f2, f2, f5
    lfs f6, lbl_8047CD34(r2)
    stw r3, 0x24(r1)
    lfs f3, 0xc(r23)
    stw r4, 0x20(r1)
    fmuls f4, f2, f0
    lfs f2, lbl_8047CD38(r2)
    lfd f0, 0x20(r1)
    fmuls f4, f6, f4
    stw r4, 0x18(r1)
    fsubs f0, f0, f1
    lwz r5, 0x8(r1)
    lwz r4, 0xc(r1)
    lwz r3, 0x10(r1)
    fadds f0, f3, f0
    stfs f0, 0x4c(r23)
    stfs f4, 0x5c(r23)
    lbz r6, 0x23(r23)
    lfs f0, 0x10(r23)
    stw r6, 0x1c(r1)
    lfd f1, 0x18(r1)
    fsubs f1, f1, f5
    fnmsubs f0, f2, f1, f0
    stfs f0, 0x50(r23)
    stw r25, 0x30(r23)
    stb r24, 0x40(r23)
    stw r5, 0x34(r23)
    stw r4, 0x38(r23)
    stw r3, 0x3c(r23)
    stb r0, 0x45(r23)
    stb r26, 0x1(r23)
    lmw r20, 0x30(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
