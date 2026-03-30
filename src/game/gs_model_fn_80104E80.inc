    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r23, r5
    mr r29, r6
    mr r30, r7
    mr r31, r8
    cmpwi r23, 0x0
    ble @L_80104ED8
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r3, 0xc(r3)
    b @L_80104ED0
@L_80104EC0:
    lwz r0, 0x4(r3)
    cmpw r0, r23
    beq @L_80104ED8
    lwz r3, 0x10(r3)
@L_80104ED0:
    cmplwi r3, 0x0
    bne @L_80104EC0
@L_80104ED8:
    mr r3, r28
    bl fn_8005DA18
    cmpwi r28, 0x0
    mr r26, r3
    bgt @L_80104EF4
    li r4, 0x0
    b @L_80104F24
@L_80104EF4:
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r4, 0xc(r3)
    b @L_80104F18
@L_80104F04:
    lwz r0, 0x4(r4)
    cmpw r0, r28
    bne @L_80104F14
    b @L_80104F24
@L_80104F14:
    lwz r4, 0x10(r4)
@L_80104F18:
    cmplwi r4, 0x0
    bne @L_80104F04
    li r4, 0x0
@L_80104F24:
    cmplwi r4, 0x0
    mr r25, r4
    bne @L_801051AC
    lbz r0, 0x0(r26)
    rlwinm r0, r0, 28, 31, 31
    cmplwi r0, 0x0
    beq @L_80104F48
    li r24, 0x64
    b @L_80104FDC
@L_80104F48:
    rlwinm r0, r29, 0, 30, 30
    cmplwi r0, 0x0
    beq @L_80104F5C
    li r24, 0x64
    b @L_80104FDC
@L_80104F5C:
    cmpwi r23, 0x0
    bne @L_80104F70
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r23, 0x4(r3)
@L_80104F70:
    cmpwi r23, 0x0
    bgt @L_80104F80
    li r3, 0x0
    b @L_80104FB0
@L_80104F80:
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r3, 0xc(r3)
    b @L_80104FA4
@L_80104F90:
    lwz r0, 0x4(r3)
    cmpw r0, r23
    bne @L_80104FA0
    b @L_80104FB0
@L_80104FA0:
    lwz r3, 0x10(r3)
@L_80104FA4:
    cmplwi r3, 0x0
    bne @L_80104F90
    li r3, 0x0
@L_80104FB0:
    cmplwi r3, 0x0
    beq @L_80104FC8
    lbz r3, 0x9(r3)
    addi r0, r3, 0x1
    extsb r24, r0
    b @L_80104FCC
@L_80104FC8:
    li r24, 0x0
@L_80104FCC:
    extsb r0, r24
    cmpwi r0, 0x64
    ble @L_80104FDC
    li r24, 0x64
@L_80104FDC:
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r25, 0x8(r3)
    lhz r0, 0x0(r3)
    mtctr r0
    cmpwi r0, 0x0
    ble @L_80105040
@L_80104FF8:
    lbz r0, 0x0(r25)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80105038
    mr r3, r25
    li r4, 0x0
    li r5, 0xb4
    bl memset
    li r0, 0x7
    lfs f0, lbl_8047CDEC(r13)
    stb r0, 0x0(r25)
    li r0, -0x1
    stfs f0, 0x8c(r25)
    stfs f0, 0x90(r25)
    stw r0, 0x88(r25)
    b @L_80105054
@L_80105038:
    addi r25, r25, 0xb4
    bdnz @L_80104FF8
@L_80105040:
    lis r3, lbl_80271E94@ha
    addi r3, r3, lbl_80271E94@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r25, 0x0
@L_80105054:
    cmplwi r25, 0x0
    bne @L_80105064
    li r3, 0x0
    b @L_801052E0
@L_80105064:
    li r3, 0x0
    cmpwi r23, 0x0
    stb r3, 0x1(r25)
    stw r28, 0x4(r25)
    stb r24, 0x9(r25)
    lbz r0, 0x1(r26)
    rlwinm r0, r0, 27, 29, 31
    stb r0, 0x1a(r25)
    bgt @L_8010508C
    b @L_801050BC
@L_8010508C:
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r3, 0xc(r3)
    b @L_801050B0
@L_8010509C:
    lwz r0, 0x4(r3)
    cmpw r0, r23
    bne @L_801050AC
    b @L_801050BC
@L_801050AC:
    lwz r3, 0x10(r3)
@L_801050B0:
    cmplwi r3, 0x0
    bne @L_8010509C
    li r3, 0x0
@L_801050BC:
    stw r3, 0xc(r25)
    lha r0, 0x6(r26)
    sth r0, 0x84(r25)
    lha r0, 0x8(r26)
    sth r0, 0x86(r25)
    lha r3, 0x4(r26)
    bl fn_8005D934
