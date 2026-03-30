    nofralloc
    cmplwi r3, 0x0
    bne @L_80143888
    li r3, 0x0
    blr
    @L_80143888:
    lbz r0, 0x3(r3)
    clrlwi r3, r0, 31
    neg r0, r3
    or r0, r0, r3
    srwi r3, r0, 31
    blr
