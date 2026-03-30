    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r25, r3
    mr r26, r4
    mr r30, r5
    mr r27, r6
    extsb r28, r25
    li r29, 0x0
    cmplwi r28, 0x9
    mr r31, r29
    mr r4, r29
    bgt @L_80106DD8
    lis r3, jumptable_8035B3C8@ha
    rlwinm r0, r28, 2, 0, 29
    addi r3, r3, jumptable_8035B3C8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r4, 0x40
    b @L_80106DD8
    li r4, 0xf
    b @L_80106DD8
    li r4, 0x10
    b @L_80106DD8
    li r4, 0x40
    b @L_80106DD8
    li r4, 0x50
    b @L_80106DD8
    li r4, 0x51
    b @L_80106DD8
    li r4, 0x10c
    b @L_80106DD8
    li r4, 0xe6
    b @L_80106DD8
    li r4, 0xe8
    b @L_80106DD8
    li r4, 0x107
@L_80106DD8:
    cmpwi r4, 0x0
    bne @L_80106DE8
    li r3, 0x0
    b @L_80106F84
@L_80106DE8:
    extsb r0, r25
    cmpwi r0, 0x6
    beq @L_80106EB4
    lbz r3, lbl_80478B30(r13)
    extsb r4, r3
    cmpw r0, r4
    beq @L_80106EA4
    lbz r0, lbl_8047AD10(r13)
    cmplwi r0, 0x0
    bne @L_80106EA4
    cmplwi r4, 0x9
    li r24, 0x0
    bgt @L_80106E80
    lis r3, jumptable_8035B3A0@ha
    rlwinm r0, r4, 2, 0, 29
    addi r3, r3, jumptable_8035B3A0@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r24, 0x40
    b @L_80106E80
    li r24, 0xf
    b @L_80106E80
    li r24, 0x10
    b @L_80106E80
    li r24, 0x40
    b @L_80106E80
    li r24, 0x50
    b @L_80106E80
    li r24, 0x51
    b @L_80106E80
    li r24, 0x10c
    b @L_80106E80
    li r24, 0xe6
    b @L_80106E80
    li r24, 0xe8
    b @L_80106E80
    li r24, 0x107
@L_80106E80:
    mr r3, r24
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80106EA4
    mr r3, r24
    mr r5, r30
    li r4, 0x2
    bl fn_80102568
@L_80106EA4:
    stb r25, lbl_80478B30(r13)
    bl fn_801046B8
    mr r4, r3
    b @L_80106EC4
@L_80106EB4:
    li r29, 0x1
    li r4, -0x1
    stb r29, lbl_8047AD10(r13)
    li r30, 0x0
@L_80106EC4:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80106ED8
    ori r0, r31, 0x1
    rlwinm r31, r0, 0, 24, 31
@L_80106ED8:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80106EEC
    ori r0, r31, 0x2
    rlwinm r31, r0, 0, 24, 31
@L_80106EEC:
    cmplwi r28, 0x9
    li r3, 0x0
    bgt @L_80106F5C
    lis r5, jumptable_8035B378@ha
    rlwinm r0, r28, 2, 0, 29
    addi r5, r5, jumptable_8035B378@l
    lwzx r0, r5, r0
    mtctr r0
    bctr
    li r3, 0x40
    b @L_80106F5C
    li r3, 0xf
    b @L_80106F5C
    li r3, 0x10
    b @L_80106F5C
    li r3, 0x40
    b @L_80106F5C
    li r3, 0x50
    b @L_80106F5C
    li r3, 0x51
    b @L_80106F5C
    li r3, 0x10c
    b @L_80106F5C
    li r3, 0xe6
    b @L_80106F5C
    li r3, 0xe8
    b @L_80106F5C
    li r3, 0x107
@L_80106F5C:
    li r0, 0x0
    mr r7, r30
    stw r0, 0x8(r1)
    mr r9, r26
    rlwinm r10, r31, 0, 24, 31
    li r5, 0x0
    li r6, 0x0
    li r8, 0x3
    crxor 6, 6, 6
    bl fn_801026A4
@L_80106F84:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

