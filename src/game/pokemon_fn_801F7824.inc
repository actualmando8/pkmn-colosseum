    nofralloc
    cmplwi r3, 0x0
    bne @L_801F7834
    li r3, 0x0
    blr
    @L_801F7834:
    clrlwi r0, r4, 24
    cmplwi r0, 0x2
    blt @L_801F7848
    li r3, 0x0
    blr
    @L_801F7848:
    clrlslwi r0, r4, 24, 2
    add r3, r3, r0
    lwz r3, 0x4(r3)
    blr
