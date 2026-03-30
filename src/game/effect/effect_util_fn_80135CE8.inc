    nofralloc
    cmplwi r3, 0x0
    bne @L_80135CF8
    li r3, 0x0
    b @L_80135CFC
    @L_80135CF8:
    addi r3, r3, 0x8
    @L_80135CFC:
    cmplwi r3, 0x0
    beqlr
    li r0, 0x0
    stw r0, 0x8(r3)
    blr
