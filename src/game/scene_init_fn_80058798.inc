    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    bl fn_80105624
    lhz r0, 0x4(r3)
    cmplwi r0, 0x0
    beq @L_800587C4
    li r0, 0x1
    stb r0, 0x98(r31)
    @L_800587C4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
