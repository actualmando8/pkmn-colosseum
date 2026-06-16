    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stw r31, 0x6c(r1)
    mr r31, r4
    stw r30, 0x68(r1)
    mr. r30, r3
    beq @L_801955C8
    lwz r0, 0x24(r30)
    cmplwi r0, 0x0
    beq @L_801955C8
    lwz r0, 0x28(r30)
    cmplwi r0, 0x0
    bne @L_801955D0
@L_801955C8:
    li r0, 0x0
    b @L_801956CC
@L_801955D0:
    cmplwi r30, 0x0
    bne @L_801955E8
    la r3, -23880(r2)
    li r4, 0x318
    la r5, -23872(r2)
    bl __assert
@L_801955E8:
    cmplwi r30, 0x0
    bne @L_80195600
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_80195600:
    lwz r3, 0x24(r30)
    addi r4, r1, 0x8
    bl fn_80191688
    cmplwi r30, 0x0
    bne @L_80195624
    la r3, -23880(r2)
    li r4, 0x300
    la r5, -23872(r2)
    bl __assert
@L_80195624:
    cmplwi r30, 0x0
    bne @L_8019563C
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_8019563C:
    lwz r3, 0x28(r30)
    addi r4, r1, 0x14
    bl fn_80191688
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    addi r5, r1, 0x20
    bl fn_800A3A9C
    lfs f0, 0x20(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801956B4
    lfs f0, 0x24(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801956B4
    lfs f0, 0x28(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801956B4
    li r0, -0x1
    b @L_801956C4
@L_801956B4:
    addi r3, r1, 0x20
    addi r4, r1, 0x20
    bl fn_800A3ADC
    li r0, 0x0
@L_801956C4:
    cntlzw r0, r0
    srwi r0, r0, 5
@L_801956CC:
    cmpwi r0, 0x0
    bne @L_801956DC
    lfs f1, -23848(r2)
    b @L_8019577C
@L_801956DC:
    mr r3, r31
    addi r4, r1, 0x20
    bl fn_800A3B7C
    fabs f1, f1
    lis r3, lbl_80478AC8@ha
    lfs f2, -23792(r2)
    lfs f0, lbl_80478AC8@l(r3)
    frsp f1, f1
    fsubs f1, f2, f1
    fcmpo cr0, f1, f0
    bge @L_80195710
    lfs f1, -23848(r2)
    b @L_8019577C
@L_80195710:
    lis r3, lbl_8036C6BC@ha
    lis r5, lbl_8036C6C8@ha
    addi r4, r3, lbl_8036C6BC@l
    addi r6, r1, 0x20
    addi r3, r1, 0x38
    addi r5, r5, lbl_8036C6C8@l
    bl C_MTXLookAt
    mr r4, r31
    addi r3, r1, 0x38
    addi r5, r1, 0x2c
    bl fn_800A3820
    lfs f1, 0x2c(r1)
    lfs f2, 0x30(r1)
    lfs f0, -23848(r2)
    fneg f1, f1
    fcmpu cr0, f0, f2
    bne @L_80195774
    lfs f0, -23848(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8019576C
    lfs f1, -23788(r2)
    b @L_8019577C
@L_8019576C:
    lfs f1, -23784(r2)
    b @L_8019577C
@L_80195774:
    bl fn_800CE2D8
    frsp f1, f1
@L_8019577C:
    lwz r0, 0x74(r1)
    lwz r31, 0x6c(r1)
    lwz r30, 0x68(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
