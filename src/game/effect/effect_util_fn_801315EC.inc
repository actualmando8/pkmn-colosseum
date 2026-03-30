    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, 0x1(r3)
    cmplwi r0, 0x0
    bne @L_8013161C
    lhz r3, lbl_8047AEA4(r13)
    cmplwi r3, 0x0
    beq @L_8013161C
    li r4, 0x0
    li r5, 0xff
    bl fn_80165A20
    @L_8013161C:
    li r3, 0x0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
