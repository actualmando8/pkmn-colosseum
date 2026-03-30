    nofralloc
    lbz r0, 0x1(r3)
    cmplwi r0, 0x0
    bne @L_801324B8
    lha r0, 0x48(r3)
    cmpwi r0, 0x0
    bne @L_80132480
    lwz r4, 0x30(r3)
    lbz r4, 0x0(r4)
    addi r0, r4, 0x1
    extsh r0, r0
    sth r0, 0x48(r3)
    @L_80132480:
    lha r4, 0x48(r3)
    subi r4, r4, 0x1
    extsh r0, r4
    sth r4, 0x48(r3)
    cmpwi r0, 0x0
    bgt @L_801324A4
    li r0, 0x0
    sth r0, 0x48(r3)
    b @L_801324B8
    @L_801324A4:
    lwz r4, 0x30(r3)
    subi r0, r4, 0x3
    stw r0, 0x30(r3)
    li r3, 0x1
    blr
    @L_801324B8:
    lwz r4, 0x30(r3)
    addi r0, r4, 0x1
    stw r0, 0x30(r3)
    li r3, 0x0
    blr
