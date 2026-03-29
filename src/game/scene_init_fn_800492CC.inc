    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    stw r28, 0x10(r1)
    mr r29, r4
    lwz r0, lbl_804788C0(r13)
    li r31, 0x0
    cmpwi r0, 0x0
    beq @L_800495A8
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lwz r5, 0x1c(r3)
    cmpwi r5, 0xc
    beq @L_800495A8
    cmpwi r5, 0x4
    beq @L_80049358
    bge @L_80049330
    cmpwi r5, 0x3
    bge @L_80049384
    cmpwi r5, 0x1
    bge @L_8004933C
    b @L_80049384
    @L_80049330:
    cmpwi r5, 0x6
    bge @L_80049384
    b @L_8004936C
    @L_8004933C:
    la r3, lbl_804788CC(r13)
    lha r4, lbl_804788CC(r13)
    lha r0, 0x2(r3)
    la r3, lbl_804788C8(r13)
    sth r4, lbl_804788C8(r13)
    sth r0, 0x2(r3)
    b @L_80049384
    @L_80049358:
    li r0, 0x0
    la r3, lbl_804788C8(r13)
    sth r0, lbl_804788C8(r13)
    sth r0, 0x2(r3)
    b @L_80049384
    @L_8004936C:
    la r3, lbl_804788D0(r13)
    lha r4, lbl_804788D0(r13)
    lha r0, 0x2(r3)
    la r3, lbl_804788C8(r13)
    sth r4, lbl_804788C8(r13)
    sth r0, 0x2(r3)
    @L_80049384:
    cmpwi r5, 0x5
    bne @L_800493D0
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lbz r0, 0x214(r3)
    cmplwi r0, 0x0
    beq @L_800493C4
    addi r3, r3, 0x7c
    bl fn_80109934
    lis r4, lbl_803A6818@ha
    mr r28, r3
    addi r4, r4, lbl_803A6818@l
    addi r3, r4, 0xc4
    bl fn_80109934
    mr r31, r3
    b @L_800493F8
    @L_800493C4:
    li r28, 0x0
    li r31, 0x0
    b @L_800493F8
    @L_800493D0:
    lis r3, lbl_803A6818@ha
    addi r3, r3, lbl_803A6818@l
    lbz r0, 0x214(r3)
    cmplwi r0, 0x0
    beq @L_800493F4
    addi r3, r3, 0x7c
    bl fn_80109934
    mr r28, r3
    b @L_800493F8
    @L_800493F4:
    li r28, 0x0
    @L_800493F8:
    lis r3, lbl_803A6818@ha
    lfs f1, lbl_8047BCA0(r2)
    addi r3, r3, lbl_803A6818@l
    cmplwi r28, 0x0
    lfs f0, 0x5c(r3)
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r30, 0xc(r1)
    beq @L_800494E0
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    mr r4, r28
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x2
    bl fn_800D67BC
    la r28, lbl_804788C8(r13)
    lha r3, lbl_804788C8(r13)
    lha r4, 0x2(r28)
    bl fn_800D61E4
    mr r7, r30
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047BC94(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lha r5, lbl_804788C8(r13)
    lha r3, 0x54(r29)
    lha r4, 0x2(r28)
    lha r0, 0x56(r29)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800D61E4
    mr r7, r30
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047BCBC(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    @L_800494E0:
    cmplwi r31, 0x0
    beq @L_800495A8
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    li r3, 0x7
    bl fn_800D6A00
    lis r3, lbl_80314F98@ha
    addi r3, r3, lbl_80314F98@l
    bl fn_800D7820
    mr r4, r31
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x2
    bl fn_800D67BC
    la r28, lbl_804788C8(r13)
    lha r3, lbl_804788C8(r13)
    lha r4, 0x2(r28)
    bl fn_800D61E4
    mr r7, r30
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047BC94(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lha r5, lbl_804788C8(r13)
    lha r3, 0x54(r29)
    lha r4, 0x2(r28)
    lha r0, 0x56(r29)
    add r3, r5, r3
    extsh r3, r3
    add r0, r4, r0
    extsh r4, r0
    bl fn_800D61E4
    mr r7, r30
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5CB8
    lfs f1, lbl_8047BCBC(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    @L_800495A8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    lwz r28, 0x10(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
