    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_8013DCF4
    lwz r0, 0xc(r30)
    lwz r31, 0x14(r30)
    cmplwi r0, 0x0
    beq @L_8013DCEC
    bl fn_800B8DF4
    bl fn_800B856C
    lwz r3, 0xc(r30)
    bl fn_800EF5A4
    b @L_8013DCEC
    @L_8013DCD4:
    lhz r30, 0xc(r31)
    lwz r31, 0x10(r31)
    mr r3, r30
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
    @L_8013DCEC:
    cmplwi r31, 0x0
    bne @L_8013DCD4
    @L_8013DCF4:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
