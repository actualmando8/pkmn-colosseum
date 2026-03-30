    nofralloc
    lbz r0, 0x0(r3)
    cmplwi r0, 0xff
    bne @L_800B431C
    li r3, -0x4
    blr
    @L_800B431C:
    lbz r0, 0x34(r3)
    rlwinm. r0, r0, 0, 29, 29
    beq @L_800B4330
    li r3, 0x0
    blr
    @L_800B4330:
    li r3, -0xa
    blr
