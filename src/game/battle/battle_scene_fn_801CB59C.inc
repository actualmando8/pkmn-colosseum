    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    bl fn_80113F48
    mr r4, r30
    mr r31, r3
    bl fn_8018D998
    cmplwi r3, 0x0
    beq @L_801CB5DC
    mr r3, r31
    mr r4, r30
    bl fn_80184470
    b @L_801CB600
@L_801CB5DC:
    mr r3, r31
    mr r4, r30
    bl fn_800F9318
    cmplwi r3, 0x0
    bne @L_801CB5F8
    li r3, 0x0
    b @L_801CB604
@L_801CB5F8:
    li r4, 0x1
    bl fn_800E465C
@L_801CB600:
    li r3, 0x1
@L_801CB604:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
