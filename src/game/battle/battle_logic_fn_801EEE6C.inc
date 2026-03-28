    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r3
    mr r31, r4
    li r3, 0x0
    bl fn_801EF1E4
    cmplwi r3, 0x0
    beq @L_801EEEA4
    clrlwi r0, r30, 16
    mulli r4, r0, 0x18
    addi r0, r4, 0x4a6
    sthx r31, r3, r0
@L_801EEEA4:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
