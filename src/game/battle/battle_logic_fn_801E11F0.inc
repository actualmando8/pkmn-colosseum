    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, -32698
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    addi r31, r3, 31992
    stw r30, 0x8(r1)
    li r30, 0x0
    stw r30, -21500(r13)
    stb r30, -21504(r13)
    b @L_801E1234
@L_801E121C:
    lwz r3, 0x0(r31)
    cmplwi r3, 0x0
    beq @L_801E122C
    bl fn_800FE6DC
@L_801E122C:
    addi r30, r30, 0x1
    addi r31, r31, 0x4
@L_801E1234:
    lwz r0, -21492(r13)
    cmplw r30, r0
    blt @L_801E121C
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
