    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr. r31, r3
    beq @L_801A05D8
    lhz r0, 0x6(r31)
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_801A0568
    b @L_801A0580
    @L_801A0568:
    lhz r3, 0x6(r31)
    subi r0, r3, 0x1
    sth r0, 0x6(r31)
    lhz r0, 0x6(r31)
    cntlzw r0, r0
    srwi r0, r0, 5
    @L_801A0580:
    cmpwi r0, 0x0
    beq @L_801A05D8
    lhz r0, 0x4(r31)
    cmplwi r0, 0xffff
    bne @L_801A059C
    li r0, -0x1
    b @L_801A05A0
    @L_801A059C:
    lhz r0, 0x4(r31)
    @L_801A05A0:
    cmpwi r0, 0x0
    bge @L_801A05D8
    cmplwi r31, 0x0
    beq @L_801A05D8
    lwz r4, 0x0(r31)
    mr r3, r31
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r4, 0x0(r31)
    mr r3, r31
    lwz r12, 0x34(r4)
    mtctr r12
    bctrl
    @L_801A05D8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
