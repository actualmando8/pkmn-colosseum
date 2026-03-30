    nofralloc
    cmplwi r3, 0x0
    beqlr
    mr r5, r3
    li r0, 0x0
    b @L_80109278
    @L_80109270:
    stb r0, 0x4(r4)
    lwz r5, 0x0(r5)
    @L_80109278:
    lwz r4, 0x0(r5)
    cmplwi r4, 0x0
    bne @L_80109270
    li r0, 0x0
    stw r0, 0x0(r3)
    blr
