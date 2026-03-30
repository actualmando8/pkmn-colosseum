    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    beq @L_8013C6FC
    mr r31, r3
    lwz r30, 0x0(r3)
    mr r3, r30
    bl fn_800EC1D4
    cmplwi r30, 0x0
    beq @L_8013C6B0
    mr r3, r30
    li r4, 0x0
    bl fn_800E4014
    @L_8013C6B0:
    bl fn_800B8DF4
    bl fn_800B856C
    mr r3, r30
    mr r4, r31
    bl fn_8013CE58
    lhz r30, 0x1c(r31)
    cmplwi r30, 0x0
    beq @L_8013C6E0
    mr r3, r30
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
    @L_8013C6E0:
    lhz r31, 0x8c(r31)
    cmplwi r31, 0x0
    beq @L_8013C6FC
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    @L_8013C6FC:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
