    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6990
    li r3, 0x0
    blr
    @L_801F6990:
    addis r3, r3, 0x1
    lwz r3, -0x5b58(r3)
    blr
