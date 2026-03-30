    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    li r28, 0x0
    li r29, 0x0
    li r30, 0x0
    b @L_801D1EC4
@L_801D1E74:
    li r3, 0x0
    li r4, 0xa
    bl fn_80129280
    cmpwi r29, 0x0
    mr r31, r3
    blt @L_801D1EA4
    li r3, 0x0
    li r4, 0xa
    bl fn_80129280
    lhz r0, 0x400(r3)
    cmpw r29, r0
    blt @L_801D1EAC
@L_801D1EA4:
    li r0, -0x1
    b @L_801D1EB0
@L_801D1EAC:
    lhzx r0, r31, r30
@L_801D1EB0:
    cmpw r27, r0
    bne @L_801D1EBC
    li r28, 0x1
@L_801D1EBC:
    addi r30, r30, 0x2
    addi r29, r29, 0x1
@L_801D1EC4:
    li r3, 0x0
    li r4, 0xa
    bl fn_80129280
    lhz r0, 0x400(r3)
    cmpw r29, r0
    bge @L_801D1EE4
    cmpwi r28, 0x0
    beq @L_801D1E74
@L_801D1EE4:
    cmpwi r28, 0x0
    bne @L_801D1EF4
    li r3, -0x1
    b @L_801D1EF8
@L_801D1EF4:
    mr r3, r29
@L_801D1EF8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
