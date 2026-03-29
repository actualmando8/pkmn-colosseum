    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    lwz r0, lbl_804788E0(r13)
    cmpwi r0, 0x0
    beq @L_8004D394
    lis r4, lbl_802EF0A8@ha
    li r0, 0x0
    addi r5, r4, lbl_802EF0A8@l
    stw r0, lbl_804788E0(r13)
    lha r4, 0x7772(r5)
    lha r0, 0x7756(r5)
    stw r4, lbl_8047A50C(r13)
    stw r0, lbl_8047A508(r13)
    @L_8004D394:
    lis r4, 0x6666
    addi r0, r4, 0x6667
    mulhw r0, r0, r3
    srawi r5, r0, 2
    srawi r0, r0, 2
    srwi r4, r0, 31
    srwi r6, r5, 31
    add r0, r0, r4
    mulli r0, r0, 0xa
    add r31, r5, r6
    subf r30, r0, r3
    bl fn_801D1F7C
    mulli r0, r31, 0xa
    subf r0, r0, r3
    cmpwi r0, 0xa
    ble @L_8004D3DC
    li r0, 0xa
    b @L_8004D3E8
    @L_8004D3DC:
    cmpwi r0, 0x0
    bge @L_8004D3E8
    li r0, 0x0
    @L_8004D3E8:
    cmpw r30, r0
    blt @L_8004D3F4
    li r30, 0xa
    @L_8004D3F4:
    extsb r3, r31
    extsb r0, r30
    stb r3, 0x14(r1)
    addi r4, r1, 0x10
    li r3, 0xa
    stb r0, 0x15(r1)
    lhz r0, 0x14(r1)
    sth r0, 0x10(r1)
    bl fn_80103EAC
    lfs f31, lbl_8047BE20(r2)
    @L_8004D41C:
    lwz r3, lbl_8047A50C(r13)
    la r4, lbl_8047A510(r13)
    lwz r0, lbl_8047A508(r13)
    stfs f31, lbl_8047A510(r13)
    stw r4, 0x18(r1)
    stw r3, 0x1c(r1)
    stw r0, 0x20(r1)
    bl fn_801046B8
    mr r4, r3
    addi r9, r1, 0x18
    li r3, 0x73
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
    mr r31, r3
    li r3, 0xa
    bl fn_80103E68
    srwi r0, r3, 16
    cmpwi r31, -0x1
    sth r0, 0x8(r1)
    sth r0, 0x14(r1)
    beq @L_8004D534
    lbz r0, 0x15(r1)
    extsb r0, r0
    cmpwi r0, 0xa
    blt @L_8004D534
    cmpwi r0, 0xb
    beq @L_8004D518
    bge @L_8004D41C
    cmpwi r0, 0xa
    bge @L_8004D4A8
    b @L_8004D41C
    @L_8004D4A8:
    bl fn_801D1B4C
    bl fn_8004DC18
    cmpwi r3, 0x0
    blt @L_8004D41C
    bl fn_801D1B10
    bl fn_8004BFB0
    li r31, 0x0
    bl fn_801D1F7C
    cmpwi r3, 0xa
    ble @L_8004D4D8
    li r3, 0xa
    b @L_8004D4E4
    @L_8004D4D8:
    cmpwi r3, 0x0
    bge @L_8004D4E4
    li r3, 0x0
    @L_8004D4E4:
    cmpwi r3, 0x0
    bgt @L_8004D4F0
    li r31, 0xa
    @L_8004D4F0:
    extsb r0, r31
    li r3, 0x0
    stb r3, 0x14(r1)
    addi r4, r1, 0xc
    li r3, 0xa
    stb r0, 0x15(r1)
    lhz r0, 0x14(r1)
    sth r0, 0xc(r1)
    bl fn_80103EAC
    b @L_8004D41C
    @L_8004D518:
    bl fn_801D16C4
    bl fn_8004DFCC
    clrlwi r0, r3, 24
    cmplwi r0, 0xff
    beq @L_8004D41C
    bl fn_801D167C
    b @L_8004D41C
    @L_8004D534:
    li r3, 0x73
    bl fn_80102510
    li r3, 0x73
    li r4, 0x1
    bl fn_80102428
    cmpwi r31, -0x1
    bne @L_8004D558
    li r3, -0x1
    b @L_8004D570
    @L_8004D558:
    lbz r0, 0x14(r1)
    lbz r3, 0x15(r1)
    extsb r0, r0
    mulli r0, r0, 0xa
    extsb r3, r3
    add r3, r3, r0
    @L_8004D570:
    psq_l f31, 0x38(r1), 0, 0
    lwz r0, 0x44(r1)
    lfd f31, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
