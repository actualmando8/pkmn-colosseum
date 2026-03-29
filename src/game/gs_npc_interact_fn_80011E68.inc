    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80011E90
    bl fn_80103FE4
    sth r31, 0x1a(r3)
    @L_80011E90:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
