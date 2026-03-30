    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stfd f31, 0x40(r1)
    psq_st f31, 0x48(r1), 0, 0
    stfd f30, 0x30(r1)
    psq_st f30, 0x38(r1), 0, 0
    stw r31, 0x2c(r1)
    stw r30, 0x28(r1)
    stw r29, 0x24(r1)
    lwz r5, 0x0(r4)
    mr r29, r3
    rlwinm r0, r5, 0, 30, 31
    cmplwi r0, 0x2
    bne @L_801DEDE8
    rlwinm r0, r5, 0, 29, 29
    cmplwi r0, 0x4
    beq @L_801DEDE8
    lwz r31, 0x8(r4)
    cmplwi r31, 0x0
    beq @L_801DEDE8
    lbz r0, 0x18(r31)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x8
    beq @L_801DEDE8
    lwz r0, 0x24(r31)
    cmplw r29, r0
    bne @L_801DEDE8
    lwz r3, 0x4(r4)
    stw r3, 0xc(r1)
    lha r0, 0x1a(r31)
    cmpw r3, r0
    beq @L_801DED00
    lha r0, 0x1e(r31)
    cmpw r3, r0
    beq @L_801DED00
    lha r0, 0x1c(r31)
    cmpw r3, r0
    bne @L_801DEDE8
@L_801DED00:
    lbz r0, 0x19(r31)
    cmpwi r0, 0x1
    beq @L_801DEDCC
    blt @L_801DEDD8
    cmpwi r0, 0x4
    bge @L_801DEDD8
    mr r3, r29
    li r4, 0x1
    bl fn_800EC188
    mr r3, r29
    bl fn_800EC960
    rlwinm. r0, r3, 0, 24, 31
    beq @L_801DEDC0
    mr r3, r29
    bl fn_800EC53C
    fmr f31, f1
    mr r3, r29
    bl fn_800EC570
    fmr f30, f1
    mr r3, r29
    bl fn_800EC568
    mr r0, r3
    mr r3, r29
    mr r30, r0
    addi r4, r1, 0xc
    addi r5, r1, 0x8
    bl fn_800EC578
    lwz r4, 0xc(r1)
    mr r3, r29
    bl fn_800EC35C
    bl fn_800D3088
    lis r0, 0x4330
    stw r3, 0x14(r1)
    lfd f1, -21200(r2)
    mr r3, r29
    stw r0, 0x10(r1)
    lfd f0, 0x10(r1)
    fsubs f0, f0, f1
    fmadds f1, f30, f0, f31
    bl fn_800EC2A4
    fmr f1, f30
    mr r3, r29
    bl fn_800EC308
    mr r3, r29
    mr r4, r30
    bl fn_800EC208
    mr r3, r29
    bl fn_800EC1E4
@L_801DEDC0:
    li r0, 0x0
    stb r0, 0x19(r31)
    b @L_801DEDE8
@L_801DEDCC:
    li r0, 0x6
    stb r0, 0x19(r31)
    b @L_801DEDE8
@L_801DEDD8:
    lis r3, -32728
    addi r3, r3, -26152
    crxor 6, 6, 6
    bl fn_800DD970
@L_801DEDE8:
    psq_l f31, 0x48(r1), 0, 0
    lfd f31, 0x40(r1)
    psq_l f30, 0x38(r1), 0, 0
    lfd f30, 0x30(r1)
    lwz r31, 0x2c(r1)
    lwz r30, 0x28(r1)
    lwz r0, 0x54(r1)
    lwz r29, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
