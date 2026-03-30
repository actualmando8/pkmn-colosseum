    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    li r3, 0xa6
    bl fn_80104704
    bl fn_801040A0
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_8007C7D8
    stb r31, 0xc8(r3)
@L_8007C7D8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
