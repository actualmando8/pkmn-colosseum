    nofralloc
    lwz r0, 0x0(r3)
    rlwinm. r0, r0, 0, 24, 24
    bne @L_800EC598
    lwz r3, 0x90(r3)
    li r0, -0x1
    stw r3, 0x0(r4)
    stw r0, 0x0(r5)
    blr
    @L_800EC598:
    lwz r0, 0xbc(r3)
    stw r0, 0x0(r4)
    lwz r0, 0xc0(r3)
    stw r0, 0x0(r5)
    blr
