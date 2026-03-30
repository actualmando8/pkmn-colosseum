    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r23, 0x1c(r1)
    mr r24, r3
    mr r25, r4
    mr r30, r5
    mr r26, r6
    mr r27, r7
    extsb r28, r24
    li r29, 0x0
    cmplwi r28, 0x9
    mr r31, r29
    mr r4, r29
    bgt @L_80106B7C
    lis r3, jumptable_8035B350@ha
    rlwinm r0, r28, 2, 0, 29
    addi r3, r3, jumptable_8035B350@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r4, 0x40
    b @L_80106B7C
    li r4, 0xf
    b @L_80106B7C
    li r4, 0x10
    b @L_80106B7C
    li r4, 0x40
    b @L_80106B7C
    li r4, 0x50
    b @L_80106B7C
    li r4, 0x51
    b @L_80106B7C
    li r4, 0x10c
    b @L_80106B7C
    li r4, 0xe6
    b @L_80106B7C
    li r4, 0xe8
    b @L_80106B7C
    li r4, 0x107
@L_80106B7C:
    cmpwi r4, 0x0
    bne @L_80106B8C
    li r3, 0x0
    b @L_80106D28
@L_80106B8C:
    extsb r0, r24
    cmpwi r0, 0x6
    beq @L_80106C58
    lbz r3, lbl_80478B30(r13)
    extsb r4, r3
    cmpw r0, r4
    beq @L_80106C48
    lbz r0, lbl_8047AD10(r13)
    cmplwi r0, 0x0
    bne @L_80106C48
    cmplwi r4, 0x9
    li r23, 0x0
    bgt @L_80106C24
    lis r3, jumptable_8035B328@ha
    rlwinm r0, r4, 2, 0, 29
    addi r3, r3, jumptable_8035B328@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r23, 0x40
    b @L_80106C24
    li r23, 0xf
    b @L_80106C24
    li r23, 0x10
    b @L_80106C24
    li r23, 0x40
    b @L_80106C24
    li r23, 0x50
    b @L_80106C24
    li r23, 0x51
    b @L_80106C24
    li r23, 0x10c
    b @L_80106C24
    li r23, 0xe6
    b @L_80106C24
    li r23, 0xe8
    b @L_80106C24
    li r23, 0x107
@L_80106C24:
    mr r3, r23
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80106C48
    mr r3, r23
    mr r5, r30
    li r4, 0x2
    bl fn_80102568
@L_80106C48:
    stb r24, lbl_80478B30(r13)
    bl fn_801046B8
    mr r4, r3
    b @L_80106C68
@L_80106C58:
    li r29, 0x1
    li r4, -0x1
    stb r29, lbl_8047AD10(r13)
    li r30, 0x0
@L_80106C68:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80106C7C
    ori r0, r31, 0x1
    rlwinm r31, r0, 0, 24, 31
@L_80106C7C:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80106C90
    ori r0, r31, 0x2
    rlwinm r31, r0, 0, 24, 31
@L_80106C90:
    cmplwi r28, 0x9
    li r3, 0x0
    bgt @L_80106D00
    lis r5, jumptable_8035B300@ha
    rlwinm r0, r28, 2, 0, 29
    addi r5, r5, jumptable_8035B300@l
    lwzx r0, r5, r0
    mtctr r0
    bctr
    li r3, 0x40
    b @L_80106D00
    li r3, 0xf
    b @L_80106D00
    li r3, 0x10
    b @L_80106D00
    li r3, 0x40
    b @L_80106D00
    li r3, 0x50
    b @L_80106D00
    li r3, 0x51
    b @L_80106D00
    li r3, 0x10c
    b @L_80106D00
    li r3, 0xe6
    b @L_80106D00
    li r3, 0xe8
    b @L_80106D00
    li r3, 0x107
@L_80106D00:
    rlwinm r0, r27, 0, 24, 31
    mr r7, r30
    stw r0, 0x8(r1)
    mr r9, r25
    rlwinm r10, r31, 0, 24, 31
    li r5, 0x0
    li r6, 0x0
    li r8, 0x3
    crxor 6, 6, 6
    bl fn_801026A4
@L_80106D28:
    lmw r23, 0x1c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

