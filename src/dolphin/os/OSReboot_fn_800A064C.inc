    nofralloc
    mflr r0
    lis r3, 0x8040
    stw r0, 0x4(r1)
    stwu r1, -0x18(r1)
    stw r31, 0x14(r1)
    addi r31, r3, -18368
    stw r30, 0x10(r1)
    addi r30, r31, 0x40
    lwz r4, 0x40(r31)
    add r3, r31, r4
    subfic r5, r4, 0x40
    bl fn_800A06AC
    stw r3, 0x4c(r31)
    lwz r0, 0x4c(r31)
    cmpwi r0, 0x0
    beq @L_800A0694
    li r0, 0x40
    stw r0, 0x0(r30)
@L_800A0694:
    lwz r0, 0x1c(r1)
    lwz r31, 0x14(r1)
    lwz r30, 0x10(r1)
    addi r1, r1, 0x18
    mtlr r0
    blr
