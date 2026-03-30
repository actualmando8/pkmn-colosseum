    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    bl fn_80104704
    cmplwi r3, 0x0
    beq @L_80102414
    lbz r4, 0x95(r3)
    lbz r0, 0x94(r3)
    extsb r3, r4
    extsb r0, r0
    add r3, r3, r0
    b @L_80102418
    @L_80102414:
    li r3, -0x1
    @L_80102418:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
