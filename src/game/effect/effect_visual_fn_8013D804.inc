    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_8013D8DC
    lwz r0, lbl_8047AEE0(r13)
    cmplwi r0, 0x0
    bne @L_8013D8C8
    lis r3, lbl_80466BC0@ha
    li r0, 0x0
    addi r4, r3, lbl_80466BC0@l
    sth r0, lbl_8047AEE4(r13)
    lhz r3, 0x4(r4)
    li r5, 0x44
    lhz r4, 0x6(r4)
    li r6, 0x0
    li r7, 0x0
    bl fn_800EF5FC
    cmplwi r3, 0x0
    stw r3, lbl_8047AEE0(r13)
    bne @L_8013D8C8
    cmplwi r30, 0x0
    beq @L_8013D8C0
    cmplwi r3, 0x0
    lwz r30, 0xc(r30)
    beq @L_8013D8B8
    lhz r3, lbl_8047AEE4(r13)
    subi r3, r3, 0x1
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AEE4(r13)
    bne @L_8013D8B8
    bl fn_800B8DF4
    lwz r3, lbl_8047AEE0(r13)
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, lbl_8047AEE0(r13)
    b @L_8013D8B8
    @L_8013D8A0:
    lhz r31, 0xc(r30)
    lwz r30, 0x10(r30)
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    @L_8013D8B8:
    cmplwi r30, 0x0
    bne @L_8013D8A0
    @L_8013D8C0:
    li r3, 0x0
    b @L_8013D8F0
    @L_8013D8C8:
    lhz r4, lbl_8047AEE4(r13)
    li r3, 0x1
    addi r0, r4, 0x1
    sth r0, lbl_8047AEE4(r13)
    b @L_8013D8F0
    @L_8013D8DC:
    lis r3, lbl_80272F00@ha
    addi r3, r3, lbl_80272F00@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013D8F0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
