    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32704
    stw r0, 0x14(r1)
    li r0, 0x0
    stw r31, 0xc(r1)
    addi r31, r3, 19560
    lwz r3, 0x370c(r31)
    stw r0, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_8010CC3C
    bl fn_800DACC0
    li r0, 0x0
    stw r0, 0x370c(r31)
@L_8010CC3C:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
