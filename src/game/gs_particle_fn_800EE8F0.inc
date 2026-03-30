    nofralloc
    lbz r0, lbl_8047ABC4(r13)
    cmplwi r0, 0x1
    beqlr
    lwz r0, lbl_8047ABD0(r13)
    cmplw r0, r3
    bne @L_800EE918
    lwz r3, lbl_8047ABC8(r13)
    li r0, 0x1
    stb r0, lbl_8047ABC4(r13)
    stw r3, lbl_8047ABCC(r13)
    @L_800EE918:
    lwz r3, lbl_8047ABC8(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047ABC8(r13)
    blr
