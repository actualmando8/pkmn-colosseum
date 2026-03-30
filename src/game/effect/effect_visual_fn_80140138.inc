    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_80140164
    bl fn_800DF608
    li r0, 0x0
    stw r0, 0x0(r31)
    @L_80140164:
    lwz r3, 0x4(r31)
    cmplwi r3, 0x0
    beq @L_8014017C
    bl fn_800EF5A4
    li r0, 0x0
    stw r0, 0x4(r31)
    @L_8014017C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
