    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, lbl_80478B30(r13)
    li r3, 0x0
    extsb r0, r0
    cmplwi r0, 0x9
    bgt @L_80106564
    lis r4, jumptable_8035B210@ha
    rlwinm r0, r0, 2, 0, 29
    addi r4, r4, jumptable_8035B210@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r3, 0x40
    b @L_80106564
    li r3, 0xf
    b @L_80106564
    li r3, 0x10
    b @L_80106564
    li r3, 0x40
    b @L_80106564
    li r3, 0x50
    b @L_80106564
    li r3, 0x51
    b @L_80106564
    li r3, 0x10c
    b @L_80106564
    li r3, 0xe6
    b @L_80106564
    li r3, 0xe8
    b @L_80106564
    li r3, 0x107
@L_80106564:
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_80106578
    li r0, -0x1
    b @L_801065A4
@L_80106578:
    lbz r0, 0x98(r3)
    cmplwi r0, 0x0
    beq @L_8010658C
    li r0, 0x0
    b @L_801065A4
@L_8010658C:
    lbz r0, 0x99(r3)
    cmplwi r0, 0x0
    beq @L_801065A0
    li r0, 0x0
    b @L_801065A4
@L_801065A0:
    li r0, 0x1
@L_801065A4:
    extsb r3, r0
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

