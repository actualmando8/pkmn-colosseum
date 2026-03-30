    nofralloc
    cmplwi r3, 0x0
    bne @L_801439A0
    li r3, 0x0
    blr
    @L_801439A0:
    lbz r0, 0x3(r3)
    extrwi r3, r0, 1, 24
    neg r0, r3
    or r0, r0, r3
    srwi r3, r0, 31
    blr
