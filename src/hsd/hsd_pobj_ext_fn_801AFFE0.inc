    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    bne @L_801B0024
    b @L_801B002C
    @L_801AFFFC:
    cmplwi r31, 0x0
    beq @L_801B0020
    lwz r0, 0x18(r31)
    cmplwi r0, 0x0
    beq @L_801B0020
    lwz r3, 0x18(r31)
    bl fn_801C25E4
    li r0, 0x0
    stw r0, 0x18(r31)
    @L_801B0020:
    lwz r31, 0x0(r31)
    @L_801B0024:
    cmplwi r31, 0x0
    bne @L_801AFFFC
    @L_801B002C:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
