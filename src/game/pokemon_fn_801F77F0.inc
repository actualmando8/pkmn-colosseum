    nofralloc
    cmplwi r3, 0x0
    bne @L_801F7800
    li r3, 0x0
    blr
    @L_801F7800:
    clrlwi r0, r4, 24
    cmplwi r0, 0x2
    blt @L_801F7814
    li r3, 0x0
    blr
    @L_801F7814:
    clrlslwi r0, r4, 24, 2
    add r3, r3, r0
    lwz r3, 0xc(r3)
    blr
