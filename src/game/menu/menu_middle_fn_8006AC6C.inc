    nofralloc
    lwz r4, lbl_80478F20(r13)
    clrlwi r3, r3, 16
    lwz r0, 0x0(r4)
    cmplw r0, r3
    bgt @L_8006AC88
    li r3, -0x1
    blr
    @L_8006AC88:
    cmpwi r3, 0x9
    bge @L_8006ACA0
    cmpwi r3, 0x1
    beq @L_8006ACC4
    bge @L_8006ACB4
    b @L_8006ACC4
    @L_8006ACA0:
    cmpwi r3, 0x30a
    bge @L_8006ACC4
    cmpwi r3, 0x308
    bge @L_8006ACBC
    b @L_8006ACC4
    @L_8006ACB4:
    li r3, 0x1
    blr
    @L_8006ACBC:
    li r3, 0x2
    blr
    @L_8006ACC4:
    li r3, 0x0
    blr
