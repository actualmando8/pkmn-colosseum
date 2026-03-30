    nofralloc
    cmplwi r3, 0x8
    bgt @L_801A4ABC
    lis r4, jumptable_8036CA64@ha
    slwi r0, r3, 2
    addi r3, r4, jumptable_8036CA64@l
    lwzx r3, r3, r0
    mtctr r3
    bctr
    li r3, 0x1
    blr
    li r3, 0x2
    blr
    li r3, 0x4
    blr
    li r3, 0x8
    blr
    li r3, 0x10
    blr
    li r3, 0x20
    blr
    li r3, 0x40
    blr
    li r3, 0x80
    blr
    li r3, 0x100
    blr
    @L_801A4ABC:
    li r3, 0x0
    blr
