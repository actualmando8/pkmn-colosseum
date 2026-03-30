    nofralloc
    lwz r4, 0x0(r3)
    rlwinm. r0, r4, 0, 29, 29
    bne @L_800EC550
    lfs f1, lbl_8047CC5C(r2)
    blr
    @L_800EC550:
    rlwinm. r0, r4, 0, 24, 24
    beq @L_800EC560
    lfs f1, 0xc8(r3)
    blr
    @L_800EC560:
    lfs f1, 0x98(r3)
    blr
