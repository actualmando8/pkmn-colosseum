    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, lbl_8047AD10(r13)
    cmplwi r0, 0x0
    bne @L_80106AC4
    lbz r0, lbl_80478B30(r13)
    li r31, 0x0
    extsb r0, r0
    cmplwi r0, 0x9
    bgt @L_80106A98
    lis r3, jumptable_8035B2D8@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8035B2D8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r31, 0x40
    b @L_80106A98
    li r31, 0xf
    b @L_80106A98
    li r31, 0x10
    b @L_80106A98
    li r31, 0x40
    b @L_80106A98
    li r31, 0x50
    b @L_80106A98
    li r31, 0x51
    b @L_80106A98
    li r31, 0x10c
    b @L_80106A98
    li r31, 0xe6
    b @L_80106A98
    li r31, 0xe8
    b @L_80106A98
    li r31, 0x107
@L_80106A98:
    mr r3, r31
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80106ABC
    mr r3, r31
    mr r5, r30
    li r4, 0x2
    bl fn_80102568
@L_80106ABC:
    li r0, -0x1
    stb r0, lbl_80478B30(r13)
@L_80106AC4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

