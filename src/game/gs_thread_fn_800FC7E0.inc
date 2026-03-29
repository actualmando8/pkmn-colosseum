    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stmw r19, 0x6c(r1)
    mr r31, r3
    mr r30, r4
    mr r23, r5
    mr r22, r6
    lwz r0, lbl_8047CD00(r2)
    cmplwi r31, 0x0
    li r27, 0x0
    li r29, 0x0
    stw r0, 0x10(r1)
    bne @L_800FC820
    li r3, -0x1
    b @L_800FD334
@L_800FC820:
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    bne @L_800FC834
    li r3, -0x1
    b @L_800FD334
@L_800FC834:
    bl fn_800FE4D4
    lis r3, 0x8000
    addi r3, r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    lwz r4, lbl_80478B08(r13)
    li r3, 0x0
    lbz r0, 0x1d(r4)
    extsb r0, r0
    slwi r0, r0, 2
    add r4, r4, r0
    lwz r4, 0xc(r4)
    bl fn_800D85D4
    li r3, 0x1
    bl fn_800DC1D4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_800DC224
    lwz r0, 0x10(r1)
    addi r4, r1, 0xc
    li r3, 0x0
    stw r0, 0xc(r1)
    bl fn_800DBEB4
    li r3, 0x0
    li r4, 0xc
    bl fn_800DBF78
    li r3, 0x0
    li r4, 0xf
    li r5, 0xe
    li r6, 0xa
    li r7, 0xf
    bl fn_800DC0D4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC14C
    li r3, 0x0
    li r4, 0x7
    li r5, 0x4
    li r6, 0x5
    li r7, 0x7
    bl fn_800DBFD4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800DC04C
    lwz r3, lbl_80478B08(r13)
    li r4, 0x0
    lbz r0, 0x1d(r3)
    extsb r0, r0
    slwi r0, r0, 2
    add r3, r3, r0
    lwz r3, 0xc(r3)
    bl fn_800EF548
    rlwinm r0, r23, 0, 26, 27
    lwz r4, lbl_80478B08(r13)
    cmplwi r0, 0x0
    stw r3, 0x14(r4)
    beq @L_800FC960
    li r0, 0x1
    stb r0, 0x45(r31)
    b @L_800FC968
@L_800FC960:
    li r0, 0x0
    stb r0, 0x45(r31)
@L_800FC968:
    li r3, 0x0
    clrlwi r0, r22, 24
    stb r3, 0x1(r31)
    cmplwi r0, 0x0
    li r22, 0x0
    stb r3, 0x4b(r31)
    stb r3, 0x46(r31)
    bne @L_800FCBDC
    li r23, 0x0
    b @L_800FCBD0
@L_800FC990:
    lwz r3, 0x30(r31)
    lhz r4, 0x0(r3)
    cmplwi r4, 0x0
    bne @L_800FC9D4
    lbz r3, 0x40(r31)
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800FC9B4
    b @L_800FC9DC
@L_800FC9B4:
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x40(r31)
    slwi r3, r0, 2
    addi r0, r3, 0x34
    lwzx r0, r31, r0
    stw r0, 0x30(r31)
    b @L_800FC990
@L_800FC9D4:
    addi r0, r3, 0x2
    stw r0, 0x30(r31)
@L_800FC9DC:
    clrlwi r0, r4, 16
    cmplwi r0, 0x0
    bne @L_800FC9F4
    li r0, 0x2
    stb r0, 0x0(r31)
    b @L_800FCBCC
@L_800FC9F4:
    cmplwi r0, 0xffff
    bne @L_800FCB9C
    lwz r4, 0x30(r31)
    addi r0, r4, 0x1
    stw r0, 0x30(r31)
    lwz r3, lbl_80478B08(r13)
    lbz r4, 0x0(r4)
    lwz r3, 0x28(r3)
    cmplwi r3, 0x0
    bne @L_800FCA24
    li r0, 0x0
    b @L_800FCB94
