    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_800E202C
    mr r31, r3
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    cmpwi r3, 0x0
    bne @L_801D0350
    li r3, 0x0
    b @L_801D0354
@L_801D0350:
    mr r3, r30
@L_801D0354:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
