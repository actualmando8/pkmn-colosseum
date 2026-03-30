    nofralloc
    cmplwi r3, 0x0
    bne @L_801F69AC
    li r3, 0x0
    blr
    @L_801F69AC:
    addis r3, r3, 0x1
    lwz r3, -0x5b4c(r3)
    blr
