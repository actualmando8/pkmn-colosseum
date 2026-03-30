    nofralloc
    cmplwi r3, 0x0
    stwu r1, -0x10(r1)
    beq @L_80140AC4
    cmplwi r4, 0x0
    beq @L_80140AC4
    lwz r5, 0x0(r3)
    lwz r0, 0x0(r4)
    stw r5, 0x8(r1)
    stw r0, 0x0(r3)
    stw r5, 0x0(r4)
    @L_80140AC4:
    addi r1, r1, 0x10
    blr
