    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_8013E594
    mr r31, r3
    lwz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_8013E584
    bl fn_800B8DF4
    bl fn_800B856C
    lwz r3, 0x4(r31)
    bl fn_800EF5A4
    @L_8013E584:
    mr r3, r31
    li r4, 0x0
    li r5, 0x34
    bl memset
    @L_8013E594:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
