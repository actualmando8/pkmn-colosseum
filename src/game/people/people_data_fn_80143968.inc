    nofralloc
    cmplwi r3, 0x0
    bne @L_80143978
    li r3, 0x0
    blr
    @L_80143978:
    lbz r0, 0x3(r3)
    extrwi r3, r0, 1, 25
    neg r0, r3
    or r0, r0, r3
    srwi r3, r0, 31
    blr
