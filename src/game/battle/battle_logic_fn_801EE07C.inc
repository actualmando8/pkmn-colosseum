    nofralloc
    lwz r4, -30888(r13)
    clrlwi r5, r3, 16
    lwz r0, 0x0(r4)
    cmplw r5, r0
    blt @L_801EE098
    lwz r3, -30884(r13)
    blr
@L_801EE098:
    lwz r3, -30884(r13)
    slwi r0, r5, 3
    add r3, r3, r0
    blr
