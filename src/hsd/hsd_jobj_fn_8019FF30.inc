    nofralloc
    cmplwi r3, 0x0
    beq @L_8019FF40
    lwz r3, 0x14(r3)
    blr
    @L_8019FF40:
    li r3, 0x0
    blr
