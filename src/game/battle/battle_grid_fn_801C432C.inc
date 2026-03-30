    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    lis r3, lbl_80466E30@ha
    li r0, 0x0
    addi r3, r3, lbl_80466E30@l
    lwz r31, 0x10(r3)
    stw r0, 0x10(r3)
    bl fn_80109708
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    bne @L_801C4368
    bl fn_80109710
    mr r31, r3
    @L_801C4368:
    cmplwi r31, 0x0
    bne @L_801C438C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    li r7, 0x0
    bl fn_800EF5FC
    mr r31, r3
    @L_801C438C:
    cmplwi r31, 0x0
    beq @L_801C43D0
    li r0, 0x0
    lis r3, fn_801C43E4@ha
    stb r0, lbl_8047B3A8(r13)
    addi r4, r3, fn_801C43E4@l
    mr r3, r31
    li r5, 0x0
    bl fn_800DC390
    b @L_801C43B8
    @L_801C43B4:
    bl fn_800F0308
    @L_801C43B8:
    lbz r0, lbl_8047B3A8(r13)
    cmplwi r0, 0x0
    beq @L_801C43B4
    lis r3, lbl_80466E30@ha
    addi r3, r3, lbl_80466E30@l
    stw r31, 0x10(r3)
    @L_801C43D0:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
