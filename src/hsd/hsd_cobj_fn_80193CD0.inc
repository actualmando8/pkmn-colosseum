    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047B230(r13)
    cmplw r3, r0
    bne @L_80193CF0
    li r0, 0x0
    stw r0, lbl_8047B230(r13)
@L_80193CF0:
    lis r4, lbl_8036C678@ha
    addi r0, r4, lbl_8036C678@l
    cmplw r3, r0
    bne @L_80193D08
    li r0, 0x0
    stw r0, lbl_8047B234(r13)
@L_80193D08:
    lis r4, lbl_8036C678@ha
    addi r4, r4, lbl_8036C678@l
    lwz r4, 0x14(r4)
    lwz r12, 0x38(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
