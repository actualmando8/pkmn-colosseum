    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stw r31, 0x9c(r1)
    mr. r31, r3
    fmr f30, f1
    beq @L_80194C08
    lwz r0, 0x8(r31)
    clrlwi. r0, r0, 31
    beq @L_80194BEC
    cmplwi r31, 0x0
    beq @L_80194820
    lwz r0, 0x24(r31)
    cmplwi r0, 0x0
    beq @L_80194820
    lwz r0, 0x28(r31)
    cmplwi r0, 0x0
    bne @L_80194828
@L_80194820:
    li r0, 0x0
    b @L_80194864
@L_80194828:
    mr r3, r31
    addi r4, r1, 0x14
    bl HSD_CObjGetEyePosition
    mr r3, r31
    addi r4, r1, 0x20
    bl HSD_CObjGetInterest
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    addi r5, r1, 0x38
    bl fn_800A3A9C
    addi r3, r1, 0x38
    addi r4, r1, 0x38
    bl fn_80194C2C
    cntlzw r0, r3
    srwi r0, r0, 5
@L_80194864:
    cmpwi r0, 0x0
    beq @L_80194A8C
    lfs f1, 0x3c(r1)
    bl fn_800CE59C
    frsp f1, f1
    lfd f2, -23832(r2)
    lfd f0, -23824(r2)
    fsub f1, f2, f1
    fcmpo cr0, f1, f0
    bge @L_80194978
    lfs f1, 0x40(r1)
    lfs f0, 0x40(r1)
    lfs f3, 0x3c(r1)
    fmuls f1, f1, f0
    lfs f2, 0x3c(r1)
    lfs f0, -23848(r2)
    fmadds f31, f3, f2, f1
    fcmpo cr0, f31, f0
    ble @L_80194908
    frsqrte f7, f31
    lfd f6, -23816(r2)
    lfd f3, -23808(r2)
    lfd f4, -23816(r2)
    fmul f5, f7, f7
    lfd f1, -23808(r2)
    lfd f2, -23816(r2)
    fmul f6, f6, f7
    lfd f0, -23808(r2)
    fnmsub f3, f31, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f31, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f31, f1, f0
    fmul f7, f2, f0
    fmul f31, f31, f7
    frsp f31, f31
    b @L_80194938
@L_80194908:
    lfd f0, -23800(r2)
    fcmpo cr0, f31, f0
    bge @L_80194920
    lis r3, lbl_80478AC0@ha
    lfs f31, lbl_80478AC0@l(r3)
    b @L_80194938
@L_80194920:
    fmr f1, f31
    bl fn_800CE718
    cmpwi r3, 0x1
    bne @L_80194938
    lis r3, lbl_80478AC0@ha
    lfs f31, lbl_80478AC0@l(r3)
@L_80194938:
    lfs f1, 0x38(r1)
    stfs f31, 0x44(r1)
    lfs f0, 0x38(r1)
    fneg f3, f1
    lfs f2, 0x44(r1)
    fneg f1, f0
    lfs f0, 0x44(r1)
    fdivs f3, f3, f2
    lfs f4, 0x3c(r1)
    lfs f2, 0x40(r1)
    fdivs f0, f1, f0
    fmuls f1, f4, f3
    fmuls f0, f2, f0
    stfs f1, 0x48(r1)
    stfs f0, 0x4c(r1)
    b @L_80194A60
@L_80194978:
    lfs f1, 0x40(r1)
    lfs f0, 0x40(r1)
    lfs f3, 0x38(r1)
    fmuls f1, f1, f0
    lfs f2, 0x38(r1)
    lfs f0, -23848(r2)
    fmadds f31, f3, f2, f1
    fcmpo cr0, f31, f0
    ble @L_801949F4
    frsqrte f7, f31
    lfd f6, -23816(r2)
    lfd f3, -23808(r2)
    lfd f4, -23816(r2)
    fmul f5, f7, f7
    lfd f1, -23808(r2)
    lfd f2, -23816(r2)
    fmul f6, f6, f7
    lfd f0, -23808(r2)
    fnmsub f3, f31, f5, f3
    fmul f7, f6, f3
    fmul f3, f7, f7
    fmul f4, f4, f7
    fnmsub f1, f31, f3, f1
    fmul f7, f4, f1
    fmul f1, f7, f7
    fmul f2, f2, f7
    fnmsub f0, f31, f1, f0
    fmul f7, f2, f0
    fmul f31, f31, f7
    frsp f31, f31
    b @L_80194A24
@L_801949F4:
    lfd f0, -23800(r2)
    fcmpo cr0, f31, f0
    bge @L_80194A0C
    lis r3, lbl_80478AC0@ha
    lfs f31, lbl_80478AC0@l(r3)
    b @L_80194A24
