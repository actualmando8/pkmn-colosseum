    nofralloc
    lbz r0, 0x1(r3)
    cmplwi r0, 0x0
    beq @L_80131780
    lwz r4, 0x30(r3)
    lbz r0, 0x0(r4)
    stb r0, 0x2(r3)
@L_80131780:
    lwz r4, 0x30(r3)
    addi r0, r4, 0x1
    stw r0, 0x30(r3)
    li r3, 0x0
    blr
