    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r4
    stw r30, 0x8(r1)
    mr. r30, r3
    lis r3, lbl_80466DE8@ha
    addi r0, r3, lbl_80466DE8@l
    mr r3, r0
    beq @L_801C3F44
    cmplwi r31, 0x0
    bne @L_801C3F58
    @L_801C3F44:
    lis r3, lbl_80275830@ha
    addi r3, r3, lbl_80275830@l
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_801C3FA4
    @L_801C3F58:
    li r4, 0x0
    b @L_801C3F98
    @L_801C3F60:
    lwz r0, 0x0(r3)
    cmplw r0, r30
    bne @L_801C3F90
    stw r31, 0x0(r3)
    mr r3, r30
    li r4, 0x0
    bl fn_801DA4E8
    lbz r3, 0x76(r30)
    li r0, 0x0
    stb r3, 0x76(r31)
    stb r0, 0x76(r30)
    b @L_801C3FA4
    @L_801C3F90:
    addi r4, r4, 0x1
    addi r3, r3, 0x10
    @L_801C3F98:
    clrlwi r0, r4, 16
    cmplwi r0, 0x4
    blt @L_801C3F60
    @L_801C3FA4:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
