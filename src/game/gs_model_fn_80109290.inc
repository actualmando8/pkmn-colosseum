    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    cmplwi r30, 0x0
    bne @L_801092B8
    li r3, 0x0
    b @L_80109340
@L_801092B8:
    lwz r31, lbl_8047AD1C(r13)
    li r0, 0x168
    mtctr r0
@L_801092C4:
    lbz r0, 0x4(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80109324
    mr r3, r31
    li r4, 0x0
    li r5, 0x78
    bl memset
    li r0, 0x7
    lfs f0, lbl_8047CE3C(r13)
    stb r0, 0x4(r31)
    li r0, -0x1
    mr r3, r30
    stfs f0, 0x68(r31)
    stfs f0, 0x6c(r31)
    stw r0, 0x64(r31)
    b @L_8010930C
@L_80109308:
    mr r3, r0
@L_8010930C:
    lwz r0, 0x0(r3)
    cmplwi r0, 0x0
    bne @L_80109308
    stw r31, 0x0(r3)
    mr r3, r31
    b @L_80109340
@L_80109324:
    addi r31, r31, 0x78
    bdnz @L_801092C4
    lis r3, lbl_80271EE8@ha
    addi r3, r3, lbl_80271EE8@l
    crxor 6, 6, 6
    bl fn_800DD970
    li r3, 0x0
@L_80109340:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr

