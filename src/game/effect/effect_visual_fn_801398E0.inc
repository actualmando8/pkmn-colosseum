    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_8013991C
    lhz r31, 0x4(r3)
    cmplwi r31, 0x0
    beq @L_8013991C
    bl fn_800B8DF4
    bl fn_800B856C
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    @L_8013991C:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
