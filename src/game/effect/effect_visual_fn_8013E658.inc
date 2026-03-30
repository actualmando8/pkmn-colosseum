    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_8013E6A0
    li r0, 0x0
    sth r0, 0x30(r3)
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_8013E6A0
    lbz r0, 0x18(r3)
    cmplwi r0, 0x0
    beq @L_8013E6A0
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_8013E6A0
    li r3, 0x1
    b @L_8013E6B4
    @L_8013E6A0:
    lis r3, lbl_80272FA0@ha
    addi r3, r3, lbl_80272FA0@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
    @L_8013E6B4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
