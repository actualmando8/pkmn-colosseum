    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r30, r3
    bl fn_801DAC54
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801DF31C
    lbz r0, 0x18(r30)
    rlwinm r0, r0, 0, 28, 28
    cmpwi r0, 0x8
    beq @L_801DF31C
    lbz r0, 0x19(r30)
    cmplwi r0, 0x0
    beq @L_801DF224
    li r0, 0x0
    sth r0, 0x20(r30)
    b @L_801DF31C
@L_801DF224:
    lha r0, 0x1c(r30)
    cmpwi r0, 0x0
    blt @L_801DF31C
    bl fn_800D3088
    lhz r0, 0x20(r30)
    rlwinm r3, r3, 0, 16, 31
    add r0, r0, r3
    sth r0, 0x20(r30)
    lhz r3, 0x20(r30)
    cmplwi r3, 0xa
    blt @L_801DF31C
    cmplwi r3, 0x3c
    blt @L_801DF270
    cmplwi r3, 0xb4
    blt @L_801DF268
    lfs f31, -21192(r2)
    b @L_801DF2AC
@L_801DF268:
    lfs f31, -21188(r2)
    b @L_801DF2AC
@L_801DF270:
    subi r3, r3, 0xa
    lis r0, 0x4330
    xoris r3, r3, 0x8000
    stw r0, 0x8(r1)
    lfd f3, -21176(r2)
    stw r3, 0xc(r1)
    lfs f2, -21184(r2)
    lfd f0, 0x8(r1)
    lfs f1, -21180(r2)
    fsubs f3, f0, f3
    lfs f0, -21188(r2)
    fdivs f2, f3, f2
    fsubs f1, f1, f2
    fmuls f31, f2, f1
    fmuls f31, f31, f0
@L_801DF2AC:
    bl fn_800E0BE4
    fcmpo cr0, f31, f1
    cror eq, gt, eq
    bne @L_801DF31C
    lwz r31, 0x24(r30)
    cmplwi r31, 0x0
    beq @L_801DF31C
    li r0, 0x3
    mr r3, r31
    stb r0, 0x19(r30)
    li r4, 0x0
    bl fn_800EC188
    lha r4, 0x1c(r30)
    mr r3, r31
    bl fn_800EC35C
    lfs f1, -21208(r2)
    mr r3, r31
    bl fn_800EC308
    mr r3, r31
    li r4, 0x0
    bl fn_800EC208
    lfs f1, -21204(r2)
    mr r3, r31
    bl fn_800EC2A4
    mr r3, r31
    bl fn_800EC1E4
    li r0, 0x0
    sth r0, 0x20(r30)
@L_801DF31C:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
