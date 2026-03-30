    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_80138BA4
    lhz r3, 0x40(r31)
    lhz r4, 0x44(r31)
    bl fn_800F9210
    lhz r3, 0x40(r31)
    lhz r4, 0x42(r31)
    bl fn_800F9210
    @L_80138BA4:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
