    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r4, -32697
    stw r0, 0x14(r1)
    addi r4, r4, -21408
    lwz r0, 0xa0(r4)
    cmpwi r0, 0x0
    beq @L_801E3964
    addi r4, r4, 0x80
    li r5, 0xc
    bl memcpy
    li r3, 0x1
    b @L_801E3968
@L_801E3964:
    li r3, 0x0
@L_801E3968:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
