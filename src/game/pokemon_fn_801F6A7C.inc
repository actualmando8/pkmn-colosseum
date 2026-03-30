    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6A8C
    li r3, 0x0
    blr
    @L_801F6A8C:
    addis r3, r3, 0x1
    lwz r3, -0x5b88(r3)
    blr
