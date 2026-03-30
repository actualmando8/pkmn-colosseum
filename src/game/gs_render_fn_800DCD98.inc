    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r31, 0xc(r3)
    cmplwi r31, 0x0
    beq @L_800DCE28
    lis r3, 0x1
    lhz r4, 0x4(r31)
    subi r0, r3, 0x1
    clrlwi r0, r0, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r0, r0, 5
    beq @L_800DCDE0
    b @L_800DCDF0
    @L_800DCDE0:
    subi r3, r4, 0x1
    cntlzw r0, r4
    sth r3, 0x4(r31)
    srwi r0, r0, 5
    @L_800DCDF0:
    cmpwi r0, 0x0
    beq @L_800DCE28
    cmplwi r31, 0x0
    beq @L_800DCE28
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
    @L_800DCE28:
    li r0, 0x0
    stb r0, 0x1(r30)
    stb r0, 0x0(r30)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
