    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lhz r3, 0x164(r3)
    cmplwi r3, 0x0
    beq @L_800E92C4
    bl fn_800E24B0
    lhz r3, 0x164(r31)
    bl fn_800E209C
    li r0, 0x0
    sth r0, 0x164(r31)
    stw r0, 0x158(r31)
    stw r0, 0x15c(r31)
    @L_800E92C4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
