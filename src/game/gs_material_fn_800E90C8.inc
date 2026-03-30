    nofralloc
    clrlwi. r0, r4, 31
    beq @L_800E90DC
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 4, 2
    stw r0, 0x0(r3)
    @L_800E90DC:
    rlwinm. r0, r4, 0, 30, 30
    beq @L_800E90F0
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 3, 1
    stw r0, 0x0(r3)
    @L_800E90F0:
    rlwinm. r0, r4, 0, 29, 29
    beqlr
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 2, 0
    stw r0, 0x0(r3)
    blr