@L_801050D8:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 25, 31, 31
    cmplwi r0, 0x0
    beq @L_801050F4
    lbz r4, 0x19(r25)
    addi r0, r4, 0x1
    stb r0, 0x19(r25)
@L_801050F4:
    lbz r0, 0x0(r3)
    rlwinm r0, r0, 26, 31, 31
    cmplwi r0, 0x0
    bne @L_80105110
    lha r3, 0x18(r3)
    bl fn_8005D934
    b @L_801050D8
@L_80105110:
    mr r3, r25
    bl fn_80104A94
    lis r4, lbl_80404ACC@ha
    addi r4, r4, lbl_80404ACC@l
    lwz r5, 0xc(r4)
    cmplwi r5, 0x0
    bne @L_80105134
    stw r25, 0xc(r4)
    b @L_8010519C
@L_80105134:
    extsb r0, r24
@L_80105138:
    lbz r3, 0x9(r5)
    extsb r3, r3
    cmpw r3, r0
    ble @L_80105174
    lwz r0, 0x14(r5)
    stw r0, 0x14(r25)
    stw r5, 0x10(r25)
    lwz r3, 0x14(r5)
    cmplwi r3, 0x0
    bne @L_80105168
    stw r25, 0xc(r4)
    b @L_8010516C
@L_80105168:
    stw r25, 0x10(r3)
@L_8010516C:
    stw r25, 0x14(r5)
    b @L_8010519C
@L_80105174:
    lwz r3, 0x10(r5)
    cmplwi r3, 0x0
    bne @L_80105194
    stw r25, 0x10(r5)
    li r0, 0x0
    stw r5, 0x14(r25)
    stw r0, 0x10(r25)
    b @L_8010519C
@L_80105194:
    mr r5, r3
    b @L_80105138
@L_8010519C:
    mr r3, r28
    li r4, 0x4
    bl fn_80103484
    b @L_801051C4
@L_801051AC:
    li r3, 0x0
    li r0, 0x1
    stb r3, 0x98(r4)
    stb r3, 0x99(r4)
    stb r3, 0x1(r4)
    stb r0, 0x2(r4)
@L_801051C4:
    cmplwi r27, 0x0
    bne @L_801051E8
    lbz r3, 0x3(r26)
    cmplwi r3, 0x0
    beq @L_801051FC
    bl fn_80103E68
    rlwinm r0, r3, 16, 16, 31
    sth r0, 0x94(r25)
    b @L_801051FC
@L_801051E8:
    li r0, 0x0
    stb r0, 0x94(r25)
    lwz r0, 0x0(r27)
    extsb r0, r0
    stb r0, 0x95(r25)
@L_801051FC:
    mr r3, r25
    bl fn_80104CA0
    bl fn_80103CB0
    stb r3, 0xb(r25)
    rlwinm r0, r29, 0, 24, 31
    cmpwi r30, 0x8
    stb r0, 0x1b(r25)
    ble @L_80105220
    li r30, 0x8
@L_80105220:
    mr r23, r25
    li r24, 0x0
    b @L_80105248
@L_8010522C:
    mr r3, r31
    li r4, 0x1
    bl __va_arg
    lwz r0, 0x0(r3)
    addi r24, r24, 0x1
    stw r0, 0x60(r23)
    addi r23, r23, 0x4
@L_80105248:
    cmpw r24, r30
    blt @L_8010522C
    lbz r0, 0x1(r26)
    rlwinm r0, r0, 30, 29, 31
    cmplwi r0, 0x0
    bne @L_8010526C
    li r0, 0x1
    stb r0, 0x98(r25)
    b @L_80105284
@L_8010526C:
    rlwinm r0, r29, 0, 29, 29
    cmplwi r0, 0x0
    bne @L_80105284
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    stw r28, 0x4(r3)
@L_80105284:
    lis r3, lbl_80404ACC@ha
    addi r3, r3, lbl_80404ACC@l
    lwz r0, 0x4(r3)
    cmpw r0, r28
    beq @L_801052A0
    li r0, 0x1
    stb r0, 0x18(r25)
@L_801052A0:
    lwz r12, 0x14(r26)
    cmplwi r12, 0x0
    beq @L_801052B8
    mr r3, r25
    mtctr r12
    bctrl
@L_801052B8:
    lbz r0, 0x2(r25)
    extsb r0, r0
    cmpwi r0, 0x0
    beq @L_801052DC
    li r3, 0x0
    li r0, 0x2
    stb r3, 0xa(r25)
    stb r0, 0x1(r25)
    stb r3, 0x2(r25)
@L_801052DC:
    mr r3, r25
@L_801052E0:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
