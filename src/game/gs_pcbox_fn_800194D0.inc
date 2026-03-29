    nofralloc
    lwz r5, 0x60(r3)
    li r3, 0x0
    lwz r0, 0x0(r5)
    stw r0, 0x64(r4)
    blr
