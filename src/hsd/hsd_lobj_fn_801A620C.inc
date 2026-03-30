    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    lis r6, lbl_80274D58@ha
    lis r5, lbl_80274D64@ha
    stw r0, 0x34(r1)
    stw r31, 0x2c(r1)
    mr r31, r4
    stw r30, 0x28(r1)
    mr. r30, r3
    lwzu r4, lbl_80274D64@l(r5)
    lwzu r8, lbl_80274D58@l(r6)
    lwz r3, 0x4(r5)
    lwz r7, 0x4(r6)
    lwz r6, 0x8(r6)
    lwz r0, 0x8(r5)
    stw r8, 0x14(r1)
    stw r7, 0x18(r1)
    stw r6, 0x1c(r1)
    stw r4, 0x8(r1)
    stw r3, 0xc(r1)
    stw r0, 0x10(r1)
    beq @L_801A6358
    cmplwi r30, 0x0
    beq @L_801A6284
    lwz r0, 0x18(r30)
    cmplwi r0, 0x0
    beq @L_801A6284
    lwz r3, 0x18(r30)
    addi r4, r1, 0x14
    bl fn_80191688
    @L_801A6284:
    cmplwi r30, 0x0
    beq @L_801A62A4
    lwz r0, 0x1c(r30)
    cmplwi r0, 0x0
    beq @L_801A62A4
    lwz r3, 0x1c(r30)
    addi r4, r1, 0x8
    bl fn_80191688
    @L_801A62A4:
    mr r5, r31
    addi r3, r1, 0x8
    addi r4, r1, 0x14
    bl fn_800A3A9C
    cmplwi r31, 0x0
    beq @L_801A62C4
    cmplwi r31, 0x0
    bne @L_801A62CC
    @L_801A62C4:
    li r0, -0x1
    b @L_801A6338
    @L_801A62CC:
    lfs f0, 0x0(r31)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A6328
    lfs f0, 0x4(r31)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A6328
    lfs f0, 0x8(r31)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801A6328
    li r0, -0x1
    b @L_801A6338
    @L_801A6328:
    mr r3, r31
    mr r4, r31
    bl fn_800A3ADC
    li r0, 0x0
    @L_801A6338:
    cmpwi r0, 0x0
    beq @L_801A6358
    lfs f0, lbl_8047DBE8(r2)
    lfs f1, lbl_8047DBE8(r2)
    stfs f0, 0x0(r31)
    lfs f0, lbl_8047DBE4(r2)
    stfs f1, 0x4(r31)
    stfs f0, 0x8(r31)
    @L_801A6358:
    lwz r0, 0x34(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
