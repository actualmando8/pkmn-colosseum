    nofralloc
    lbz r0, 0x17(r3)
    cmplwi r0, 0x1
    bne @L_800F04A8
    li r3, 0x0
    blr
    @L_800F04A8:
    lwz r4, 0x18(r3)
    li r0, 0x0
    stb r0, 0x8(r3)
    mr r3, r4
    blr
