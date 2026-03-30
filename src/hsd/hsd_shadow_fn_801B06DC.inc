    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lwz r3, 0x8(r3)
    lwz r30, 0x58(r3)
    lwz r0, 0x0(r30)
    cmplwi r0, 0x0
    beq @L_801B0724
    lwz r3, 0x0(r30)
    bl fn_800E202C
    mr r31, r3
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    li r0, 0x0
    stw r0, 0x0(r30)
@L_801B0724:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
