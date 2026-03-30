    nofralloc
    cmplwi r3, 0x0
    bne @L_801FD818
    li r3, 0x0
    blr
    @L_801FD818:
    lwz r3, 0x600(r3)
    blr