@L_800FCA24:
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    bne @L_800FCA40
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 27
    b @L_800FCA4C
@L_800FCA40:
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 28
@L_800FCA4C:
    cmplwi r0, 0x0
    bne @L_800FCA5C
    li r0, 0x0
    b @L_800FCB94
@L_800FCA5C:
    slwi r0, r4, 3
    add r22, r3, r0
    lwz r12, 0x4(r22)
    cmplwi r12, 0x0
    beq @L_800FCB8C
    mr r3, r31
    mtctr r12
    bctrl
    lbz r0, 0x0(r22)
    extrwi r0, r0, 2, 24
    cmplwi r0, 0x0
    beq @L_800FCB8C
    cmplwi r3, 0x0
    beq @L_800FCB8C
    cmpwi r0, 0x2
    beq @L_800FCAB4
    bge @L_800FCB44
    cmpwi r0, 0x1
    bge @L_800FCAAC
    b @L_800FCB44
@L_800FCAAC:
    mr r21, r3
    b @L_800FCB44
@L_800FCAB4:
    cmplwi r3, 0x0
    bne @L_800FCAC4
    li r21, 0x0
    b @L_800FCB44
@L_800FCAC4:
    lwz r4, lbl_80478B08(r13)
    srwi r5, r3, 20
    clrlwi r3, r3, 12
    lwz r9, 0x8(r4)
    b @L_800FCB38
@L_800FCAD8:
    lhz r0, 0x0(r9)
    cmplw r0, r5
    bne @L_800FCB34
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FCB2C
@L_800FCAF4:
    add r0, r7, r6
    srwi r4, r0, 1
    slwi r0, r4, 3
    add r10, r8, r0
    lwz r0, 0x0(r10)
    cmplw r0, r3
    bne @L_800FCB1C
    lwz r0, 0x4(r10)
    add r21, r9, r0
    b @L_800FCB44
@L_800FCB1C:
    bge @L_800FCB28
    addi r7, r4, 0x1
    b @L_800FCB2C
@L_800FCB28:
    mr r6, r4
@L_800FCB2C:
    cmplw r7, r6
    blt @L_800FCAF4
@L_800FCB34:
    lwz r9, 0x8(r9)
@L_800FCB38:
    cmplwi r9, 0x0
    bne @L_800FCAD8
    li r21, 0x0
@L_800FCB44:
    lbz r5, 0x40(r31)
    extsb r3, r5
    cmpwi r3, 0x3
    blt @L_800FCB70
    lis r3, lbl_80271700@ha
    lis r4, lbl_80315678@ha
    addi r3, r3, lbl_80271700@l
    addi r4, r4, lbl_80315678@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800FCB8C
@L_800FCB70:
    lwz r4, 0x30(r31)
    addi r0, r5, 0x1
    slwi r3, r3, 2
    stb r0, 0x40(r31)
    addi r0, r3, 0x34
    stwx r4, r31, r0
    stw r21, 0x30(r31)
@L_800FCB8C:
    lbz r0, 0x0(r22)
    extrwi r0, r0, 1, 26
@L_800FCB94:
    mr r22, r0
    b @L_800FCBC0
@L_800FCB9C:
    lbz r0, 0x4b(r31)
    cmplwi r0, 0x2
    beq @L_800FCBC0
    clrlwi r0, r30, 24
    li r22, 0x1
    cmplwi r0, 0x0
    beq @L_800FCBBC
    li r22, 0x0
@L_800FCBBC:
    li r29, 0x1
@L_800FCBC0:
    clrlwi r0, r22, 24
    cmplwi r0, 0x0
    beq @L_800FC990
@L_800FCBCC:
    addi r23, r23, 0x1
@L_800FCBD0:
    bl fn_800D3088
    cmplw r23, r3
    blt @L_800FC990
