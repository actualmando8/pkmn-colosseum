    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    mr r26, r3
    li r4, 0x0
    bl fn_801040D0
    cmplwi r3, 0x0
    bne @L_8001AF70
    li r3, 0x0
    b @L_8001B170
    @L_8001AF70:
    lbz r0, 0x1(r26)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8001B0A4
    li r0, -0x1
    stb r0, 0x97(r26)
    lbz r0, 0x2(r26)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8001B13C
    lis r4, lbl_803A1D40@ha
    mr r28, r3
    addi r30, r4, lbl_803A1D40@l
    li r27, 0x0
    li r29, 0x0
    lis r3, lbl_802E4E58@ha
    addi r31, r3, lbl_802E4E58@l
    @L_8001AFB4:
    lbz r0, 0x4(r30)
    extsb r0, r0
    mulli r0, r0, 0x30
    add r0, r31, r0
    add r5, r0, r29
    lha r3, 0x2(r5)
    lha r4, 0x4(r5)
    lha r5, 0x6(r5)
    bl fn_8005D880
    lbz r0, 0x4(r30)
    extsb r0, r0
    mulli r0, r0, 0x30
    add r0, r31, r0
    add r5, r0, r29
    lha r3, 0x0(r5)
    lha r4, 0x4(r5)
    lha r5, 0x6(r5)
    bl fn_8005D9AC
    lbz r0, 0x4(r30)
    mr r9, r28
    mr r10, r27
    li r4, 0x63
    extsb r0, r0
    li r5, 0x0
    mulli r0, r0, 0x30
    li r6, 0x0
    li r7, 0x0
    li r8, 0x2
    add r3, r31, r0
    lhax r3, r3, r29
    crxor 6, 6, 6
    bl fn_801026A4
    lbz r0, 0x4(r30)
    addi r4, r1, 0xc
    addi r5, r1, 0xe
    extsb r0, r0
    mulli r0, r0, 0x30
    add r3, r31, r0
    lhax r25, r3, r29
    mr r3, r25
    bl fn_8005D95C
    lha r0, 0xc(r1)
    mr r3, r25
    cmpwi r0, 0xfa
    ble @L_8001B070
    li r4, 0x116
    b @L_8001B074
    @L_8001B070:
    li r4, 0x11e
    @L_8001B074:
    bl fn_801080CC
    addi r29, r29, 0x8
    addi r28, r28, 0x30
    addi r27, r27, 0x1
    cmpwi r27, 0x6
    blt @L_8001AFB4
    lwz r3, 0x4(r26)
    li r4, 0x1
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r26)
    b @L_8001B13C
    @L_8001B0A4:
    cmpwi r0, 0x3
    bne @L_8001B13C
    lbz r0, 0x2(r26)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_8001B13C
    li r30, 0x0
    lis r3, lbl_803A1D40@ha
    mr r29, r30
    addi r28, r3, lbl_803A1D40@l
    lis r3, lbl_802E4E58@ha
    addi r27, r3, lbl_802E4E58@l
    @L_8001B0D4:
    lbz r0, 0x4(r28)
    addi r4, r1, 0x8
    addi r5, r1, 0xa
    extsb r0, r0
    mulli r0, r0, 0x30
    add r3, r27, r0
    lhax r25, r3, r29
    mr r3, r25
    bl fn_8005D95C
    lha r0, 0x8(r1)
    mr r3, r25
    cmpwi r0, 0xfa
    ble @L_8001B110
    li r4, 0x11a
    b @L_8001B114
    @L_8001B110:
    li r4, 0x122
    @L_8001B114:
    bl fn_801080CC
    addi r29, r29, 0x8
    addi r30, r30, 0x1
    cmpwi r30, 0x6
    blt @L_8001B0D4
    lwz r3, 0x4(r26)
    li r4, 0x7
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r26)
    @L_8001B13C:
    lha r4, lbl_8047A308(r13)
    lis r3, 0x1062
    addi r0, r3, 0x4dd3
    li r3, 0x0
    addi r5, r4, 0x1
    mulhw r0, r0, r5
    srawi r0, r0, 6
    srwi r4, r0, 31
    add r0, r0, r4
    mulli r0, r0, 0x3e8
    subf r0, r0, r5
    extsh r0, r0
    sth r0, lbl_8047A308(r13)
    @L_8001B170:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
