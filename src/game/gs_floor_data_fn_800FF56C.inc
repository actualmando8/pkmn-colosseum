    nofralloc
    lwz r3, lbl_8047ACC8(r13)
    lwz r3, 0x0(r3)
    cmplwi r3, 0x0
    beq @L_800FF584
    lwz r3, 0xc(r3)
    blr
@L_800FF584:
    li r3, 0x0
    blr

