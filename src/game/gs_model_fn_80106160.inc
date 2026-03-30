    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, lbl_80478B30(r13)
    li r3, 0x0
    extsb r0, r0
    cmplwi r0, 0x9
    bgt @L_801061E4
    lis r4, jumptable_8035B198@ha
    rlwinm r0, r0, 2, 0, 29
    addi r4, r4, jumptable_8035B198@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r3, 0x40
    b @L_801061E4
    li r3, 0xf
    b @L_801061E4
    li r3, 0x10
    b @L_801061E4
    li r3, 0x40
    b @L_801061E4
    li r3, 0x50
    b @L_801061E4
    li r3, 0x51
    b @L_801061E4
    li r3, 0x10c
    b @L_801061E4
    li r3, 0xe6
    b @L_801061E4
    li r3, 0xe8
    b @L_801061E4
    li r3, 0x107
@L_801061E4:
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_801061F8
    li r0, -0x1
    b @L_80106224
@L_801061F8:
    lbz r0, 0x98(r3)
    cmplwi r0, 0x0
    beq @L_8010620C
    li r0, 0x0
    b @L_80106224
@L_8010620C:
    lbz r0, 0x99(r3)
    cmplwi r0, 0x0
    beq @L_80106220
    li r0, 0x0
    b @L_80106224
@L_80106220:
    li r0, 0x1
@L_80106224:
    extsb r0, r0
    rlwinm r0, r0, 1, 31, 31
    xori r0, r0, 0x1
    rlwinm r3, r0, 0, 24, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

