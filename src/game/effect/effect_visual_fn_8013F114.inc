    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8013F32C
    lwz r0, 0x4(r31)
    cmplwi r0, 0x0
    bne @L_8013F140
    li r3, 0x0
    b @L_8013F330
    @L_8013F140:
    lwz r0, lbl_8047AEE8(r13)
    cmplwi r0, 0x0
    bne @L_8013F1FC
    lis r3, lbl_80466BC0@ha
    li r0, 0x0
    addi r4, r3, lbl_80466BC0@l
    sth r0, lbl_8047AEEC(r13)
    lhz r3, 0x4(r4)
    li r5, 0x45
    lhz r4, 0x6(r4)
    li r6, 0x0
    li r7, 0x0
    bl fn_800EF5FC
    cmplwi r3, 0x0
    stw r3, lbl_8047AEE8(r13)
    bne @L_8013F1FC
    cmplwi r31, 0x0
    beq @L_8013F1F4
    bl fn_800B8DF4
    bl fn_800B856C
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_8013F1A8
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, 0x4(r31)
    @L_8013F1A8:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_8013F1C0
    bl fn_800D75F4
    li r0, 0x0
    stw r0, 0x0(r31)
    @L_8013F1C0:
    lwz r0, lbl_8047AEE8(r13)
    cmplwi r0, 0x0
    beq @L_8013F1F4
    lhz r3, lbl_8047AEEC(r13)
    subi r3, r3, 0x1
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AEEC(r13)
    bne @L_8013F1F4
    bl fn_800B8DF4
    lwz r3, lbl_8047AEE8(r13)
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, lbl_8047AEE8(r13)
    @L_8013F1F4:
    li r3, 0x0
    b @L_8013F330
    @L_8013F1FC:
    lhz r3, lbl_8047AEEC(r13)
    addi r0, r3, 0x1
    sth r0, lbl_8047AEEC(r13)
    bl fn_800D7894
    stw r3, 0x0(r31)
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    bne @L_8013F298
    cmplwi r31, 0x0
    beq @L_8013F290
    bl fn_800B8DF4
    bl fn_800B856C
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_8013F244
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, 0x4(r31)
    @L_8013F244:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_8013F25C
    bl fn_800D75F4
    li r0, 0x0
    stw r0, 0x0(r31)
    @L_8013F25C:
    lwz r0, lbl_8047AEE8(r13)
    cmplwi r0, 0x0
    beq @L_8013F290
    lhz r3, lbl_8047AEEC(r13)
    subi r3, r3, 0x1
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AEEC(r13)
    bne @L_8013F290
    bl fn_800B8DF4
    lwz r3, lbl_8047AEE8(r13)
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, lbl_8047AEE8(r13)
    @L_8013F290:
    li r3, 0x0
    b @L_8013F330
    @L_8013F298:
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, 0x0(r31)
    li r4, 0x4
    li r5, 0x0
    li r6, 0x5
    li r7, 0x6
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, 0x0(r31)
    li r4, 0x6
    li r5, 0x0
    li r6, 0x8
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, 0x0(r31)
    li r4, 0x7
    li r5, 0x0
    li r6, 0x8
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    li r3, 0x1
    b @L_8013F330
    @L_8013F32C:
    li r3, 0x0
    @L_8013F330:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
