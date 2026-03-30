    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_800A13F8
    cmplwi r31, 0x0
    beq @L_80093B2C
    cmplw r31, r3
    bne @L_80093B38
    @L_80093B2C:
    li r4, 0x10
    bl fn_800A257C
    bl fn_800A1990
    @L_80093B38:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
