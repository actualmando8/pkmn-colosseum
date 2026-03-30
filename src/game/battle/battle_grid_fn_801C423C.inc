    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r3
    cmplwi r30, 0x0
    bne @L_801C426C
    lis r3, lbl_80466E30@ha
    addi r3, r3, lbl_80466E30@l
    lwz r3, 0xc(r3)
    b @L_801C4304
    @L_801C426C:
    lis r3, lbl_80466E30@ha
    clrlwi r0, r4, 24
    addi r6, r3, lbl_80466E30@l
    li r3, 0x1
    clrlwi r4, r5, 16
    stb r3, 0x1(r6)
    li r3, 0x0
    cmplwi r0, 0x1
    sth r4, 0x2(r6)
    stfs f1, 0x14(r6)
    lfs f0, lbl_8047DFB8(r2)
    stfs f0, 0x18(r6)
    stb r3, 0x0(r6)
    lwz r31, 0xc(r6)
    stw r3, 0xc(r6)
    bne @L_801C42B4
    bl fn_801C432C
    b @L_801C42F0
    @L_801C42B4:
    lwz r0, 0x10(r6)
    cmplwi r0, 0x0
    beq @L_801C42F0
    bl fn_80109710
    lis r4, lbl_80466E30@ha
    addi r4, r4, lbl_80466E30@l
    lwz r0, 0x10(r4)
    cmplw r0, r3
    beq @L_801C42E0
    lwz r3, 0x10(r4)
    bl fn_800EF5A4
    @L_801C42E0:
    lis r3, lbl_80466E30@ha
    li r0, 0x0
    addi r3, r3, lbl_80466E30@l
    stw r0, 0x10(r3)
    @L_801C42F0:
    bl fn_801C6928
    mr r12, r30
    mtctr r12
    bctrl
    mr r3, r31
    @L_801C4304:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
