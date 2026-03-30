    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    lis r3, lbl_80466BC0@ha
    lis r4, lbl_80466BC0@ha
    stw r0, 0x14(r1)
    addi r3, r3, lbl_80466BC0@l
    li r5, 0x0
    lwz r0, 0x1e8(r3)
    addi r3, r4, lbl_80466BC0@l
    stw r5, 0x1e0(r3)
    cmplwi r0, 0x0
    beq @L_801BFD00
    lis r3, lbl_80466BC0@ha
    lis r4, lbl_80466BC0@ha
    addi r3, r3, lbl_80466BC0@l
    lwz r12, 0x1e8(r3)
    addi r3, r4, lbl_80466BC0@l
    lwz r3, 0x1e4(r3)
    mtctr r12
    bctrl
    @L_801BFD00:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
