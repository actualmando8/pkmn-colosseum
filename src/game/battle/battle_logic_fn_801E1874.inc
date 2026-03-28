    nofralloc
    lbz r0, -21472(r13)
    cmplwi r0, 0x0
    beq @L_801E188C
    lbz r0, -21471(r13)
    cmplwi r0, 0x0
    bne @L_801E1894
@L_801E188C:
    li r3, 0x0
    blr
@L_801E1894:
    li r3, 0x1
    blr
