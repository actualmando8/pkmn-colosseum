    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr. r30, r3
    beq @L_80137A88
    bl fn_800B8DF4
    bl fn_800B856C
    lhz r31, 0xc(r30)
    cmplwi r31, 0x0
    beq @L_80137A6C
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
@L_80137A6C:
    lhz r31, 0xe(r30)
    cmplwi r31, 0x0
    beq @L_80137A88
    mr r3, r31
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
@L_80137A88:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
