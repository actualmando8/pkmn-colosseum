    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    bne @L_801315D4
    lhz r3, lbl_8047AEA4(r13)
    cmplwi r3, 0x0
    bne @L_801315BC
    li r3, 0x0
    b @L_801315D8
    @L_801315BC:
    bl fn_801666BC
    cmpwi r3, 0x2
    bne @L_801315D4
    lwz r3, 0x30(r31)
    subi r0, r3, 0x3
    stw r0, 0x30(r31)
    @L_801315D4:
    li r3, 0x1
    @L_801315D8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
