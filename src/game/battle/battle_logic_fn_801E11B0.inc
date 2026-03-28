    nofralloc
    lwz r0, -21496(r13)
    li r3, 0x1
    stw r3, -21500(r13)
    cmpwi r0, 0x2
    beqlr
    stw r3, -21496(r13)
    blr
