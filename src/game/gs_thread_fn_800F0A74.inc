    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr r30, r3
    mr r31, r4
    clrlwi r3, r4, 24
    lwz r21, lbl_8047AC08(r13)
    b @L_800F0AE4
    @L_800F0A98:
    lbz r0, 0x8(r21)
    cmplwi r0, 0x0
    beq @L_800F0AE0
    lbz r0, 0x9(r21)
    cmplwi r0, 0x0
    bne @L_800F0AE0
    lbz r0, 0xa(r21)
    cmplwi r0, 0x0
    bne @L_800F0AE0
    lbz r0, 0x15(r21)
    cmplwi r0, 0x0
    bne @L_800F0AE0
    cmplwi r3, 0x0
    beq @L_800F0AF0
    lwz r0, 0xc(r21)
    cmplw r0, r30
    bne @L_800F0AE0
    b @L_800F0AF0
    @L_800F0AE0:
    lwz r21, 0x4(r21)
    @L_800F0AE4:
    cmplwi r21, 0x0
    bne @L_800F0A98
    li r21, 0x0
    @L_800F0AF0:
    cmplwi r21, 0x0
    beq @L_800F0F38
    lhz r3, 0x22(r21)
    bl fn_800E27B0
    mr r0, r3
    lhz r3, 0x20(r21)
    mr r25, r0
    bl fn_800E27B0
    mr r24, r3
    lis r4, fn_800F02F4@ha
    lis r3, lbl_804019F0@ha
    clrlwi r29, r31, 24
    mr r23, r24
    addi r27, r4, fn_800F02F4@l
    addi r28, r3, lbl_804019F0@l
    b @L_800F0F30
    @L_800F0B30:
    cmplwi r21, 0x0
    bne @L_800F0B40
    lwz r3, lbl_8047AC08(r13)
    b @L_800F0B94
    @L_800F0B40:
    lwz r3, 0x4(r21)
    b @L_800F0B94
    @L_800F0B48:
    lbz r0, 0x8(r3)
    cmplwi r0, 0x0
    beq @L_800F0B90
    lbz r0, 0x9(r3)
    cmplwi r0, 0x0
    bne @L_800F0B90
    lbz r0, 0xa(r3)
    cmplwi r0, 0x0
    bne @L_800F0B90
    lbz r0, 0x15(r3)
    cmplwi r0, 0x0
    bne @L_800F0B90
    cmplwi r29, 0x0
    beq @L_800F0BA0
    lwz r0, 0xc(r3)
    cmplw r0, r30
    bne @L_800F0B90
    b @L_800F0BA0
    @L_800F0B90:
    lwz r3, 0x4(r3)
    @L_800F0B94:
    cmplwi r3, 0x0
    bne @L_800F0B48
    li r3, 0x0
    @L_800F0BA0:
    cmplwi r3, 0x0
    stw r3, lbl_8047AC04(r13)
    beq @L_800F0BB8
    lhz r3, 0x20(r3)
    bl fn_800E27B0
    mr r22, r3
    @L_800F0BB8:
    stw r21, lbl_8047AC00(r13)
    li r0, 0x1
    stb r0, 0x14(r21)
    lwz r0, 0x4(r25)
    add r0, r23, r0
    stw r0, 0x4(r25)
    stw r27, 0x84(r28)
    stw r28, lbl_8047AC24(r13)
    stw r25, lbl_8047AC20(r13)
    lbz r0, 0x16(r21)
    stw r0, lbl_8047AC10(r13)
    bl fn_800F028C
    lwz r3, 0x4(r25)
    li r0, 0x0
    subf r3, r23, r3
    stw r3, 0x4(r25)
    stw r0, lbl_8047AC00(r13)
    lbz r0, 0x15(r21)
    cmplwi r0, 0x0
    beq @L_800F0CC0
    mr r3, r23
    bl fn_800EEA50
    clrlwi. r0, r3, 24
    beq @L_800F0C20
    mr r3, r23
    bl fn_800EEB34
    @L_800F0C20:
    lhz r3, 0x22(r21)
    bl fn_800E24B0
    lhz r3, 0x20(r21)
    bl fn_800E24B0
    lwz r0, lbl_8047AC00(r13)
    cmplw r21, r0
    beq @L_800F0C48
    lwz r0, lbl_8047AC04(r13)
    cmplw r21, r0
    bne @L_800F0C64
    @L_800F0C48:
    li r3, 0x1
    stb r3, 0x15(r21)
    lwz r0, lbl_8047AC04(r13)
    cmplw r21, r0
    bne @L_800F0D08
    stb r3, lbl_8047AC0C(r13)
    b @L_800F0D08
    @L_800F0C64:
    li r0, 0x0
    stb r0, 0x14(r21)
    stb r0, 0x8(r21)
    lwz r3, 0x0(r21)
    cmplwi r3, 0x0
    beq @L_800F0C84
    lwz r0, 0x4(r21)
    stw r0, 0x4(r3)
    @L_800F0C84:
    lwz r3, 0x4(r21)
    cmplwi r3, 0x0
    beq @L_800F0C98
    lwz r0, 0x0(r21)
    stw r0, 0x0(r3)
    @L_800F0C98:
    lwz r0, lbl_8047AC08(r13)
    cmplw r0, r21
    bne @L_800F0CAC
    lwz r0, 0x4(r21)
    stw r0, lbl_8047AC08(r13)
    @L_800F0CAC:
    lhz r3, 0x20(r21)
    bl fn_800E209C
    lhz r3, 0x22(r21)
    bl fn_800E209C
    b @L_800F0D08
    @L_800F0CC0:
    mr r3, r23
    bl fn_800EEA50
    clrlwi. r0, r3, 24
    beq @L_800F0CF8
    lwz r4, 0x10(r21)
    mr r3, r24
    bl DCFlushRange
    lwz r5, 0x10(r21)
    mr r3, r24
    mr r4, r23
    bl fn_800EEA98
    bl fn_800EEA6C
    mr r3, r23
    bl fn_800EEB34
    @L_800F0CF8:
    lhz r3, 0x22(r21)
    bl fn_800E24B0
    lhz r3, 0x20(r21)
    bl fn_800E24B0
    @L_800F0D08:
    lbz r0, lbl_8047AC0C(r13)
    cmplwi r0, 0x0
    beq @L_800F0D94
    cmplwi r21, 0x0
    bne @L_800F0D24
    lwz r4, lbl_8047AC08(r13)
    b @L_800F0D28
    @L_800F0D24:
    lwz r4, 0x4(r21)
    @L_800F0D28:
    clrlwi r3, r31, 24
    b @L_800F0D7C
    @L_800F0D30:
    lbz r0, 0x8(r4)
    cmplwi r0, 0x0
    beq @L_800F0D78
    lbz r0, 0x9(r4)
    cmplwi r0, 0x0
    bne @L_800F0D78
    lbz r0, 0xa(r4)
    cmplwi r0, 0x0
    bne @L_800F0D78
    lbz r0, 0x15(r4)
    cmplwi r0, 0x0
    bne @L_800F0D78
    cmplwi r3, 0x0
    beq @L_800F0D88
    lwz r0, 0xc(r4)
    cmplw r0, r30
    bne @L_800F0D78
    b @L_800F0D88
    @L_800F0D78:
    lwz r4, 0x4(r4)
    @L_800F0D7C:
    cmplwi r4, 0x0
    bne @L_800F0D30
    li r4, 0x0
    @L_800F0D88:
    lwz r0, lbl_8047AC04(r13)
    cmplw r0, r4
    bne @L_800F0DBC
    @L_800F0D94:
    lwz r3, lbl_8047AC04(r13)
    cmplwi r3, 0x0
    mr r21, r3
    beq @L_800F0F28
    lhz r3, 0x22(r3)
    mr r23, r22
    mr r24, r22
    bl fn_800E27B0
    mr r25, r3
    b @L_800F0F28
    @L_800F0DBC:
    mr r3, r22
    bl fn_800EEA50
    clrlwi. r0, r3, 24
    beq @L_800F0DD8
    bl fn_800EEA6C
    mr r3, r22
    bl fn_800EEB34
    @L_800F0DD8:
    lwz r3, lbl_8047AC04(r13)
    cmplwi r3, 0x0
    beq @L_800F0E88
    lhz r3, 0x20(r3)
    bl fn_800E24B0
    lwz r26, lbl_8047AC04(r13)
    lbz r0, 0x15(r26)
    cmplwi r0, 0x0
    beq @L_800F0E88
    lwz r0, lbl_8047AC00(r13)
    li r3, 0x0
    stw r3, lbl_8047AC04(r13)
    cmplw r26, r0
    beq @L_800F0E18
    cmplwi r26, 0x0
    bne @L_800F0E34
    @L_800F0E18:
    li r3, 0x1
    stb r3, 0x15(r26)
    lwz r0, lbl_8047AC04(r13)
    cmplw r26, r0
    bne @L_800F0E88
    stb r3, lbl_8047AC0C(r13)
    b @L_800F0E88
    @L_800F0E34:
    stb r3, 0x14(r26)
    stb r3, 0x8(r26)
    lwz r3, 0x0(r26)
    cmplwi r3, 0x0
    beq @L_800F0E50
    lwz r0, 0x4(r26)
    stw r0, 0x4(r3)
    @L_800F0E50:
    lwz r3, 0x4(r26)
    cmplwi r3, 0x0
    beq @L_800F0E64
    lwz r0, 0x0(r26)
    stw r0, 0x0(r3)
    @L_800F0E64:
    lwz r0, lbl_8047AC08(r13)
    cmplw r0, r26
    bne @L_800F0E78
    lwz r0, 0x4(r26)
    stw r0, lbl_8047AC08(r13)
    @L_800F0E78:
    lhz r3, 0x20(r26)
    bl fn_800E209C
    lhz r3, 0x22(r26)
    bl fn_800E209C
    @L_800F0E88:
    cmplwi r21, 0x0
    bne @L_800F0E98
    lwz r21, lbl_8047AC08(r13)
    b @L_800F0E9C
    @L_800F0E98:
    lwz r21, 0x4(r21)
    @L_800F0E9C:
    clrlwi r3, r31, 24
    b @L_800F0EF0
    @L_800F0EA4:
    lbz r0, 0x8(r21)
    cmplwi r0, 0x0
    beq @L_800F0EEC
    lbz r0, 0x9(r21)
    cmplwi r0, 0x0
    bne @L_800F0EEC
    lbz r0, 0xa(r21)
    cmplwi r0, 0x0
    bne @L_800F0EEC
    lbz r0, 0x15(r21)
    cmplwi r0, 0x0
    bne @L_800F0EEC
    cmplwi r3, 0x0
    beq @L_800F0EFC
    lwz r0, 0xc(r21)
    cmplw r0, r30
    bne @L_800F0EEC
    b @L_800F0EFC
    @L_800F0EEC:
    lwz r21, 0x4(r21)
    @L_800F0EF0:
    cmplwi r21, 0x0
    bne @L_800F0EA4
    li r21, 0x0
    @L_800F0EFC:
    cmplwi r21, 0x0
    stw r21, lbl_8047AC04(r13)
    beq @L_800F0F28
    lhz r3, 0x22(r21)
    bl fn_800E27B0
    mr r0, r3
    lhz r3, 0x20(r21)
    mr r25, r0
    bl fn_800E27B0
    mr r24, r3
    mr r23, r24
    @L_800F0F28:
    li r0, 0x0
    stb r0, lbl_8047AC0C(r13)
    @L_800F0F30:
    cmplwi r21, 0x0
    bne @L_800F0B30
    @L_800F0F38:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
