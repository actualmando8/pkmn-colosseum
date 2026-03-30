    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
    li r5, 0x51
    cmpwi r5, 0x0
    beq @L_80105F94
    lbz r0, lbl_80478B30(r13)
    extsb r4, r0
    cmpwi r4, 0x5
    beq @L_80105F58
    lbz r0, lbl_8047AD10(r13)
    cmplwi r0, 0x0
    bne @L_80105F58
    cmplwi r4, 0x9
    li r31, 0x0
    bgt @L_80105F34
    lis r3, jumptable_8035B148@ha
    rlwinm r0, r4, 2, 0, 29
    addi r3, r3, jumptable_8035B148@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r31, 0x40
    b @L_80105F34
    li r31, 0xf
    b @L_80105F34
    li r31, 0x10
    b @L_80105F34
    li r31, 0x40
    b @L_80105F34
    li r31, 0x50
    b @L_80105F34
    mr r31, r5
    b @L_80105F34
    li r31, 0x10c
    b @L_80105F34
    li r31, 0xe6
    b @L_80105F34
    li r31, 0xe8
    b @L_80105F34
    li r31, 0x107
@L_80105F34:
    mr r3, r31
    bl fn_80102620
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x0
    beq @L_80105F58
    mr r3, r31
    mr r5, r30
    li r4, 0x2
    bl fn_80102568
@L_80105F58:
    li r0, 0x5
    stb r0, lbl_80478B30(r13)
    bl fn_801046B8
    li r0, 0x0
    mr r4, r3
    stw r0, 0x8(r1)
    mr r7, r30
    mr r9, r29
    li r3, 0x51
    li r5, 0x0
    li r6, 0x0
    li r8, 0x3
    li r10, 0x1
    crxor 6, 6, 6
    bl fn_801026A4
@L_80105F94:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr

