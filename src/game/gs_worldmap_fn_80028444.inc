    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    lwz r3, 0x60(r3)
    lwz r3, 0x0(r3)
    lwz r3, 0x8(r3)
    bl fn_800FA280
    mr r4, r3
    li r3, 0x37
    bl fn_80132A38
    li r0, 0xcf
    li r3, 0x0
    stw r0, 0x4c(r31)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
