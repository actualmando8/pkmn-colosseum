    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8013A4F8
    lhz r3, 0xc(r31)
    lhz r4, 0xe(r31)
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_8013A4E8
    lis r3, lbl_80272D90@ha
    lhz r4, 0xc(r31)
    lhz r5, 0xe(r31)
    addi r3, r3, lbl_80272D90@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    b @L_8013A50C
    @L_8013A4E8:
    li r0, 0x0
    li r3, 0x1
    sth r0, 0x8(r31)
    b @L_8013A50C
    @L_8013A4F8:
    lis r3, lbl_80272DF4@ha
    addi r3, r3, lbl_80272DF4@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013A50C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