@L_800FCBDC:
    lwz r6, 0x34(r31)
    li r3, 0x1
    lwz r5, 0x38(r31)
    li r0, 0x0
    lwz r4, 0x3c(r31)
    addi r25, r1, 0x14
    lwz r26, 0x30(r31)
    lbz r24, 0x40(r31)
    stw r6, 0x14(r1)
    stb r3, 0x1(r31)
    lfs f0, 0x4(r31)
    stw r5, 0x18(r1)
    stfs f0, 0xc(r31)
    lfs f0, 0x8(r31)
    stw r4, 0x1c(r1)
    stfs f0, 0x10(r31)
    lwz r3, 0x2c(r31)
    stw r3, 0x30(r31)
    stb r0, 0x40(r31)
    stb r0, 0x4b(r31)
@L_800FCC2C:
    lwz r0, 0x30(r31)
    cmplw r0, r26
    bne @L_800FCC80
    lbz r6, 0x40(r31)
    mr r4, r25
    mr r5, r31
    li r7, 0x0
    extsb r6, r6
    mtctr r6
    cmpwi r6, 0x0
    ble @L_800FCC78
@L_800FCC58:
    lwz r3, 0x34(r5)
    lwz r0, 0x0(r4)
    cmplw r3, r0
    bne @L_800FCC78
    addi r4, r4, 0x4
    addi r5, r5, 0x4
    addi r7, r7, 0x1
    bdnz @L_800FCC58
@L_800FCC78:
    cmpw r7, r6
    beq @L_800FD13C
@L_800FCC80:
    lwz r3, 0x30(r31)
    lhz r23, 0x0(r3)
    cmplwi r23, 0x0
    bne @L_800FCCC4
    lbz r3, 0x40(r31)
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_800FCCA4
    b @L_800FCCCC
@L_800FCCA4:
    subi r3, r3, 0x1
    extsb r0, r3
    stb r3, 0x40(r31)
    slwi r3, r0, 2
    addi r0, r3, 0x34
    lwzx r0, r31, r0
    stw r0, 0x30(r31)
    b @L_800FCC80
@L_800FCCC4:
    addi r0, r3, 0x2
    stw r0, 0x30(r31)
@L_800FCCCC:
    clrlwi r3, r23, 16
    cmplwi r3, 0x0
    beq @L_800FD13C
    cmplwi r3, 0xffff
    bne @L_800FCEA8
    lwz r4, 0x30(r31)
    addi r0, r4, 0x1
    stw r0, 0x30(r31)
    lwz r3, lbl_80478B08(r13)
    lbz r4, 0x0(r4)
    lwz r3, 0x28(r3)
    cmplwi r3, 0x0
    beq @L_800FCE60
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    bne @L_800FCD1C
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 27
    b @L_800FCD28
@L_800FCD1C:
    slwi r0, r4, 3
    lbzx r0, r3, r0
    extrwi r0, r0, 1, 28
@L_800FCD28:
    cmplwi r0, 0x0
    beq @L_800FCE60
    slwi r0, r4, 3
    add r19, r3, r0
    lwz r12, 0x4(r19)
    cmplwi r12, 0x0
    beq @L_800FCE60
    mr r3, r31
    mtctr r12
    bctrl
    lbz r0, 0x0(r19)
    extrwi r0, r0, 2, 24
    cmplwi r0, 0x0
    beq @L_800FCE60
    cmplwi r3, 0x0
    beq @L_800FCE60
    cmpwi r0, 0x2
    beq @L_800FCD88
    bge @L_800FCE18
    cmpwi r0, 0x1
    bge @L_800FCD80
    b @L_800FCE18
@L_800FCD80:
    mr r28, r3
    b @L_800FCE18
@L_800FCD88:
    cmplwi r3, 0x0
    bne @L_800FCD98
    li r28, 0x0
    b @L_800FCE18
@L_800FCD98:
    lwz r4, lbl_80478B08(r13)
    srwi r5, r3, 20
    clrlwi r3, r3, 12
    lwz r9, 0x8(r4)
    b @L_800FCE0C
