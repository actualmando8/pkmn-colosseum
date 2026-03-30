    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x12
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    lwz r5, lbl_8047B610(r13)
    lbz r31, 0x1(r5)
    bl fn_801F025C
    rlwinm r0, r31, 0, 24, 31
    mr r31, r3
    cmplwi r0, 0x7
    bgt @L_80217DB8
    lis r3, jumptable_8039A028@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A028@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r5, 0xe6
    b @L_80217DBC
    li r5, 0xe7
    b @L_80217DBC
    li r5, 0xea
    b @L_80217DBC
    li r5, 0xe8
    b @L_80217DBC
    li r5, 0xe9
    b @L_80217DBC
    li r5, 0xeb
    b @L_80217DBC
    li r5, 0xec
    b @L_80217DBC
@L_80217DB8:
    li r5, 0x0
@L_80217DBC:
    mr r3, r31
    li r4, 0x0
    li r6, 0x0
    bl fn_8012640C
    extsb r30, r3
    mr r3, r31
    li r4, 0x9
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80217E00
    cmpwi r30, 0xc
    blt @L_80217E00
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x2(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80217E0C
@L_80217E00:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x6
    stw r0, lbl_8047B610(r13)
@L_80217E0C:
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
