    nofralloc
    lwz r3, 0x60(r3)
    lbz r0, 0x1d(r3)
    clrlwi r0, r0, 31
    cmpwi r0, 0x0
    beq @L_8002AE60
    li r0, 0x0
    stb r0, 0x67(r4)
    @L_8002AE60:
    li r3, 0x0
    blr
