    nofralloc
    lwz r4, 0x4(r3)
    li r3, -0x1
    lwz r0, -32728(r13)
    cmpw r4, r0
    bltlr
    subf r3, r0, r4
    blr