@L_800FCDAC:
    lhz r0, 0x0(r9)
    cmplw r0, r5
    bne @L_800FCE08
    lhz r6, 0x4(r9)
    addi r8, r9, 0x10
    li r7, 0x0
    b @L_800FCE00
@L_800FCDC8:
    add r0, r7, r6
    srwi r4, r0, 1
    slwi r0, r4, 3
    add r10, r8, r0
    lwz r0, 0x0(r10)
    cmplw r0, r3
    bne @L_800FCDF0
    lwz r0, 0x4(r10)
    add r28, r9, r0
    b @L_800FCE18
@L_800FCDF0:
    bge @L_800FCDFC
    addi r7, r4, 0x1
    b @L_800FCE00
@L_800FCDFC:
    mr r6, r4
@L_800FCE00:
    cmplw r7, r6
    blt @L_800FCDC8
@L_800FCE08:
    lwz r9, 0x8(r9)
@L_800FCE0C:
    cmplwi r9, 0x0
    bne @L_800FCDAC
    li r28, 0x0
@L_800FCE18:
    lbz r5, 0x40(r31)
    extsb r3, r5
    cmpwi r3, 0x3
    blt @L_800FCE44
    lis r3, lbl_80271700@ha
    lis r4, lbl_80315678@ha
    addi r3, r3, lbl_80271700@l
    addi r4, r4, lbl_80315678@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_800FCE60
@L_800FCE44:
    lwz r4, 0x30(r31)
    addi r0, r5, 0x1
    slwi r3, r3, 2
    stb r0, 0x40(r31)
    addi r0, r3, 0x34
    stwx r4, r31, r0
    stw r28, 0x30(r31)
@L_800FCE60:
    clrlwi r0, r27, 24
    cmplwi r0, 0x0
    beq @L_800FCC2C
    lfs f2, 0xc(r31)
    lfs f0, 0x4(r31)
    fcmpu cr0, f2, f0
    bne @L_800FCC2C
    lbz r3, 0x22(r31)
    lis r0, 0x4330
    stw r0, 0x20(r1)
    xoris r0, r3, 0x8000
    lfd f1, lbl_8047CD10(r2)
    stw r0, 0x24(r1)
    lfd f0, 0x20(r1)
    fsubs f0, f0, f1
    fadds f0, f2, f0
    stfs f0, 0xc(r31)
    b @L_800FCC2C
