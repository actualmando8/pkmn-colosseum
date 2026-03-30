    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r5, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    beq @L_8020C130
    lbz r4, 0x0(r5)
    bl fn_8022E6F0
    b @L_8020C144
@L_8020C130:
    li r4, 0x0
    bl fn_8022E6F0
    mr r3, r31
    li r4, 0x1
    bl fn_8022E6F0
@L_8020C144:
    lwz r0, 0x14(r1)
    li r3, 0x1
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
