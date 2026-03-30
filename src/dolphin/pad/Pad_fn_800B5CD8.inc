    nofralloc
    cmplwi r3, 0x14
    blt @L_800B5CE8
    li r3, 0x0
    b @L_800B5CF8
    @L_800B5CE8:
    slwi r3, r3, 4
    lwz r0, gx(r2)
    addi r3, r3, 0x2d0
    add r3, r0, r3
    @L_800B5CF8:
    blr
