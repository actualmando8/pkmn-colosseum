    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r4, 0x14
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    beq @L_801A69F4
    lis r3, 0x8027
    lis r5, 0x8027
    addi r3, r3, 19984
    li r4, 0x14
    addi r5, r5, 19996
    bl fn_80196E10
@L_801A69F4:
    lwz r3, 0x0(r31)
    lis r4, 0x8046
    lwz r0, 0x4(r31)
    stwu r3, 22024(r4)
    stw r0, 0x4(r4)
    lwz r3, 0x8(r31)
    lwz r0, 0xc(r31)
    stw r3, 0x8(r4)
    stw r0, 0xc(r4)
    lwz r0, 0x10(r31)
    stw r0, 0x10(r4)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
