    nofralloc
    stwu r1, -0x10(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r0, 0x24(r3)
    cmpwi r0, 0x1
    beq @L_801808D4
    li r0, 0x0
    stw r0, 0x20(r31)
    @L_801808D4:
    lwz r3, 0x20(r31)
    lwz r31, 0xc(r1)
    addi r1, r1, 0x10
    blr
