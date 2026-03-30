    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lwz r0, lbl_8047B378(r13)
    cmplw r3, r0
    bne @L_801BBBD4
    li r0, 0x0
    stw r0, lbl_8047B378(r13)
@L_801BBBD4:
    lis r4, lbl_8036D3F0@ha
    addi r0, r4, lbl_8036D3F0@l
    cmplw r3, r0
    bne @L_801BBBEC
    li r0, 0x0
    stw r0, lbl_8047B37C(r13)
@L_801BBBEC:
    lis r4, lbl_8036D3F0@ha
    addi r4, r4, lbl_8036D3F0@l
    lwz r4, 0x14(r4)
    lwz r12, 0x38(r4)
    mtctr r12
    bctrl
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

