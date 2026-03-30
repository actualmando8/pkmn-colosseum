    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, 0xaaab
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    subi r31, r3, 0x5555
    stw r30, 0x8(r1)
    lwz r4, -23432(r13)
    b @L_800FEC0C
@L_800FEBC4:
    lwz r0, 0x8(r4)
    lwz r30, 0x4(r4)
    cmpwi r0, 0x3
    bne @L_800FEC08
    lbz r0, 0xd(r4)
    cmplwi r0, 0x0
    bne @L_800FEC08
    lwz r0, -23460(r13)
    stw r4, -23436(r13)
    subf r0, r0, r4
    mulhwu r0, r31, r0
    lwz r12, 0x14(r4)
    lwz r4, 0x10(r4)
    rlwinm r3, r0, 28, 4, 31
    addi r3, r3, 0x1
    mtctr r12
    bctrl
@L_800FEC08:
    mr r4, r30
@L_800FEC0C:
    cmplwi r4, 0x0
    bne @L_800FEBC4
    li r0, 0x0
    stw r0, -23436(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
