    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_80134324
    li r3, 0x0
    b @L_8013432C
    @L_80134324:
    mtctr r12
    bctrl
    @L_8013432C:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
