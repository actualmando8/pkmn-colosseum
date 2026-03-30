    nofralloc
    lwz r0, lbl_8047B268(r13)
    li r4, 0x0
    cmplw r0, r3
    bgt @L_8019C7D8
    lwz r0, lbl_8047B26C(r13)
    cmplw r3, r0
    bge @L_8019C7D8
    li r4, 0x1
    @L_8019C7D8:
    mr r3, r4
    blr
