    nofralloc
    addis r3, r3, 0x1
    lbz r0, -0x3678(r3)
    cmplwi r0, 0x0
    beq @L_8006AFDC
    subi r3, r3, 0x4cd8
    blr
    @L_8006AFDC:
    li r3, 0x0
    blr
