    nofralloc
    cmplwi r3, 0x0
    bne @L_801F6598
    li r3, 0x0
    blr
    @L_801F6598:
    clrlwi r0, r4, 16
    cmplwi r0, 0x8
    blt @L_801F65AC
    li r3, 0x0
    blr
    @L_801F65AC:
    addis r3, r3, 0x1
    clrlslwi r0, r4, 16, 2
    add r3, r3, r0
    lwz r3, -0x5b3c(r3)
    blr
