    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80133E6C
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_80133E40
    li r3, 0x0
    b @L_80133E48
    @L_80133E40:
    mtctr r12
    bctrl
    @L_80133E48:
    cmplwi r3, 0x0
    bne @L_80133E58
    li r3, 0x0
    b @L_80133E5C
    @L_80133E58:
    lwz r3, 0x4(r3)
    @L_80133E5C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
