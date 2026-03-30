    nofralloc
    lwz r5, lbl_8047ABB0(r13)
    lwz r0, lbl_8047ABAC(r13)
    addi r4, r5, 0x1
    cmplw r5, r0
    stw r4, lbl_8047ABB0(r13)
    bnelr
    stw r3, lbl_8047ABA8(r13)
    blr
