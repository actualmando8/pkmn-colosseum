    nofralloc
    lis r4, 0x8046
    addi r4, r4, 29688
    lwz r0, 0xc(r4)
    or r0, r0, r3
    stw r0, 0xc(r4)
    blr
