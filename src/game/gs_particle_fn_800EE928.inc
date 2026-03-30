    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    lbz r0, lbl_8047ABE8(r13)
    cmplwi r0, 0x0
    beq @L_800EE9A4
    lis r3, lbl_804018F0@ha
    li r31, 0x1f
    addi r0, r3, lbl_804018F0@l
    mr r30, r0
    @L_800EE958:
    lbz r0, 0x0(r30)
    cmplwi r0, 0xff
    beq @L_800EE98C
    lwz r12, 0x4(r30)
    cmplwi r12, 0x0
    beq @L_800EE98C
    lwz r3, lbl_8047ABE0(r13)
    clrlslwi r0, r0, 24, 9
    lbz r5, 0x1(r30)
    add r4, r3, r0
    li r3, 0x1
    mtctr r12
    bctrl
    @L_800EE98C:
    cmpwi r31, 0x0
    addi r30, r30, 0x8
    subi r31, r31, 0x1
    bne @L_800EE958
    li r0, 0x0
    stb r0, lbl_8047ABE8(r13)
    @L_800EE9A4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
