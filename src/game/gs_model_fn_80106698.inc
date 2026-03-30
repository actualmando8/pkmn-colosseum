    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r29, r3
    mr r30, r4
    mr r26, r5
    mr r31, r6
    li r5, 0x40
    li r28, 0x0
    cmpwi r5, 0x0
    bne @L_801066CC
    b @L_801067D0
@L_801066CC:
    lbz r0, lbl_80478B30(r13)
    extsb r4, r0
    cmpwi r4, 0x3
    beq @L_8010677C
    lbz r0, lbl_8047AD10(r13)
    cmplwi r0, 0x0
    bne @L_8010677C
    cmplwi r4, 0x9
    mr r27, r28
    bgt @L_80106758
    lis r3, jumptable_8035B260@ha
    rlwinm r0, r4, 2, 0, 29
    addi r3, r3, jumptable_8035B260@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r27, r5
    b @L_80106758
    li r27, 0xf
    b @L_80106758
    li r27, 0x10
    b @L_80106758
    mr r27, r5
    b @L_80106758
    li r27, 0x50
    b @L_80106758
    li r27, 0x51
    b @L_80106758
    li r27, 0x10c
    b @L_80106758
    li r27, 0xe6
    b @L_80106758
    li r27, 0xe8
    b @L_80106758
    li r27, 0x107
@L_80106758:
    mr r3, r27
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010677C
    mr r3, r27
    mr r5, r30
    li r4, 0x2
    bl fn_80102568
@L_8010677C:
    li r0, 0x3
    stb r0, lbl_80478B30(r13)
    bl fn_801046B8
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_8010679C
    ori r0, r28, 0x1
    rlwinm r28, r0, 0, 24, 31
@L_8010679C:
    rlwinm r0, r31, 0, 24, 31
    mr r4, r3
    stw r0, 0x8(r1)
    mr r7, r30
    mr r9, r29
    rlwinm r10, r28, 0, 24, 31
    li r3, 0x40
    li r5, 0x0
    li r6, 0x0
    li r8, 0x3
    crxor 6, 6, 6
    bl fn_801026A4
    mr r28, r3
@L_801067D0:
    mr r3, r28
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr

