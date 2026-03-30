    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lwz r3, -21580(r13)
    lwz r0, 0x0(r3)
    cmpwi r0, 0x0
    beq @L_801D0448
    lwz r0, 0x0(r3)
    cmpwi r0, 0x8
    beq @L_801D03D4
    li r3, 0x1
    bl fn_801069FC
@L_801D03D4:
    lwz r3, -21580(r13)
    lwz r3, 0x64(r3)
    bl fn_800FE714
    lwz r3, -21580(r13)
    lwz r30, 0x54(r3)
    mr r3, r30
    bl fn_800E202C
    mr r31, r3
    bl fn_800E24B0
    mr r3, r31
    bl fn_800E209C
    cmpwi r3, 0x0
    bne @L_801D040C
    li r30, 0x0
@L_801D040C:
    lwz r3, -21580(r13)
    stw r30, 0x54(r3)
    lwz r3, -21580(r13)
    lwz r31, 0x50(r3)
    mr r3, r31
    bl fn_800E202C
    mr r30, r3
    bl fn_800E24B0
    mr r3, r30
    bl fn_800E209C
    cmpwi r3, 0x0
    bne @L_801D0440
    li r31, 0x0
@L_801D0440:
    lwz r3, -21580(r13)
    stw r31, 0x50(r3)
@L_801D0448:
    lwz r3, -21580(r13)
    li r0, 0x0
    stw r0, 0x0(r3)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
