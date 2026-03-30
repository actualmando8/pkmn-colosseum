    nofralloc
    lbz r0, 0x1(r3)
    cmplwi r0, 0x0
    beq @L_8013167C
    lwz r4, 0x30(r3)
    lbz r0, 0x0(r4)
    extsb r0, r0
    stb r0, 0x42(r3)
@L_8013167C:
    lwz r4, 0x30(r3)
    addi r0, r4, 0x1
    stw r0, 0x30(r3)
    li r3, 0x0
    blr
