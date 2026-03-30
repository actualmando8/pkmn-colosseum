    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr r30, r3
    lbz r0, 0x2(r3)
    cmplwi r0, 0x0
    beq @L_800DCC14
    lwz r3, 0xc(r30)
    bl fn_801A66E0
    lwz r0, 0x58(r30)
    cmplw r31, r0
    bgt @L_800DCC14
    stw r31, 0x60(r30)
    lwz r3, 0x8(r30)
    lwz r0, 0x60(r30)
    lwz r4, 0x4(r3)
    slwi r0, r0, 2
    lwz r3, 0xc(r30)
    lwzx r4, r4, r0
    bl fn_801A426C
    lwz r3, 0xc(r30)
    lfs f1, lbl_8047CA78(r2)
    bl fn_801A6370
    lfs f0, lbl_8047CA78(r2)
    lis r3, fn_800DD128@ha
    addi r6, r3, fn_800DD128@l
    li r4, 0x7
    stfs f0, lbl_8047AAF4(r13)
    lis r3, 0x1
    subi r5, r3, 0x1
    li r7, 0x0
    lwz r3, 0xc(r30)
    crxor 6, 6, 6
    bl fn_801C028C
    lfs f0, lbl_8047AAF4(r13)
    stfs f0, 0x6c(r30)
    @L_800DCC14:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
