    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_801EF1E4
    cmplwi r3, 0x0
    beq @L_801EEEF0
    clrlwi r0, r31, 16
    mulli r4, r0, 0x18
    addi r0, r4, 0x4a6
    lhzx r3, r3, r0
    b @L_801EEEF4
@L_801EEEF0:
    li r3, 0x0
@L_801EEEF4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
