    nofralloc
    cmplwi r3, 0x0
    beq @L_80135C20
    lfs f1, 0x18(r3)
    blr
    @L_80135C20:
    lfs f1, lbl_8047D110(r2)
    blr
