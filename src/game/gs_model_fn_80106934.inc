    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, lbl_80478B30(r13)
    li r3, 0x0
    extsb r0, r0
    cmplwi r0, 0x9
    bgt @L_801069B8
    lis r4, jumptable_8035B2B0@ha
    rlwinm r0, r0, 2, 0, 29
    addi r4, r4, jumptable_8035B2B0@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r3, 0x40
    b @L_801069B8
    li r3, 0xf
    b @L_801069B8
    li r3, 0x10
    b @L_801069B8
    li r3, 0x40
    b @L_801069B8
    li r3, 0x50
    b @L_801069B8
    li r3, 0x51
    b @L_801069B8
    li r3, 0x10c
    b @L_801069B8
    li r3, 0xe6
    b @L_801069B8
    li r3, 0xe8
    b @L_801069B8
    li r3, 0x107
@L_801069B8:
    bl fn_80104704
    cmplwi r3, 0x0
    bne @L_801069CC
    li r3, -0x1
    b @L_801069EC
@L_801069CC:
    lbz r0, 0x98(r3)
    cmplwi r0, 0x0
    beq @L_801069E0
    li r3, 0x0
    b @L_801069EC
@L_801069E0:
    lbz r0, 0x99(r3)
    cntlzw r0, r0
    rlwinm r3, r0, 27, 5, 31
@L_801069EC:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

