    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r29, r3
    mr r30, r4
    lha r0, 0x6(r30)
    cmpwi r0, 0xd96
    bge @L_8003B018
    cmpwi r0, 0x30b
    beq @L_8003B2C4
    bge @L_8003B13C
    cmpwi r0, 0x308
    beq @L_8003B2C4
    b @L_8003B13C
    @L_8003B018:
    cmpwi r0, 0xd99
    beq @L_8003B2C4
    bge @L_8003B13C
    cmpwi r0, 0xd98
    bge @L_8003B030
    b @L_8003B2C4
    @L_8003B030:
    lis r3, lbl_803A6748@ha
    lwz r4, lbl_8047A4D4(r13)
    addi r3, r3, lbl_803A6748@l
    lwz r0, 0x0(r3)
    slwi r0, r0, 2
    add r3, r4, r0
    lhz r3, 0x2(r3)
    bl fn_801EE8F4
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003B2C4
    lis r3, lbl_803A6748@ha
    li r4, 0x0
    addi r3, r3, lbl_803A6748@l
    addi r26, r3, 0x4c
    mr r3, r26
    bl fn_80109B90
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_8003B2C4
    mr r3, r26
    bl fn_80109934
    mr r27, r3
    cmplwi r27, 0x0
    beq @L_8003B2C4
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    mr r4, r27
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x2
    bl fn_800D67BC
    li r3, 0x0
    li r4, 0x0
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047BAC4(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lha r3, 0x54(r30)
    lha r4, 0x56(r30)
    bl fn_800D61E4
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    li r7, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047BAC8(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    b @L_8003B2C4
    @L_8003B13C:
    lis r3, lbl_803A6748@ha
    lwz r4, lbl_8047A4D4(r13)
    addi r3, r3, lbl_803A6748@l
    lwz r3, 0x0(r3)
    addi r4, r4, 0x2
    clrlwi r0, r3, 16
    slwi r3, r3, 2
    slwi r0, r0, 2
    lhzx r31, r4, r3
    lhzx r3, r4, r0
    bl fn_801EED88
    lis r3, lbl_803A6748@ha
    lis r5, lbl_802EF0A8@ha
    addi r3, r3, lbl_803A6748@l
    lwz r4, lbl_8047A4D4(r13)
    lwz r0, 0x0(r3)
    addi r27, r5, lbl_802EF0A8@l
    clrlwi r0, r0, 16
    slwi r0, r0, 2
    add r3, r4, r0
    lhz r26, 0x2(r3)
    mr r3, r26
    bl fn_801EE614
    mr r3, r26
    bl fn_801EE8F4
    mr r3, r26
    bl fn_801EEAD0
    mr r28, r3
    mr r3, r26
    bl fn_801EEC74
    clrlwi r0, r28, 24
    cmplwi r0, 0x0
    beq @L_8003B1DC
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003B1D4
    li r0, 0x1
    b @L_8003B1E0
    @L_8003B1D4:
    li r0, 0x0
    b @L_8003B1E0
    @L_8003B1DC:
    li r0, 0x2
    @L_8003B1E0:
    cmpwi r0, 0x1
    beq @L_8003B23C
    bge @L_8003B1F8
    cmpwi r0, 0x0
    bge @L_8003B200
    b @L_8003B26C
    @L_8003B1F8:
    cmpwi r0, 0x3
    b @L_8003B26C
    @L_8003B200:
    lha r8, 0x50(r30)
    mr r5, r29
    lha r4, 0x55fa(r27)
    li r6, 0x161
    lha r3, 0x52(r30)
    li r7, 0x0
    lha r0, 0x55fc(r27)
    subf r4, r8, r4
    subi r4, r4, 0x7
    subf r3, r3, r0
    subi r0, r3, 0x4
    extsh r3, r4
    extsh r4, r0
    bl fn_801040F0
    b @L_8003B26C
    @L_8003B23C:
    lha r8, 0x50(r30)
    mr r5, r29
    lha r3, 0x55fa(r27)
    li r6, 0x160
    lha r4, 0x52(r30)
    li r7, 0x0
    lha r0, 0x55fc(r27)
    subf r3, r8, r3
    extsh r3, r3
    subf r0, r4, r0
    extsh r4, r0
    bl fn_801040F0
    @L_8003B26C:
    mr r3, r31
    bl fn_801EE8F4
    mr r30, r3
    mr r3, r31
    bl fn_801EEC74
    clrlwi r0, r30, 24
    cmplwi r0, 0x0
    beq @L_8003B2A8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8003B2A0
    li r6, 0x36e7
    b @L_8003B2AC
    @L_8003B2A0:
    li r6, 0x36e6
    b @L_8003B2AC
    @L_8003B2A8:
    li r6, 0x36e8
    @L_8003B2AC:
    lbz r4, 0x8b(r29)
    li r0, -0x100
    li r3, 0x0
    or r5, r4, r0
    li r4, 0x0
    bl fn_800FB680
    @L_8003B2C4:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
