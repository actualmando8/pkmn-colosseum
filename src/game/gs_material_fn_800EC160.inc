    nofralloc
    clrlwi. r0, r4, 24
    beq @L_800EC178
    lwz r0, 0x0(r3)
    ori r0, r0, 0x800
    stw r0, 0x0(r3)
    blr
    @L_800EC178:
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 21, 19
    stw r0, 0x0(r3)
    blr