@L_80194A0C:
    fmr f1, f31
    bl fn_800CE718
    cmpwi r3, 0x1
    bne @L_80194A24
    lis r3, lbl_80478AC0@ha
    lfs f31, lbl_80478AC0@l(r3)
@L_80194A24:
    lfs f1, 0x3c(r1)
    stfs f31, 0x48(r1)
    lfs f0, 0x3c(r1)
    fneg f3, f1
    lfs f2, 0x48(r1)
    fneg f1, f0
    lfs f0, 0x48(r1)
    fdivs f3, f3, f2
    lfs f4, 0x38(r1)
    lfs f2, 0x40(r1)
    fdivs f0, f1, f0
    fmuls f1, f4, f3
    fmuls f0, f2, f0
    stfs f1, 0x44(r1)
    stfs f0, 0x4c(r1)
@L_80194A60:
    fneg f1, f30
    addi r3, r1, 0x68
    addi r4, r1, 0x38
    bl fn_800A3244
    addi r3, r1, 0x68
    addi r4, r1, 0x44
    addi r5, r1, 0x50
    bl fn_800A3820
    addi r3, r1, 0x50
    addi r4, r1, 0x5c
    bl fn_800A3ADC
@L_80194A8C:
    cmplwi r31, 0x0
    beq @L_80194C08
    lwz r0, 0x8(r31)
    clrlwi. r0, r0, 31
    beq @L_80194B8C
    lfs f1, 0x5c(r1)
    bl fn_800CE59C
    lis r3, lbl_80478AC8@ha
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194AFC
    lfs f1, 0x60(r1)
    bl fn_800CE59C
    lis r3, lbl_80478AC8@ha
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194AFC
    lfs f1, 0x64(r1)
    bl fn_800CE59C
    lis r3, lbl_80478AC8@ha
    lfs f0, lbl_80478AC8@l(r3)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_80194AFC
    li r0, -0x1
    b @L_80194B0C
@L_80194AFC:
    addi r3, r1, 0x5c
    addi r4, r1, 0x2c
    bl fn_800A3ADC
    li r0, 0x0
@L_80194B0C:
    cmpwi r0, 0x0
    beq @L_80194B34
    lis r3, lbl_8027464C@ha
    addi r3, r3, lbl_8027464C@l
    crxor 6, 6, 6
    bl OSReport
    la r3, -23880(r2)
    li r4, 0x3e4
    la r5, -23864(r2)
    bl __assert
@L_80194B34:
    lfs f1, 0x2c(r31)
    lfs f0, 0x2c(r1)
    fcmpu cr0, f1, f0
    bne @L_80194B64
    lfs f1, 0x30(r31)
    lfs f0, 0x30(r1)
    fcmpu cr0, f1, f0
    bne @L_80194B64
    lfs f1, 0x34(r31)
    lfs f0, 0x34(r1)
    fcmpu cr0, f1, f0
    beq @L_80194C08
@L_80194B64:
    lwz r0, 0x8(r31)
    oris r0, r0, 0xc000
    stw r0, 0x8(r31)
    lwz r3, 0x2c(r1)
    lwz r0, 0x30(r1)
    stw r3, 0x2c(r31)
    stw r0, 0x30(r31)
    lwz r0, 0x34(r1)
    stw r0, 0x34(r31)
    b @L_80194C08
@L_80194B8C:
    mr r3, r31
    addi r4, r1, 0x5c
    bl fn_80195590
    cmplwi r31, 0x0
    fmr f30, f1
    beq @L_80194C08
    lwz r0, 0x8(r31)
    clrlwi. r0, r0, 31
    beq @L_80194BD0
    fmr f1, f30
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_8019513C
    mr r3, r31
    addi r4, r1, 0x8
    bl fn_80194DA4
    b @L_80194C08
@L_80194BD0:
    lfs f0, 0x2c(r31)
    fcmpu cr0, f0, f30
    beq @L_80194BE4
    mr r3, r31
    bl HSD_CObjSetMtxDirty
@L_80194BE4:
    stfs f30, 0x2c(r31)
    b @L_80194C08
@L_80194BEC:
    lfs f0, 0x2c(r31)
    fcmpu cr0, f0, f30
    beq @L_80194C04
    lwz r0, 0x8(r31)
    oris r0, r0, 0xc000
    stw r0, 0x8(r31)
@L_80194C04:
    stfs f30, 0x2c(r31)
@L_80194C08:
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    lwz r0, 0xc4(r1)
    lwz r31, 0x9c(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
