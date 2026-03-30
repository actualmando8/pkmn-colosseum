    nofralloc
    cmplwi r3, 0x0
    bne @L_801FCFE4
    li r3, 0x0
    blr
    @L_801FCFE4:
    lha r3, 0x674(r3)
    blr
