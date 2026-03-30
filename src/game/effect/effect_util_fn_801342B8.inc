    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_801342D8
    li r3, 0x0
    b @L_801342E0
    @L_801342D8:
    mtctr r12
    bctrl
    @L_801342E0:
    cmplwi r3, 0x0
    bne @L_801342F0
    li r3, 0x0
    b @L_801342F4
    @L_801342F0:
    lha r3, 0x2(r3)
    @L_801342F4:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
