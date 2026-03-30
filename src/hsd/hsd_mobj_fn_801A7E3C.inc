    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_801A7E70
    lwz r5, 0x0(r31)
    mr r4, r31
    lwz r3, 0x14(r31)
    lwz r5, 0x4c(r5)
    bl fn_801C27F4
    lwz r3, 0x8(r31)
    bl fn_801BE800
    @L_801A7E70:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
