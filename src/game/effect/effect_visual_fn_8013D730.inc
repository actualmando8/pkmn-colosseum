    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    beq @L_8013D7B0
    lwz r0, lbl_8047AEE0(r13)
    lwz r31, 0xc(r3)
    cmplwi r0, 0x0
    beq @L_8013D7A0
    lhz r3, lbl_8047AEE4(r13)
    subi r3, r3, 0x1
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AEE4(r13)
    bne @L_8013D7A0
    bl fn_800B8DF4
    lwz r3, lbl_8047AEE0(r13)
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, lbl_8047AEE0(r13)
    b @L_8013D7A0
    @L_8013D788:
    lhz r30, 0xc(r31)
    lwz r31, 0x10(r31)
    mr r3, r30
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
    @L_8013D7A0:
    cmplwi r31, 0x0
    bne @L_8013D788
    li r3, 0x1
    b @L_8013D7B4
    @L_8013D7B0:
    li r3, 0x0
    @L_8013D7B4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
