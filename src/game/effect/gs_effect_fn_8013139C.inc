    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    beq @L_801313E8
    lis r5, -32714
    addi r5, r5, 13760
    lwz r0, 0x0(r5)
    cmplw r3, r0
    bgt @L_801313E8
    subi r0, r3, 0x1
    lwz r3, 0xc(r5)
    mulli r0, r0, 0x34
    add r31, r3, r0
    lwz r0, 0x4(r31)
    cmpwi r0, -0x1
    beq @L_801313E8
    b @L_801313EC
@L_801313E8:
    li r31, 0x0
@L_801313EC:
    cmplwi r31, 0x0
    beq @L_80131414
    lwz r12, 0x8(r31)
    cmplwi r12, 0x0
    beq @L_8013140C
    lwz r3, 0x24(r31)
    mtctr r12
    bctrl
@L_8013140C:
    li r0, 0x1
    stw r0, 0x4(r31)
@L_80131414:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
