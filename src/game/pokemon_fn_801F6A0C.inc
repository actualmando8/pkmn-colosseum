    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6A1C
    li r3, 0x0
    blr
    @L_801F6A1C:
    addis r3, r3, 0x1
    lwz r3, -0x5b68(r3)
    blr
