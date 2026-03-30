    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lwz r0, -23980(r13)
    cmplw r30, r0
    bne @L_800D2764
    li r0, 0x0
    stw r0, -23980(r13)
@L_800D2764:
    lwz r31, 0xc(r30)
    cmplwi r31, 0x0
    beq @L_800D27DC
    lis r3, 0x1
    lhz r4, 0x4(r31)
    subi r0, r3, 0x1
    rlwinm r0, r0, 0, 16, 31
    subf r0, r4, r0
    cntlzw r0, r0
    rlwinm. r0, r0, 27, 5, 31
    beq @L_800D2794
    b @L_800D27A4
@L_800D2794:
    subi r3, r4, 0x1
    cntlzw r0, r4
    sth r3, 0x4(r31)
    rlwinm r0, r0, 27, 5, 31
@L_800D27A4:
    cmpwi r0, 0x0
    beq @L_800D27DC
    cmplwi r31, 0x0
    beq @L_800D27DC
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
@L_800D27DC:
    li r0, 0x0
    stb r0, 0x0(r30)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
