    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r3, lbl_80478C94(r13)
    bl fn_801A6990
    cmpwi r3, 0x0
    beq @L_801ADC2C
    la r0, lbl_80478C90(r13)
    stw r0, lbl_80478C94(r13)
@L_801ADC2C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
