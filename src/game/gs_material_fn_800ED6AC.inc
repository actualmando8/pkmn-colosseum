    nofralloc
    lwz r0, 0x0(r3)
    rlwinm r0, r0, 0, 20, 20
    cmplwi r0, 0x800
    beq @L_800ED6D4
    lwz r0, 0x114(r3)
    cmpwi r0, 0x0
    beq @L_800ED6DC
    lwz r0, 0x118(r3)
    cmplwi r0, 0x0
    beq @L_800ED6DC
    @L_800ED6D4:
    li r3, 0x1
    blr
    @L_800ED6DC:
    li r3, 0x0
    blr
