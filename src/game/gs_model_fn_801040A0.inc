    nofralloc
    cmplwi r3, 0x0
    beq @L_801040B0
    addi r3, r3, 0x9c
    blr
    @L_801040B0:
    li r3, 0x0
    blr
