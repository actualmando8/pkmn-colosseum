    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_8013F3E4
    lwz r0, 0x4(r30)
    cmplwi r0, 0x0
    beq @L_8013F378
    lwz r0, lbl_8047AEE8(r13)
    cmplwi r0, 0x0
    bne @L_8013F380
    @L_8013F378:
    li r3, 0x0
    b @L_8013F3F8
    @L_8013F380:
    lwz r3, 0x8(r30)
    cmplwi r3, 0x0
    bne @L_8013F394
    li r3, 0x0
    b @L_8013F3F8
    @L_8013F394:
    lwz r4, 0xc(r30)
    bl fn_800EE150
    mr. r31, r3
    beq @L_8013F3C0
    addi r4, r30, 0x2c
    li r5, 0x0
    li r6, 0x0
    bl fn_800EE3BC
    mr r3, r31
    bl fn_800EE828
    b @L_8013F3CC
    @L_8013F3C0:
    lwz r3, 0x8(r30)
    addi r4, r30, 0x2c
    bl fn_800E3D98
    @L_8013F3CC:
    li r0, 0x0
    lfs f0, lbl_8047D300(r2)
    sth r0, 0xb0(r30)
    li r3, 0x1
    stfs f0, 0x10(r30)
    b @L_8013F3F8
    @L_8013F3E4:
    lis r3, lbl_80272FE0@ha
    addi r3, r3, lbl_80272FE0@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013F3F8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
