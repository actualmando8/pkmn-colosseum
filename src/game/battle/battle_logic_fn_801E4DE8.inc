    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, -21408(r13)
    cmpwi r0, 0x0
    beq @L_801E4E0C
    lis r3, -32697
    addi r3, r3, -16776
    bl fn_800A1F94
@L_801E4E0C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
