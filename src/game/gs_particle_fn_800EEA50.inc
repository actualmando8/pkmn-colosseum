    nofralloc
    lwz r0, lbl_8047ABE0(r13)
    clrrwi r3, r3, 28
    clrrwi r0, r0, 28
    subf r0, r3, r0
    cntlzw r0, r0
    extrwi r3, r0, 8, 19
    blr
