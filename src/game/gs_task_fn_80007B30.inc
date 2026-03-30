    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r19, 0x1c(r1)
    lbz r25, lbl_80478828(r13)
    lbz r22, lbl_8047A271(r13)
    lbz r23, lbl_8047A270(r13)
    lbz r24, lbl_8047A284(r13)
    lbz r30, lbl_8047A285(r13)
    bl fn_802117FC
    lbz r29, lbl_8047A286(r13)
    mr r31, r3
    lbz r28, lbl_8047A280(r13)
    lhz r27, lbl_8047A282(r13)
    lbz r26, lbl_8047882E(r13)
    @L_80007B6C:
    li r3, 0xe
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    li r9, 0x0
    crxor 6, 6, 6
    bl fn_801026A4
    cmpwi r3, -0x1
    bne @L_80007BF8
    li r3, 0xe
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    cmplwi r25, 0x1
    bge @L_80007BB4
    li r25, 0x1
    @L_80007BB4:
    clrlwi r0, r25, 24
    cmplwi r0, 0x9
    ble @L_80007BC4
    li r25, 0x9
    @L_80007BC4:
    stb r25, lbl_80478828(r13)
    mr r3, r31
    stb r22, lbl_8047A271(r13)
    stb r23, lbl_8047A270(r13)
    stb r24, lbl_8047A284(r13)
    stb r30, lbl_8047A285(r13)
    bl fn_80211810
    stb r29, lbl_8047A286(r13)
    li r3, -0x1
    stb r28, lbl_8047A280(r13)
    sth r27, lbl_8047A282(r13)
    stb r26, lbl_8047882E(r13)
    b @L_80007FC8
    @L_80007BF8:
    cmpwi r3, -0x2
    bne @L_80007C3C
    li r3, 0x44
    li r4, 0x1
    bl fn_8010264C
    cmpwi r3, 0x0
    beq @L_80007C28
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_80007B6C
    @L_80007C28:
    li r3, 0x44
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    b @L_80007FB4
    @L_80007C3C:
    cmpwi r3, 0x57
    bne @L_80007C8C
    lbz r0, lbl_8047A271(r13)
    li r3, 0x7f
    li r4, -0x1
    li r5, -0x1
    cntlzw r0, r0
    srwi r6, r0, 5
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80007C78
    li r0, 0x1
    stb r0, lbl_8047A271(r13)
    b @L_80007B6C
    @L_80007C78:
    cmpwi r0, 0x1
    bne @L_80007B6C
    li r0, 0x0
    stb r0, lbl_8047A271(r13)
    b @L_80007B6C
    @L_80007C8C:
    cmpwi r3, 0x59
    bne @L_80007CF4
    lbz r3, lbl_80478828(r13)
    addi r4, r1, 0xc
    li r5, 0x1
    li r6, 0x32
    li r7, 0x32
    li r8, 0x0
    bl fn_8001E224
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007CC4
    bl fn_8001E200
    b @L_80007B6C
    @L_80007CC4:
    lwz r0, 0xc(r1)
    clrlwi r3, r0, 24
    cmplwi r3, 0x1
    bge @L_80007CD8
    li r3, 0x1
    @L_80007CD8:
    clrlwi r0, r3, 24
    cmplwi r0, 0x9
    ble @L_80007CE8
    li r3, 0x9
    @L_80007CE8:
    stb r3, lbl_80478828(r13)
    bl fn_8001E200
    b @L_80007B6C
    @L_80007CF4:
    cmpwi r3, 0x5b
    bne @L_80007D44
    lbz r0, lbl_8047A270(r13)
    li r3, 0x7f
    li r4, -0x1
    li r5, -0x1
    cntlzw r0, r0
    srwi r6, r0, 5
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80007D30
    li r0, 0x1
    stb r0, lbl_8047A270(r13)
    b @L_80007B6C
    @L_80007D30:
    cmpwi r0, 0x1
    bne @L_80007B6C
    li r0, 0x0
    stb r0, lbl_8047A270(r13)
    b @L_80007B6C
    @L_80007D44:
    cmpwi r3, 0x5d
    bne @L_80007D98
    bl fn_802117FC
    clrlwi r0, r3, 24
    li r3, 0x7f
    cntlzw r0, r0
    li r4, -0x1
    srwi r6, r0, 5
    li r5, -0x1
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80007D84
    li r3, 0x1
    bl fn_80211810
    b @L_80007B6C
    @L_80007D84:
    cmpwi r0, 0x1
    bne @L_80007B6C
    li r3, 0x0
    bl fn_80211810
    b @L_80007B6C
    @L_80007D98:
    cmpwi r3, 0x5f
    bne @L_80007DE8
    lbz r0, lbl_8047A284(r13)
    li r3, 0x7f
    li r4, -0x1
    li r5, -0x1
    cntlzw r0, r0
    srwi r6, r0, 5
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80007DD4
    li r0, 0x1
    stb r0, lbl_8047A284(r13)
    b @L_80007B6C
    @L_80007DD4:
    cmpwi r0, 0x1
    bne @L_80007B6C
    li r0, 0x0
    stb r0, lbl_8047A284(r13)
    b @L_80007B6C
    @L_80007DE8:
    cmpwi r3, 0x6de
    bne @L_80007E38
    lbz r0, lbl_8047A285(r13)
    li r3, 0x7f
    li r4, -0x1
    li r5, -0x1
    cntlzw r0, r0
    srwi r6, r0, 5
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80007E24
    li r0, 0x1
    stb r0, lbl_8047A285(r13)
    b @L_80007B6C
    @L_80007E24:
    cmpwi r0, 0x1
    bne @L_80007B6C
    li r0, 0x0
    stb r0, lbl_8047A285(r13)
    b @L_80007B6C
    @L_80007E38:
    cmpwi r3, 0x6f9
    bne @L_80007E88
    lbz r0, lbl_8047A286(r13)
    li r3, 0x7f
    li r4, -0x1
    li r5, -0x1
    cntlzw r0, r0
    srwi r6, r0, 5
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80007E74
    li r0, 0x1
    stb r0, lbl_8047A286(r13)
    b @L_80007B6C
    @L_80007E74:
    cmpwi r0, 0x1
    bne @L_80007B6C
    li r0, 0x0
    stb r0, lbl_8047A286(r13)
    b @L_80007B6C
    @L_80007E88:
    cmpwi r3, 0x778
    bne @L_80007ED8
    lbz r0, lbl_8047A280(r13)
    li r3, 0x7f
    li r4, -0x1
    li r5, -0x1
    cntlzw r0, r0
    srwi r6, r0, 5
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80007EC4
    li r0, 0x1
    stb r0, lbl_8047A280(r13)
    b @L_80007B6C
    @L_80007EC4:
    cmpwi r0, 0x1
    bne @L_80007B6C
    li r0, 0x0
    stb r0, lbl_8047A280(r13)
    b @L_80007B6C
    @L_80007ED8:
    cmpwi r3, 0x77a
    bne @L_80007F64
    bl fn_801EF63C
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80007B6C
    lhz r19, lbl_8047A282(r13)
    lis r3, fn_800083FC@ha
    addi r20, r3, fn_800083FC@l
    li r21, 0x162
    @L_80007F00:
    clrlwi r3, r19, 16
    mr r5, r20
    addi r4, r1, 0x8
    bl fn_8001E304
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_80007F24
    li r19, -0x1
    b @L_80007F3C
    @L_80007F24:
    lwz r0, 0x8(r1)
    cmplwi r0, 0x163
    blt @L_80007F34
    stw r21, 0x8(r1)
    @L_80007F34:
    bl fn_8001E200
    lwz r19, 0x8(r1)
    @L_80007F3C:
    cmpwi r19, 0x0
    blt @L_80007B6C
    clrlwi r3, r19, 16
    bl fn_80219FE4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80007F00
    clrlwi r0, r19, 16
    sth r0, lbl_8047A282(r13)
    b @L_80007B6C
    @L_80007F64:
    cmpwi r3, 0x1198
    bne @L_80007B6C
    lbz r0, lbl_8047882E(r13)
    li r3, 0x7f
    li r4, -0x1
    li r5, -0x1
    cntlzw r0, r0
    srwi r6, r0, 5
    bl fn_8001E074
    extsb r0, r3
    cmpwi r0, 0x0
    bne @L_80007FA0
    li r0, 0x1
    stb r0, lbl_8047882E(r13)
    b @L_80007B6C
    @L_80007FA0:
    cmpwi r0, 0x1
    bne @L_80007B6C
    li r0, 0x0
    stb r0, lbl_8047882E(r13)
    b @L_80007B6C
    @L_80007FB4:
    li r3, 0xe
    li r4, 0x0
    li r5, 0x1
    bl fn_80102568
    li r3, 0x1
    @L_80007FC8:
    lmw r19, 0x1c(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
