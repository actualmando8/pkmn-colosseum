    nofralloc
    cmplwi r3, 0x0
    beq @L_801040E8
    rlwinm r0, r4, 2, 0, 29
    add r3, r3, r0
    lwz r3, 0x60(r3)
    blr
@L_801040E8:
    li r3, 0x0
    blr
