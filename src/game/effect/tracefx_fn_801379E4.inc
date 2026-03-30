    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_80137A14
    bl fn_800B8DF4
    bl fn_800B856C
    lwz r3, 0x14(r31)
    cmplwi r3, 0x0
    beq @L_80137A14
    bl fn_800EF5A4
@L_80137A14:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
