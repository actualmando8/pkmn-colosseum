    nofralloc
    mr. r4, r3
    beq @L_80135C6C
    lwz r3, 0x0(r4)
    lwz r4, 0x4(r4)
    blr
    @L_80135C6C:
    li r4, 0x0
    li r3, 0x0
    blr
