    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stw r31, 0x7c(r1)
    mr r31, r4
    stw r30, 0x78(r1)
    mr. r30, r3
    beq @L_801950B8
    cmplwi r31, 0x0
    bne @L_80194DD0
    b @L_801950B8
@L_80194DD0:
    lwz r0, 0x8(r30)
    clrlwi. r0, r0, 31
    beq @L_80194EDC
    cmplwi r31, 0x0
    beq @L_80194DE8
    b @L_80194DF0
@L_80194DE8:
    li r0, -0x1
    b @L_80194E5C
@L_80194DF0:
    lfs f0, 0x0(r31)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194E4C
    lfs f0, 0x4(r31)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194E4C
    lfs f0, 0x8(r31)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194E4C
    li r0, -0x1
    b @L_80194E5C
@L_80194E4C:
    mr r3, r31
    addi r4, r1, 0x38
    bl fn_800A3ADC
    li r0, 0x0
@L_80194E5C:
    cmpwi r0, 0x0
    beq @L_80194E84
    lis r3, lbl_8027464C@ha
    addi r3, r3, lbl_8027464C@l
    crxor 6, 6, 6
    bl OSReport
    la r3, -23880(r2)
    li r4, 0x3e4
    la r5, -23864(r2)
    bl __assert
@L_80194E84:
    lfs f1, 0x2c(r30)
    lfs f0, 0x38(r1)
    fcmpu cr0, f1, f0
    bne @L_80194EB4
    lfs f1, 0x30(r30)
    lfs f0, 0x3c(r1)
    fcmpu cr0, f1, f0
    bne @L_80194EB4
    lfs f1, 0x34(r30)
    lfs f0, 0x40(r1)
    fcmpu cr0, f1, f0
    beq @L_801950B8
@L_80194EB4:
    lwz r0, 0x8(r30)
    oris r0, r0, 0xc000
    stw r0, 0x8(r30)
    lwz r3, 0x38(r1)
    lwz r0, 0x3c(r1)
    stw r3, 0x2c(r30)
    stw r0, 0x30(r30)
    lwz r0, 0x40(r1)
    stw r0, 0x34(r30)
    b @L_801950B8
@L_80194EDC:
    cmplwi r30, 0x0
    beq @L_80194EFC
    lwz r0, 0x24(r30)
    cmplwi r0, 0x0
    beq @L_80194EFC
    lwz r0, 0x28(r30)
    cmplwi r0, 0x0
    bne @L_80194F04
@L_80194EFC:
    li r0, 0x0
    b @L_80195000
@L_80194F04:
    cmplwi r30, 0x0
    bne @L_80194F1C
    la r3, -23880(r2)
    li r4, 0x318
    la r5, -23872(r2)
    bl __assert
@L_80194F1C:
    cmplwi r30, 0x0
    bne @L_80194F34
    la r3, -23880(r2)
    li r4, 0x2e8
    la r5, -23872(r2)
    bl __assert
@L_80194F34:
    lwz r3, 0x24(r30)
    addi r4, r1, 0x8
    bl fn_80191688
    cmplwi r30, 0x0
    bne @L_80194F58
    la r3, -23880(r2)
    li r4, 0x300
    la r5, -23872(r2)
    bl __assert
@L_80194F58:
    cmplwi r30, 0x0
    bne @L_80194F70
    la r3, -23880(r2)
    li r4, 0x2d0
    la r5, -23872(r2)
    bl __assert
@L_80194F70:
    lwz r3, 0x28(r30)
    addi r4, r1, 0x14
    bl fn_80191688
    addi r3, r1, 0x14
    addi r4, r1, 0x8
    addi r5, r1, 0x2c
    bl fn_800A3A9C
    lfs f0, 0x2c(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194FE8
    lfs f0, 0x30(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194FE8
    lfs f0, 0x34(r1)
    lis r3, lbl_80478AC8@ha
    fabs f1, f0
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194FE8
    li r0, -0x1
    b @L_80194FF8
@L_80194FE8:
    addi r3, r1, 0x2c
    addi r4, r1, 0x2c
    bl fn_800A3ADC
    li r0, 0x0
@L_80194FF8:
    cntlzw r0, r0
    srwi r0, r0, 5
@L_80195000:
    cmpwi r0, 0x0
    bne @L_80195010
    lfs f1, -23848(r2)
    b @L_801950B0
@L_80195010:
    mr r3, r31
    addi r4, r1, 0x2c
    bl fn_800A3B7C
    fabs f1, f1
    lis r3, lbl_80478AC8@ha
    lfs f2, -23792(r2)
    lfs f0, lbl_80478AC8@l(r3)
    frsp f1, f1
    fsubs f1, f2, f1
    fcmpo cr0, f1, f0
    bge @L_80195044
    lfs f1, -23848(r2)
    b @L_801950B0
@L_80195044:
    lis r3, lbl_8036C6BC@ha
    lis r5, lbl_8036C6C8@ha
    addi r4, r3, lbl_8036C6BC@l
    addi r6, r1, 0x2c
    addi r3, r1, 0x44
    addi r5, r5, lbl_8036C6C8@l
    bl C_MTXLookAt
    mr r4, r31
    addi r3, r1, 0x44
    addi r5, r1, 0x20
    bl fn_800A3820
    lfs f1, 0x20(r1)
    lfs f2, 0x24(r1)
    lfs f0, -23848(r2)
    fneg f1, f1
    fcmpu cr0, f0, f2
    bne @L_801950A8
    lfs f0, -23848(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801950A0
    lfs f1, -23788(r2)
    b @L_801950B0
@L_801950A0:
    lfs f1, -23784(r2)
    b @L_801950B0
@L_801950A8:
    bl fn_800CE2D8
    frsp f1, f1
@L_801950B0:
    mr r3, r30
    bl fn_801947C8
@L_801950B8:
    lwz r0, 0x84(r1)
    lwz r31, 0x7c(r1)
    lwz r30, 0x78(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
