    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lwz r3, 0x60(r3)
    lwz r3, 0x0(r3)
    lwz r3, 0x0(r3)
    bl fn_8004BE40
    bl fn_801D1A88
    cmplwi r3, 0x0
    beq @L_8004D740
    bl fn_800FA280
    mr r0, r3
    li r3, 0x37
    mr r4, r0
    bl fn_80132A38
    li r0, 0xe7
    stw r0, 0x4c(r31)
    b @L_8004D748
    @L_8004D740:
    li r0, 0x0
    stw r0, 0x4c(r31)
    @L_8004D748:
    li r3, 0x0
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
