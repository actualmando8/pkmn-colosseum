    nofralloc
    lwz r4, 0x14(r3)
    lwz r5, 0x0(r3)
    lwz r0, 0x0(r4)
    add r0, r5, r0
    stw r0, 0x14(r3)
    li r3, 0x1
    blr
