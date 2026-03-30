    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r5, lbl_8036CB30@ha
    stw r0, 0x14(r1)
    addi r5, r5, lbl_8036CB30@l
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r12, 0x40(r5)
    mtctr r12
    bctrl
    cmpwi r3, 0x0
    beq @L_800DFECC
    b @L_800DFED8
    @L_800DFECC:
    li r0, 0x0
    li r3, 0x0
    stw r0, 0x20(r31)
    @L_800DFED8:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
