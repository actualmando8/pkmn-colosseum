    nofralloc
    clrlwi. r0, r4, 31
    beq @L_800E911C
    lwz r0, 0x0(r3)
    oris r0, r0, 0x1000
    stw r0, 0x0(r3)
    @L_800E911C:
    rlwinm. r0, r4, 0, 30, 30
    beq @L_800E9130
    lwz r0, 0x0(r3)
    oris r0, r0, 0x2000
    stw r0, 0x0(r3)
    @L_800E9130:
    rlwinm. r0, r4, 0, 29, 29
    beqlr
    lwz r0, 0x0(r3)
    oris r0, r0, 0x4000
    stw r0, 0x0(r3)
    blr
