    nofralloc
    lwz r0, 0x0(r3)
    rlwinm. r0, r0, 0, 24, 24
    beq @L_800E519C
    lwz r3, 0xc(r3)
    blr
    @L_800E519C:
    lwz r3, 0x8(r3)
    blr
