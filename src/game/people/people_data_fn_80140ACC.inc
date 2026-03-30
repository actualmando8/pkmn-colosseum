    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr. r24, r3
    mr r25, r4
    mr r26, r5
    mr r29, r6
    mr r21, r7
    mr r27, r9
    bne @L_80140B00
    li r3, -0x1
    b @L_801412F4
    @L_80140B00:
    mr r4, r26
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80140B24
    li r0, 0x0
    b @L_80140B40
    @L_80140B24:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r26, 16
    cmplw r3, r0
    blt @L_80140B3C
    li r0, 0x0
    b @L_80140B40
    @L_80140B3C:
    li r0, 0x1
    @L_80140B40:
    clrlwi. r0, r0, 24
    bne @L_80140B50
    li r3, -0x1
    b @L_801412F4
    @L_80140B50:
    extsh. r0, r21
    li r28, 0x0
    bge @L_80140FD0
    clrlwi r29, r29, 16
    clrlwi r31, r26, 16
    @L_80140B64:
    cmplwi r24, 0x0
    bne @L_80140B74
    li r30, 0x0
    b @L_80140CB8
    @L_80140B74:
    mr r4, r26
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80140B98
    li r0, 0x0
    b @L_80140BB0
    @L_80140B98:
    lwz r0, lbl_80478BD8(r13)
    cmplw r31, r0
    blt @L_80140BAC
    li r0, 0x0
    b @L_80140BB0
    @L_80140BAC:
    li r0, 0x1
    @L_80140BB0:
    clrlwi. r0, r0, 24
    bne @L_80140BC0
    li r30, 0x0
    b @L_80140CB8
    @L_80140BC0:
    clrlwi r23, r25, 16
    li r22, 0x0
    b @L_80140CA8
    @L_80140BCC:
    clrlslwi r0, r22, 16, 2
    add. r30, r24, r0
    bne @L_80140BE0
    li r0, 0x0
    b @L_80140C98
    @L_80140BE0:
    bne @L_80140BEC
    li r0, 0x0
    b @L_80140C60
    @L_80140BEC:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r21, r3, 16
    bne @L_80140C10
    li r0, 0x0
    b @L_80140C60
    @L_80140C10:
    mr r4, r21
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80140C34
    li r0, 0x0
    b @L_80140C4C
    @L_80140C34:
    lwz r0, lbl_80478BD8(r13)
    cmplw r21, r0
    blt @L_80140C48
    li r0, 0x0
    b @L_80140C4C
    @L_80140C48:
    li r0, 0x1
    @L_80140C4C:
    clrlwi. r0, r0, 24
    bne @L_80140C5C
    li r0, 0x0
    b @L_80140C60
    @L_80140C5C:
    li r0, 0x1
    @L_80140C60:
    clrlwi. r0, r0, 24
    bne @L_80140C70
    li r0, 0x0
    b @L_80140C98
    @L_80140C70:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    cmpw r31, r3
    bne @L_80140C94
    li r0, 0x1
    b @L_80140C98
    @L_80140C94:
    li r0, 0x0
    @L_80140C98:
    clrlwi. r0, r0, 24
    beq @L_80140CA4
    b @L_80140CB8
    @L_80140CA4:
    addi r22, r22, 0x1
    @L_80140CA8:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_80140BCC
    li r30, 0x0
    @L_80140CB8:
    cmplwi r30, 0x0
    beq @L_801412D4
    bne @L_80140CCC
    li r22, -0x1
    b @L_80140F1C
    @L_80140CCC:
    mr r4, r26
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80140CF0
    li r0, 0x0
    b @L_80140D08
    @L_80140CF0:
    lwz r0, lbl_80478BD8(r13)
    cmplw r31, r0
    blt @L_80140D04
    li r0, 0x0
    b @L_80140D08
    @L_80140D04:
    li r0, 0x1
    @L_80140D08:
    clrlwi. r0, r0, 24
    bne @L_80140D18
    li r22, -0x1
    b @L_80140F1C
    @L_80140D18:
    cmplwi r30, 0x0
    bne @L_80140D28
    li r0, 0x0
    b @L_80140DE0
    @L_80140D28:
    bne @L_80140D34
    li r0, 0x0
    b @L_80140DA8
    @L_80140D34:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r21, r3, 16
    bne @L_80140D58
    li r0, 0x0
    b @L_80140DA8
    @L_80140D58:
    mr r4, r21
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80140D7C
    li r0, 0x0
    b @L_80140D94
    @L_80140D7C:
    lwz r0, lbl_80478BD8(r13)
    cmplw r21, r0
    blt @L_80140D90
    li r0, 0x0
    b @L_80140D94
    @L_80140D90:
    li r0, 0x1
    @L_80140D94:
    clrlwi. r0, r0, 24
    bne @L_80140DA4
    li r0, 0x0
    b @L_80140DA8
    @L_80140DA4:
    li r0, 0x1
    @L_80140DA8:
    clrlwi. r0, r0, 24
    bne @L_80140DB8
    li r0, 0x0
    b @L_80140DE0
    @L_80140DB8:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    cmpw r31, r3
    bne @L_80140DDC
    li r0, 0x1
    b @L_80140DE0
    @L_80140DDC:
    li r0, 0x0
    @L_80140DE0:
    clrlwi. r0, r0, 24
    bne @L_80140DF0
    li r22, -0x1
    b @L_80140F1C
    @L_80140DF0:
    cmplwi r30, 0x0
    bne @L_80140E00
    li r22, -0x1
    b @L_80140F1C
    @L_80140E00:
    bne @L_80140E0C
    li r0, 0x0
    b @L_80140E80
    @L_80140E0C:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r21, r3, 16
    bne @L_80140E30
    li r0, 0x0
    b @L_80140E80
    @L_80140E30:
    mr r4, r21
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80140E54
    li r0, 0x0
    b @L_80140E6C
    @L_80140E54:
    lwz r0, lbl_80478BD8(r13)
    cmplw r21, r0
    blt @L_80140E68
    li r0, 0x0
    b @L_80140E6C
    @L_80140E68:
    li r0, 0x1
    @L_80140E6C:
    clrlwi. r0, r0, 24
    bne @L_80140E7C
    li r0, 0x0
    b @L_80140E80
    @L_80140E7C:
    li r0, 0x1
    @L_80140E80:
    clrlwi. r0, r0, 24
    bne @L_80140E90
    li r22, -0x1
    b @L_80140F1C
    @L_80140E90:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r3, r3, 16
    clrlwi r0, r29, 16
    subf. r23, r0, r3
    bge @L_80140EC0
    subf r22, r23, r0
    li r23, 0x0
    b @L_80140EC4
    @L_80140EC0:
    li r22, 0x0
    @L_80140EC4:
    clrlwi r7, r23, 16
    mr r3, r30
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142B24
    cmpwi r23, 0x0
    bgt @L_80140F1C
    cmplwi r30, 0x0
    beq @L_80140F1C
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    mr r3, r30
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    @L_80140F1C:
    cmpwi r22, 0x0
    mr r21, r22
    bge @L_80140F30
    mr r3, r22
    b @L_801412F4
    @L_80140F30:
    cmplwi r30, 0x0
    bne @L_80140F40
    li r0, 0x0
    b @L_80140FB4
    @L_80140F40:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r22, r3, 16
    bne @L_80140F64
    li r0, 0x0
    b @L_80140FB4
    @L_80140F64:
    mr r4, r22
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80140F88
    li r0, 0x0
    b @L_80140FA0
    @L_80140F88:
    lwz r0, lbl_80478BD8(r13)
    cmplw r22, r0
    blt @L_80140F9C
    li r0, 0x0
    b @L_80140FA0
    @L_80140F9C:
    li r0, 0x1
    @L_80140FA0:
    clrlwi. r0, r0, 24
    bne @L_80140FB0
    li r0, 0x0
    b @L_80140FB4
    @L_80140FB0:
    li r0, 0x1
    @L_80140FB4:
    clrlwi. r0, r0, 24
    bne @L_80140FC0
    li r28, 0x1
    @L_80140FC0:
    cmpwi r21, 0x0
    mr r29, r21
    bgt @L_80140B64
    b @L_801412D4
    @L_80140FD0:
    extsh r0, r21
    slwi r0, r0, 2
    add. r30, r24, r0
    bne @L_80140FE8
    li r22, -0x1
    b @L_80141240
    @L_80140FE8:
    mr r4, r26
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_8014100C
    li r0, 0x0
    b @L_80141028
    @L_8014100C:
    lwz r0, lbl_80478BD8(r13)
    clrlwi r3, r26, 16
    cmplw r3, r0
    blt @L_80141024
    li r0, 0x0
    b @L_80141028
    @L_80141024:
    li r0, 0x1
    @L_80141028:
    clrlwi. r0, r0, 24
    bne @L_80141038
    li r22, -0x1
    b @L_80141240
    @L_80141038:
    cmplwi r30, 0x0
    bne @L_80141048
    li r0, 0x0
    b @L_80141104
    @L_80141048:
    bne @L_80141054
    li r0, 0x0
    b @L_801410C8
    @L_80141054:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r21, r3, 16
    bne @L_80141078
    li r0, 0x0
    b @L_801410C8
    @L_80141078:
    mr r4, r21
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_8014109C
    li r0, 0x0
    b @L_801410B4
    @L_8014109C:
    lwz r0, lbl_80478BD8(r13)
    cmplw r21, r0
    blt @L_801410B0
    li r0, 0x0
    b @L_801410B4
    @L_801410B0:
    li r0, 0x1
    @L_801410B4:
    clrlwi. r0, r0, 24
    bne @L_801410C4
    li r0, 0x0
    b @L_801410C8
    @L_801410C4:
    li r0, 0x1
    @L_801410C8:
    clrlwi. r0, r0, 24
    bne @L_801410D8
    li r0, 0x0
    b @L_80141104
    @L_801410D8:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r0, r26, 16
    cmpw r0, r3
    bne @L_80141100
    li r0, 0x1
    b @L_80141104
    @L_80141100:
    li r0, 0x0
    @L_80141104:
    clrlwi. r0, r0, 24
    bne @L_80141114
    li r22, -0x1
    b @L_80141240
    @L_80141114:
    cmplwi r30, 0x0
    bne @L_80141124
    li r22, -0x1
    b @L_80141240
    @L_80141124:
    bne @L_80141130
    li r0, 0x0
    b @L_801411A4
    @L_80141130:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r21, r3, 16
    bne @L_80141154
    li r0, 0x0
    b @L_801411A4
    @L_80141154:
    mr r4, r21
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_80141178
    li r0, 0x0
    b @L_80141190
    @L_80141178:
    lwz r0, lbl_80478BD8(r13)
    cmplw r21, r0
    blt @L_8014118C
    li r0, 0x0
    b @L_80141190
    @L_8014118C:
    li r0, 0x1
    @L_80141190:
    clrlwi. r0, r0, 24
    bne @L_801411A0
    li r0, 0x0
    b @L_801411A4
    @L_801411A0:
    li r0, 0x1
    @L_801411A4:
    clrlwi. r0, r0, 24
    bne @L_801411B4
    li r22, -0x1
    b @L_80141240
    @L_801411B4:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi r3, r3, 16
    clrlwi r0, r29, 16
    subf. r23, r0, r3
    bge @L_801411E4
    subf r22, r23, r0
    li r23, 0x0
    b @L_801411E8
    @L_801411E4:
    li r22, 0x0
    @L_801411E8:
    clrlwi r7, r23, 16
    mr r3, r30
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142B24
    cmpwi r23, 0x0
    bgt @L_80141240
    cmplwi r30, 0x0
    beq @L_80141240
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    mr r3, r30
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    li r7, 0x0
    bl fn_80142B24
    @L_80141240:
    cmplwi r30, 0x0
    mr r29, r22
    bne @L_80141254
    li r0, 0x0
    b @L_801412C8
    @L_80141254:
    mr r3, r30
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r22, r3, 16
    bne @L_80141278
    li r0, 0x0
    b @L_801412C8
    @L_80141278:
    mr r4, r22
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80142CF4
    cmpwi r3, 0x0
    bne @L_8014129C
    li r0, 0x0
    b @L_801412B4
    @L_8014129C:
    lwz r0, lbl_80478BD8(r13)
    cmplw r22, r0
    blt @L_801412B0
    li r0, 0x0
    b @L_801412B4
    @L_801412B0:
    li r0, 0x1
    @L_801412B4:
    clrlwi. r0, r0, 24
    bne @L_801412C4
    li r0, 0x0
    b @L_801412C8
    @L_801412C4:
    li r0, 0x1
    @L_801412C8:
    clrlwi. r0, r0, 24
    bne @L_801412D4
    li r28, 0x1
    @L_801412D4:
    clrlwi r0, r28, 24
    cmplwi r0, 0x1
    bne @L_801412F0
    mr r3, r24
    mr r4, r25
    mr r5, r27
    bl fn_801425E8
    @L_801412F0:
    mr r3, r29
    @L_801412F4:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
