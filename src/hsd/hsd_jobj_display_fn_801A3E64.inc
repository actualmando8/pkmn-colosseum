    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_801A3E9C
    lis r4, -32698
    lwz r31, 0x0(r3)
    addi r0, r4, 21940
    mr r4, r3
    mr r3, r0
    bl fn_801AA498
    mr r3, r31
    b @L_801A3EA0
@L_801A3E9C:
    li r3, 0x0
@L_801A3EA0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
