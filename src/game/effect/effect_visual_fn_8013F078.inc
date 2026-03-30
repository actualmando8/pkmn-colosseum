    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8013F0FC
    bl fn_800B8DF4
    bl fn_800B856C
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_8013F0B0
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, 0x4(r31)
    @L_8013F0B0:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_8013F0C8
    bl fn_800D75F4
    li r0, 0x0
    stw r0, 0x0(r31)
    @L_8013F0C8:
    lwz r0, lbl_8047AEE8(r13)
    cmplwi r0, 0x0
    beq @L_8013F0FC
    lhz r3, lbl_8047AEEC(r13)
    subi r3, r3, 0x1
    clrlwi. r0, r3, 16
    sth r3, lbl_8047AEEC(r13)
    bne @L_8013F0FC
    bl fn_800B8DF4
    lwz r3, lbl_8047AEE8(r13)
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, lbl_8047AEE8(r13)
    @L_8013F0FC:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
