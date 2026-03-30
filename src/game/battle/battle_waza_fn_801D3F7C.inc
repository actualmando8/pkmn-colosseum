    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    stw r0, 0xa4(r1)
    stfd f31, 0x90(r1)
    psq_st f31, 0x98(r1), 0, 0
    stfd f30, 0x80(r1)
    psq_st f30, 0x88(r1), 0, 0
    stfd f29, 0x70(r1)
    psq_st f29, 0x78(r1), 0, 0
    stfd f28, 0x60(r1)
    psq_st f28, 0x68(r1), 0, 0
    stmw r27, 0x4c(r1)
    lwz r0, 0x0(r3)
    mr r31, r4
    lfs f28, -21672(r2)
    mr r27, r6
    rlwinm r0, r0, 2, 0, 29
    lfs f29, -21568(r2)
    add r3, r3, r0
    lwz r29, 0x4(r3)
    cmpwi r29, 0x0
    bne @L_801D3FD8
    lwz r29, 0x8(r3)
@L_801D3FD8:
    slw r29, r29, r5
    bl fn_800E0BE4
    lfs f0, -21564(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801D4000
    lfs f28, -21560(r2)
    li r28, 0x1
    lfs f29, -21556(r2)
    b @L_801D4004
@L_801D4000:
    li r28, 0x0
@L_801D4004:
    bl fn_800E0BE4
    fcmpo cr0, f1, f28
    bge @L_801D4018
    li r30, 0x0
    b @L_801D4060
@L_801D4018:
    fcmpo cr0, f1, f29
    bge @L_801D4044
    mr r3, r29
    li r30, 0x0
    bl fn_800E0C04
    srawi r0, r29, 1
    mr r29, r3
    cmpw r29, r0
    bge @L_801D4060
    mr r29, r0
    b @L_801D4060
@L_801D4044:
    mr r3, r29
    bl fn_800E0C04
    srawi r0, r29, 1
    mr r30, r3
    cmpw r30, r0
    ble @L_801D4060
    mr r30, r0
@L_801D4060:
    lfs f28, 0x20(r31)
    bl fn_800E0BE4
    lfs f0, 0x24(r31)
    lfs f29, 0x20(r31)
    fsubs f0, f0, f28
    fmadds f28, f0, f1, f28
    fmr f31, f28
    bl fn_800E0BE4
    lfs f0, 0x24(r31)
    rlwinm. r0, r28, 0, 24, 31
    fsubs f0, f0, f29
    fmadds f0, f0, f1, f29
    fmr f30, f0
    beq @L_801D40C4
    fcmpo cr0, f28, f0
    bge @L_801D40A8
    fmr f30, f28
    fmr f31, f0
@L_801D40A8:
    lfs f28, 0x18(r31)
    bl fn_800E0BE4
    lfs f0, 0x1c(r31)
    fsubs f0, f0, f28
    fmadds f0, f0, f1, f28
    fmr f29, f0
    b @L_801D40EC
@L_801D40C4:
    fcmpo cr0, f28, f0
    ble @L_801D40D4
    fmr f30, f28
    fmr f31, f0
@L_801D40D4:
    lfs f28, 0x18(r31)
    bl fn_800E0BE4
    lfs f0, 0x1c(r31)
    fsubs f0, f0, f28
    fmadds f0, f0, f1, f28
    fmr f29, f0
@L_801D40EC:
    rlwinm. r0, r27, 0, 24, 31
    beq @L_801D4114
    lfs f28, 0xc(r31)
    bl fn_800E0BE4
    lfs f2, 0x10(r31)
    lfs f0, 0x14(r31)
    fsubs f2, f2, f28
    fsubs f0, f0, f28
    fnmsubs f28, f2, f1, f0
    b @L_801D4130
@L_801D4114:
    lfs f28, 0xc(r31)
    bl fn_800E0BE4
    lfs f2, 0x10(r31)
    lfs f0, 0x14(r31)
    fsubs f2, f2, f28
    fadds f0, f0, f28
    fmadds f28, f2, f1, f0
@L_801D4130:
    fmr f1, f31
    bl fn_801767E0
    fmr f1, f29
    bl fn_80176868
    fmr f1, f28
    bl fn_80176758
    bl fn_80177A64
    addi r3, r1, 0x20
    bl fn_801778B4
    addi r3, r1, 0x14
    bl fn_80177908
    addi r3, r1, 0x20
    addi r5, r1, 0x14
    mr r4, r3
    bl fn_800E0168
    fdivs f1, f30, f31
    lfs f0, -21668(r2)
    addi r3, r1, 0x20
    stfs f0, 0x24(r1)
    mr r4, r3
    bl fn_800E013C
    addi r3, r1, 0x20
    stfs f29, 0x24(r1)
    mr r4, r3
    addi r5, r1, 0x14
    bl fn_800E019C
    bl fn_800D37CC
    lis r5, 0x4330
    xoris r3, r3, 0x8000
    subf r0, r30, r29
    stw r3, 0x34(r1)
    xoris r0, r0, 0x8000
    lfd f2, -21576(r2)
    stw r5, 0x30(r1)
    addi r4, r1, 0x20
    li r3, 0x7
    lfd f0, 0x30(r1)
    stw r0, 0x3c(r1)
    fsubs f1, f0, f2
    stw r5, 0x38(r1)
    lfd f0, 0x38(r1)
    fsubs f0, f0, f2
    fdivs f1, f0, f1
    bl fn_80177574
    fmuls f0, f29, f29
    lfs f1, -21668(r2)
    fmadds f5, f31, f31, f0
    fcmpo cr0, f5, f1
    ble @L_801D423C
    frsqrte f2, f5
    lfd f4, -21640(r2)
    lfd f3, -21632(r2)
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f1, f2, f1
    fmul f5, f5, f1
    frsp f5, f5
    b @L_801D42C0
@L_801D423C:
    lfd f1, -21624(r2)
    fcmpo cr0, f5, f1
    bge @L_801D4254
    lis r3, -32696
    lfs f5, -30016(r3)
    b @L_801D42C0
@L_801D4254:
    stfs f5, 0x10(r1)
    lis r0, 0x7f80
    lwz r4, 0x10(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_801D427C
    bge @L_801D42AC
    cmpwi r3, 0x0
    beq @L_801D4294
    b @L_801D42AC
@L_801D427C:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D428C
    li r0, 0x1
    b @L_801D42B0
@L_801D428C:
    li r0, 0x2
    b @L_801D42B0
@L_801D4294:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D42A4
    li r0, 0x5
    b @L_801D42B0
@L_801D42A4:
    li r0, 0x3
    b @L_801D42B0
@L_801D42AC:
    li r0, 0x4
@L_801D42B0:
    cmpwi r0, 0x1
    bne @L_801D42C0
    lis r3, -32696
    lfs f5, -30016(r3)
@L_801D42C0:
    fadds f2, f31, f30
    lfs f3, -21672(r2)
    lfs f1, -21668(r2)
    stfs f5, 0x28(r31)
    fmuls f2, f3, f2
    fmadds f5, f2, f2, f0
    fcmpo cr0, f5, f1
    ble @L_801D4328
    frsqrte f2, f5
    lfd f4, -21640(r2)
    lfd f3, -21632(r2)
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f2, f2, f1
    fmul f1, f2, f2
    fmul f2, f4, f2
    fnmsub f1, f5, f1, f3
    fmul f1, f2, f1
    fmul f5, f5, f1
    frsp f5, f5
    b @L_801D43AC
@L_801D4328:
    lfd f1, -21624(r2)
    fcmpo cr0, f5, f1
    bge @L_801D4340
    lis r3, -32696
    lfs f5, -30016(r3)
    b @L_801D43AC
@L_801D4340:
    stfs f5, 0xc(r1)
    lis r0, 0x7f80
    lwz r4, 0xc(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_801D4368
    bge @L_801D4398
    cmpwi r3, 0x0
    beq @L_801D4380
    b @L_801D4398
@L_801D4368:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D4378
    li r0, 0x1
    b @L_801D439C
@L_801D4378:
    li r0, 0x2
    b @L_801D439C
@L_801D4380:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D4390
    li r0, 0x5
    b @L_801D439C
@L_801D4390:
    li r0, 0x3
    b @L_801D439C
@L_801D4398:
    li r0, 0x4
@L_801D439C:
    cmpwi r0, 0x1
    bne @L_801D43AC
    lis r3, -32696
    lfs f5, -30016(r3)
@L_801D43AC:
    fmadds f4, f30, f30, f0
    lfs f0, -21668(r2)
    stfs f5, 0x2c(r31)
    fcmpo cr0, f4, f0
    ble @L_801D4408
    frsqrte f1, f4
    lfd f3, -21640(r2)
    lfd f2, -21632(r2)
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f1, f1, f0
    fmul f0, f1, f1
    fmul f1, f3, f1
    fnmsub f0, f4, f0, f2
    fmul f0, f1, f0
    fmul f4, f4, f0
    frsp f4, f4
    b @L_801D448C
@L_801D4408:
    lfd f0, -21624(r2)
    fcmpo cr0, f4, f0
    bge @L_801D4420
    lis r3, -32696
    lfs f4, -30016(r3)
    b @L_801D448C
@L_801D4420:
    stfs f4, 0x8(r1)
    lis r0, 0x7f80
    lwz r4, 0x8(r1)
    rlwinm r3, r4, 0, 1, 8
    cmpw r3, r0
    beq @L_801D4448
    bge @L_801D4478
    cmpwi r3, 0x0
    beq @L_801D4460
    b @L_801D4478
@L_801D4448:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D4458
    li r0, 0x1
    b @L_801D447C
@L_801D4458:
    li r0, 0x2
    b @L_801D447C
@L_801D4460:
    rlwinm. r0, r4, 0, 9, 31
    beq @L_801D4470
    li r0, 0x5
    b @L_801D447C
@L_801D4470:
    li r0, 0x3
    b @L_801D447C
@L_801D4478:
    li r0, 0x4
@L_801D447C:
    cmpwi r0, 0x1
    bne @L_801D448C
    lis r3, -32696
    lfs f4, -30016(r3)
@L_801D448C:
    stfs f4, 0x30(r31)
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    psq_l f29, 0x78(r1), 0, 0
    lfd f29, 0x70(r1)
    psq_l f28, 0x68(r1), 0, 0
    lfd f28, 0x60(r1)
    lmw r27, 0x4c(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
