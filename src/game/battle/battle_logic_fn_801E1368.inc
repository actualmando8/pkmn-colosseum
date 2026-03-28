    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    lbz r0, -21504(r13)
    cmplwi r0, 0x0
    beq @L_801E16C0
    lbz r0, -21484(r13)
    cmplwi r0, 0x0
    bne @L_801E1390
    b @L_801E16C0
@L_801E1390:
    lwz r0, -21480(r13)
    cmplwi r0, 0x0
    beq @L_801E16C0
    li r3, 0x1
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    lfs f1, -21112(r2)
    lfs f3, -21108(r2)
    fmr f2, f1
    lfs f4, -21104(r2)
    bl fn_800D9B58
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x1
    bl fn_800D9ED8
    li r3, 0x4
    bl fn_800D6A00
    lbz r0, -21483(r13)
    cmplwi r0, 0x0
    beq @L_801E14C4
    li r3, 0x2
    bl fn_800D888C
    lis r3, -32719
    addi r3, r3, 18776
    bl fn_800D7820
    li r3, 0x4
    bl fn_800D67BC
    lfs f1, -21100(r2)
    lfs f2, -21096(r2)
    lfs f3, -21112(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5C18
    lfs f1, -21092(r2)
    lfs f2, -21096(r2)
    lfs f3, -21112(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5C18
    lfs f1, -21100(r2)
    lfs f2, -21088(r2)
    lfs f3, -21112(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5C18
    lfs f1, -21092(r2)
    lfs f2, -21088(r2)
    lfs f3, -21112(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5C18
    bl fn_800D6728
@L_801E14C4:
    li r3, 0x2
    bl fn_800D88DC
    lis r3, -32719
    addi r3, r3, 19576
    bl fn_800D7820
    lwz r4, -21480(r13)
    li r3, 0x0
    bl fn_800D85D4
    li r3, 0x4
    bl fn_800D67BC
    lfs f1, -21084(r2)
    lfs f2, -21080(r2)
    lfs f3, -21112(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5C18
    lfs f1, -21112(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    lfs f1, -21076(r2)
    lfs f2, -21080(r2)
    lfs f3, -21112(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5C18
    lfs f1, -21072(r2)
    li r3, 0x0
    lfs f2, -21112(r2)
    bl fn_800D59B8
    lfs f1, -21084(r2)
    lfs f2, -21068(r2)
    lfs f3, -21112(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5C18
    lfs f1, -21112(r2)
    li r3, 0x0
    lfs f2, -21072(r2)
    bl fn_800D59B8
    lfs f1, -21076(r2)
    lfs f2, -21068(r2)
    lfs f3, -21112(r2)
    bl fn_800D6680
    li r3, 0x0
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800D5C18
    lfs f1, -21072(r2)
    li r3, 0x0
    fmr f2, f1
    bl fn_800D59B8
    bl fn_800D6728
    lwz r0, -21500(r13)
    cmpwi r0, 0x2
    beq @L_801E1628
    bge @L_801E15E0
    cmpwi r0, 0x0
    beq @L_801E15F0
    bge @L_801E160C
    b @L_801E16B8
@L_801E15E0:
    cmpwi r0, 0x4
    beq @L_801E16A0
    bge @L_801E16B8
    b @L_801E1644
@L_801E15F0:
    li r3, 0x230
    li r4, 0x2c
    li r5, -0x1
    addi r6, r2, -21064
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801E16B8
@L_801E160C:
    li r3, 0x230
    li r4, 0x2c
    li r5, -0x1
    addi r6, r2, -21060
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801E16B8
@L_801E1628:
    li r3, 0x230
    li r4, 0x2c
    li r5, -0x1
    addi r6, r2, -21052
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801E16B8
@L_801E1644:
    lwz r0, -21496(r13)
    cmpwi r0, 0x2
    bge @L_801E165C
    cmpwi r0, 0x0
    bge @L_801E1668
    b @L_801E16B8
@L_801E165C:
    cmpwi r0, 0x4
    bge @L_801E16B8
    b @L_801E1684
@L_801E1668:
    li r3, 0x230
    li r4, 0x2c
    li r5, -0x1
    addi r6, r2, -21044
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801E16B8
@L_801E1684:
    li r3, 0x230
    li r4, 0x2c
    li r5, -0x1
    addi r6, r2, -21060
    crxor 6, 6, 6
    bl fn_800FAEF8
    b @L_801E16B8
@L_801E16A0:
    li r3, 0x230
    li r4, 0x2c
    li r5, -0x1
    addi r6, r2, -21036
    crxor 6, 6, 6
    bl fn_800FAEF8
@L_801E16B8:
    li r3, 0x0
    bl fn_800D9ED8
@L_801E16C0:
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
