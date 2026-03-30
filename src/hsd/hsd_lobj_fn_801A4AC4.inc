    nofralloc
    lwz r5, lbl_8047B2B4(r13)
    clrlwi r3, r3, 30
    b @L_801A4AF0
    @L_801A4AD0:
    lwz r4, 0x4(r5)
    lhz r0, 0x8(r4)
    clrlwi r0, r0, 30
    cmplw r3, r0
    bne @L_801A4AEC
    lwz r3, 0x4(r5)
    blr
    @L_801A4AEC:
    lwz r5, 0x0(r5)
    @L_801A4AF0:
    cmplwi r5, 0x0
    bne @L_801A4AD0
    li r3, 0x0
    blr
