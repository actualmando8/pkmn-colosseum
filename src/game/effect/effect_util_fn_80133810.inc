    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r12, lbl_80478F88(r13)
    cmplwi r12, 0x0
    bne @L_80133838
    li r3, 0x0
    b @L_80133840
    @L_80133838:
    mtctr r12
    bctrl
    @L_80133840:
    cmplwi r3, 0x0
    beq @L_80133890
    lbz r0, lbl_8047AED0(r13)
    cmplwi r0, 0x0
    beq @L_80133890
    lwz r3, lbl_80478848(r13)
    bl fn_80102620
    clrlwi r3, r3, 24
    neg r0, r3
    or r0, r0, r3
    srwi. r0, r0, 31
    bne @L_80133890
    stb r31, lbl_8047AED1(r13)
    @L_80133874:
    li r3, 0x0
    bl fn_801338A4
    cmpwi r3, 0x0
    blt @L_80133890
    lbz r0, lbl_8047AED1(r13)
    cmplwi r0, 0x1
    beq @L_80133874
    @L_80133890:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
