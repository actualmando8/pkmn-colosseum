    nofralloc
    lwz r0, lbl_8047A4B0(r13)
    cmpwi r0, 0x0
    bne @L_8003955C
    li r0, 0x1b6d
    b @L_80039560
    @L_8003955C:
    li r0, 0x1b6e
    @L_80039560:
    stw r0, 0x4c(r4)
    li r3, 0x0
    blr
