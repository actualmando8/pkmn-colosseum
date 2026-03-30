    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_8013C658
    bl fn_800B8DF4
    bl fn_800B856C
    lwz r0, 0x7c(r31)
    cmpwi r0, 0x0
    beq @L_8013C658
    lwz r3, 0x74(r31)
    lwz r4, 0x78(r31)
    bl fn_800F9210
    lwz r3, 0x74(r31)
    lwz r4, 0x7c(r31)
    bl fn_800F9210
    @L_8013C658:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
