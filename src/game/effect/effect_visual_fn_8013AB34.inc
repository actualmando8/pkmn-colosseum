    nofralloc
    lwz r3, 0x54(r3)
    cmplwi r3, 0x0
    beq @L_8013AB58
    lwz r3, 0x8(r3)
    addis r0, r3, 0x1
    cmplwi r0, 0xffff
    bne @L_8013AB58
    li r3, 0x1
    blr
    @L_8013AB58:
    li r3, 0x0
    blr