@L_800FCEA8:
    lbz r0, 0x4b(r31)
    cmplwi r0, 0x2
    beq @L_800FCC2C
    cmplwi r3, 0x20
    bne @L_800FCEF0
    lbz r3, 0x22(r31)
    lis r0, 0x4330
    stw r0, 0x20(r1)
    srwi r0, r3, 1
    lfd f2, lbl_8047CD10(r2)
    xoris r0, r0, 0x8000
    lfs f0, 0x60(r31)
    stw r0, 0x24(r1)
    lfd f1, 0x20(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    stfs f0, 0x14(r31)
    b @L_800FD0E4
@L_800FCEF0:
    mr r3, r31
    mr r4, r23
    addi r5, r1, 0x8
    bl fn_800FDF1C
    mr r6, r3
    cmplwi r6, 0x0
    bne @L_800FD084
    lfs f0, 0x10(r31)
    lis r6, 0x4330
    lfs f1, 0xc(r31)
    lis r3, 0x8000
    fctiwz f0, f0
    lbz r0, 0x22(r31)
    fctiwz f1, f1
    lbz r4, 0x23(r31)
    stw r0, 0x34(r1)
    addi r3, r3, 0x2
    stfd f0, 0x28(r1)
    lfd f6, lbl_8047CD28(r2)
    stfd f1, 0x20(r1)
    lwz r5, 0x2c(r1)
    lwz r21, 0x24(r1)
    addi r0, r5, 0x2
    stw r6, 0x30(r1)
    extsh r22, r0
    extsh r0, r21
    xoris r5, r0, 0x8000
    stw r6, 0x38(r1)
    xoris r0, r22, 0x8000
    lfd f0, 0x30(r1)
    stw r5, 0x3c(r1)
    lfd f4, lbl_8047CD10(r2)
    fsubs f5, f0, f6
    lfd f0, 0x38(r1)
    stw r4, 0x4c(r1)
    fsubs f2, f0, f4
    lfs f3, 0x60(r31)
    stw r6, 0x48(r1)
    lfs f1, 0x64(r31)
    lfd f0, 0x48(r1)
    fmadds f3, f5, f3, f2
    stw r0, 0x54(r1)
    fsubs f2, f0, f6
    stw r6, 0x50(r1)
    fctiwz f3, f3
    lfd f0, 0x50(r1)
    stfd f3, 0x40(r1)
    fsubs f0, f0, f4
    lwz r19, 0x44(r1)
    fmadds f0, f2, f1, f0
    fctiwz f0, f0
    stfd f0, 0x58(r1)
    lwz r20, 0x5c(r1)
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    li r3, 0x2
    bl fn_800D67BC
    mr r3, r21
    mr r4, r22
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    mr r3, r19
    mr r4, r20
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    bl fn_800D6728
    lis r3, 0x8000
    addi r3, r3, 0x2
    bl fn_800D88DC
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    li r3, 0x1
    bl fn_800DC1D4
    lbz r3, 0x22(r31)
    lis r0, 0x4330
    stw r0, 0x60(r1)
    lfd f2, lbl_8047CD28(r2)
    stw r3, 0x64(r1)
    lfs f0, 0x60(r31)
    lfd f1, 0x60(r1)
    lfs f3, lbl_8047CD30(r2)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    fadds f0, f3, f0
    stfs f0, 0x14(r31)
    b @L_800FD0E4
@L_800FD084:
    lwz r8, 0x8(r1)
    mr r3, r31
    lwz r0, 0x4(r6)
    lwz r5, 0x4(r8)
    clrlwi r4, r0, 8
    lbz r21, 0x2(r6)
    add r4, r5, r4
    srwi r0, r0, 24
    lbz r6, 0x3(r6)
    extsb r7, r0
    mr r5, r21
    add r4, r8, r4
    bl fn_800FD69C
    extsh r3, r21
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x60(r1)
    lfd f2, lbl_8047CD10(r2)
    stw r3, 0x64(r1)
    lfs f0, 0x60(r31)
    lfd f1, 0x60(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    stfs f0, 0x14(r31)
@L_800FD0E4:
    lfs f1, 0xc(r31)
    lfs f0, 0x14(r31)
    fadds f0, f1, f0
    stfs f0, 0xc(r31)
    lbz r0, 0x41(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_800FD124
    clrlwi r0, r23, 16
    cmplwi r0, 0x300c
    bne @L_800FD114
    li r27, 0x1
@L_800FD114:
    clrlwi r0, r23, 16
    cmplwi r0, 0x300d
    bne @L_800FD124
    li r27, 0x0
@L_800FD124:
    lbz r0, 0x4b(r31)
    cmplwi r0, 0x1
    bne @L_800FCC2C
    mr r3, r31
    bl fn_800FD348
    b @L_800FCC2C
@L_800FD13C:
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_800FD14C
    li r29, 0x0
@L_800FD14C:
    lbz r0, 0x47(r31)
    clrlwi r3, r29, 24
    cmplw r3, r0
    beq @L_800FD1E4
    lbz r0, 0x3(r31)
    li r3, 0x0
    cmpwi r0, 0x3
    beq @L_800FD1AC
    bge @L_800FD188
    cmpwi r0, 0x1
    beq @L_800FD19C
    bge @L_800FD1A4
    cmpwi r0, 0x0
    bge @L_800FD1C0
    b @L_800FD1C0
@L_800FD188:
    cmpwi r0, 0x5
    beq @L_800FD1BC
    bge @L_800FD1C0
    b @L_800FD1B4
    b @L_800FD1C0
@L_800FD19C:
    li r3, 0x57
    b @L_800FD1C0
@L_800FD1A4:
    li r3, 0x58
    b @L_800FD1C0
@L_800FD1AC:
    li r3, 0x59
    b @L_800FD1C0
@L_800FD1B4:
    li r3, 0x497
    b @L_800FD1C0
@L_800FD1BC:
    li r3, 0x498
@L_800FD1C0:
    cmplwi r3, 0x0
    beq @L_800FD1E0
    clrlwi r0, r29, 24
    cmplwi r0, 0x0
    beq @L_800FD1DC
    bl fn_80166A28
    b @L_800FD1E0
@L_800FD1DC:
    bl fn_801669BC
@L_800FD1E0:
    stb r29, 0x47(r31)
@L_800FD1E4:
    lbz r0, 0x46(r31)
    cmplwi r0, 0x0
    beq @L_800FD2EC
    lwz r3, lbl_8047AC68(r13)
    lis r0, 0x4330
    stw r0, 0x50(r1)
    xoris r0, r3, 0x8000
    lfd f2, lbl_8047CD10(r2)
    stw r0, 0x54(r1)
    lfs f3, lbl_8047CD40(r2)
    lfd f1, 0x50(r1)
    lfs f0, lbl_8047CD44(r2)
    fsubs f1, f1, f2
    lfs f6, lbl_8047CD30(r2)
    lfs f5, 0xc(r31)
    lfs f4, lbl_8047CD3C(r2)
    fmuls f1, f3, f1
    lfs f2, 0x10(r31)
    fadds f3, f6, f5
    fadds f2, f4, f2
    fdivs f1, f1, f0
    fctiwz f3, f3
    fctiwz f0, f2
    stfd f3, 0x60(r1)
    stfd f0, 0x58(r1)
    lwz r22, 0x64(r1)
    lwz r21, 0x5c(r1)
    bl fn_800CDBE0
    frsp f1, f1
    lfs f0, lbl_8047CD48(r2)
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x48(r1)
    lwz r0, 0x4c(r1)
    add r21, r21, r0
    bl fn_800D3088
    lwz r5, lbl_8047AC68(r13)
    lis r4, 0x51ec
    subi r0, r4, 0x7ae1
    add r5, r5, r3
    lis r3, 0x8000
    mulhw r0, r0, r5
    stw r5, lbl_8047AC68(r13)
    srawi r0, r0, 4
    srwi r4, r0, 31
    add r0, r0, r4
    mulli r0, r0, 0x32
    subf r0, r0, r5
    stw r0, lbl_8047AC68(r13)
    bl fn_800D888C
    lis r3, lbl_80314E08@ha
    addi r3, r3, lbl_80314E08@l
    bl fn_800D7820
    mr r3, r22
    mr r4, r21
    li r5, 0x0
    li r6, 0xb9
    li r7, 0x0
    bl fn_801040F0
    lis r3, 0x8000
    bl fn_800D88DC
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    li r3, 0x1
    bl fn_800DC1D4
@L_800FD2EC:
    stw r26, 0x30(r31)
    lwz r0, 0x14(r1)
    stb r24, 0x40(r31)
    lwz r3, 0x18(r1)
    stw r0, 0x34(r31)
    lwz r0, 0x1c(r1)
    stw r3, 0x38(r31)
    stw r0, 0x3c(r31)
    lwz r3, lbl_80478B08(r13)
    lbz r0, 0x1d(r3)
    extsb r0, r0
    slwi r0, r0, 2
    add r3, r3, r0
    lwz r3, 0xc(r3)
    bl fn_800EF504
    lis r3, 0x8000
    bl fn_800D888C
    li r3, 0x0
@L_800FD334:
    lmw r19, 0x6c(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
