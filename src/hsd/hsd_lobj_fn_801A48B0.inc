    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    beq @L_801A48E0
    lwz r0, 0x1c(r3)
    cmplwi r0, 0x0
    beq @L_801A48E0
    lwz r3, 0x1c(r3)
    bl fn_80191688
    li r3, 0x1
    b @L_801A48E4
    @L_801A48E0:
    li r3, 0x0
    @L_801A48E4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
