    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r31, 0xc(r3)
    cmplwi r31, 0x0
    beq @L_8019B90C
    lis r3, 0x1
    lhz r4, 0x4(r31)
    subi r3, r3, 0x1
    clrlwi r0, r3, 16
    subf r0, r4, r0
    cntlzw r0, r0
    srwi. r3, r0, 5
    beq @L_8019B8BC
    b @L_8019B8D4
    @L_8019B8BC:
    lhz r0, 0x4(r31)
    lhz r3, 0x4(r31)
    cntlzw r4, r0
    subi r0, r3, 0x1
    sth r0, 0x4(r31)
    srwi r3, r4, 5
    @L_8019B8D4:
    cmpwi r3, 0x0
    beq @L_8019B90C
    cmplwi r31, 0x0
    beq @L_8019B90C
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
    @L_8019B90C:
    lwz r3, 0x1c(r30)
    bl fn_801C25E4
    lis r4, lbl_8036C7E8@ha
    mr r3, r30
    addi r4, r4, lbl_8036C7E8@l
    lwz r4, 0x14(r4)
    lwz r12, 0x30(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
