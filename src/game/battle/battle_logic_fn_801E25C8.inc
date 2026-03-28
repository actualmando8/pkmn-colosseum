    nofralloc
    lis r3, -32697
    addi r4, r3, -21408
    lwz r0, 0xa0(r4)
    cmpwi r0, 0x0
    beq @L_801E2604
    lbz r0, 0xa4(r4)
    cmplwi r0, 0x0
    beq @L_801E2604
    lwz r3, 0xe8(r4)
    cmplwi r3, 0x0
    beq @L_801E2604
    lwz r3, 0xc(r3)
    lwz r0, 0xc0(r4)
    add r3, r3, r0
    blr
@L_801E2604:
    li r3, -0x1
    blr
