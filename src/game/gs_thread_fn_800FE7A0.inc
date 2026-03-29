    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, 0xaaab
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    subi r31, r3, 0x5555
    stw r30, 0x8(r1)
    lwz r4, lbl_8047AC98(r13)
    b @L_800FE80C
@L_800FE7C4:
    lwz r0, 0x8(r4)
    lwz r30, 0x4(r4)
    cmpwi r0, 0x1
    bne @L_800FE808
    lbz r0, 0xd(r4)
    cmplwi r0, 0x0
    bne @L_800FE808
    lwz r0, lbl_8047AC7C(r13)
    stw r4, lbl_8047AC94(r13)
    subf r0, r0, r4
    mulhwu r0, r31, r0
    lwz r12, 0x14(r4)
    lwz r4, 0x10(r4)
    srwi r3, r0, 4
    addi r3, r3, 0x1
    mtctr r12
    bctrl
@L_800FE808:
    mr r4, r30
@L_800FE80C:
    cmplwi r4, 0x0
    bne @L_800FE7C4
    li r0, 0x0
    stw r0, lbl_8047AC94(r13)
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
