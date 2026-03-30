    nofralloc
    rlwinm. r0, r3, 0, 24, 31
    beq @L_800B7568
    li r0, 0x1
    b @L_800B756C
@L_800B7568:
    li r0, 0x0
@L_800B756C:
    lwz r4, -29632(r13)
    rlwinm r0, r0, 4, 0, 27
    lwz r3, 0x8(r4)
    rlwinm r3, r3, 0, 28, 26
    or r0, r3, r0
    stw r0, 0x8(r4)
    lwz r0, 0x8(r4)
    lwz r3, -24228(r13)
    sth r0, 0x2(r3)
    blr
