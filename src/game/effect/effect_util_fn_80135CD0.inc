    nofralloc
    cmplwi r3, 0x0
    bne @L_80135CE0
    li r3, 0x0
    blr
@L_80135CE0:
    addi r3, r3, 0x8
    blr
