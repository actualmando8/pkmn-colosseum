    nofralloc
    clrlwi r0, r3, 24
    lwz r3, -30892(r13)
    slwi r0, r0, 1
    lhzx r3, r3, r0
    blr
