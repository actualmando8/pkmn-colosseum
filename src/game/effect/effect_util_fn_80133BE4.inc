    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80133E6C
    lwz r12, lbl_80478F8C(r13)
    cmplwi r12, 0x0
    bne @L_80133C08
    li r3, 0x0
    b @L_80133C10
    @L_80133C08:
    mtctr r12
    bctrl
    @L_80133C10:
    cmplwi r3, 0x0
    bne @L_80133C20
    li r0, 0x1
    b @L_80133C28
    @L_80133C20:
    lbz r0, 0x0(r3)
    extrwi r0, r0, 1, 24
    @L_80133C28:
    clrlwi r3, r0, 24
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
