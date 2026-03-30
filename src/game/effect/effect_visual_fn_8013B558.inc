    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_8013B5C8
    lwz r3, 0x0(r30)
    cmplwi r3, 0x0
    beq @L_8013B588
    li r4, 0x0
    bl fn_800E4014
    @L_8013B588:
    bl fn_800B8DF4
    bl fn_800B856C
    lhz r31, 0x20(r30)
    cmplwi r31, 0x0
    beq @L_8013B5AC
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    @L_8013B5AC:
    lhz r31, 0xc0(r30)
    cmplwi r31, 0x0
    beq @L_8013B5C8
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    @L_8013B5C8:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
