    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    cmpwi r31, 0x0
    bne @L_80102534
    bl fn_801046B8
    mr r31, r3
    @L_80102534:
    mr r3, r31
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80102554
    li r4, 0x0
    bl fn_80104828
    mr r3, r31
    bl fn_80104704
    @L_80102554:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
