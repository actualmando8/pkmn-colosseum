    nofralloc
    cmplwi r3, 0x0
    bne @L_8020D918
    li r3, 0x0
    blr
@L_8020D918:
    lwz r3, 0x14(r3)
    blr
