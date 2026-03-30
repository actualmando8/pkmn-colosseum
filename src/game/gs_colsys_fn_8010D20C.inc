    nofralloc
    stwu r1, -0xb0(r1)
    mflr r0
    stw r0, 0xb4(r1)
    stfd f31, 0xa0(r1)
    psq_st f31, 0xa8(r1), 0, 0
    stfd f30, 0x90(r1)
    psq_st f30, 0x98(r1), 0, 0
    stfd f29, 0x80(r1)
    psq_st f29, 0x88(r1), 0, 0
    stfd f28, 0x70(r1)
    psq_st f28, 0x78(r1), 0, 0
    stfd f27, 0x60(r1)
    psq_st f27, 0x68(r1), 0, 0
    stmw r25, 0x44(r1)
    mr r29, r3
    mr r30, r4
    li r3, 0x3
    bl fn_800D6A00
    lwz r31, 0x0(r29)
    li r26, 0x0
    lfs f27, -26580(r2)
    lis r28, 0x4330
    lfd f28, -26568(r2)
    lfs f29, -26576(r2)
    lfs f30, -26584(r2)
    lfs f31, -26572(r2)
    b @L_8010D380
@L_8010D278:
    addi r3, r1, 0x8
    li r4, 0x0
    li r5, 0x4
    bl memset
    li r0, 0xc0
    stw r28, 0x18(r1)
    stb r0, 0xb(r1)
    lbz r3, 0x30(r31)
    rlwinm r0, r3, 28, 28, 31
    rlwinm r3, r3, 0, 28, 31
    xoris r0, r0, 0x8000
    stw r0, 0x1c(r1)
    addi r3, r3, 0x1
    cmpwi r3, 0x10
    lfd f0, 0x18(r1)
    fsubs f0, f0, f28
    fdivs f0, f0, f29
    fmadds f0, f27, f0, f30
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r0, 0x24(r1)
    stb r0, 0x9(r1)
    blt @L_8010D2D8
    li r3, 0x0
@L_8010D2D8:
    xoris r0, r3, 0x8000
    stw r28, 0x28(r1)
    stw r0, 0x2c(r1)
    lfd f0, 0x28(r1)
    fsubs f0, f0, f28
    fdivs f0, f0, f29
    fmuls f0, f31, f0
    fctiwz f0, f0
    stfd f0, 0x30(r1)
    lwz r0, 0x34(r1)
    stb r0, 0xa(r1)
    lbz r0, 0x31(r31)
    rlwinm. r0, r0, 28, 28, 31
    ble @L_8010D31C
    rlwinm r3, r0, 2, 0, 29
    addi r0, r3, 0xc0
    stb r0, 0x8(r1)
@L_8010D31C:
    li r3, 0x3
    bl fn_800D67BC
    li r25, 0x0
    mr r27, r31
@L_8010D32C:
    mr r3, r30
    mr r4, r27
    addi r5, r1, 0xc
    bl fn_800A37CC
    lfs f1, 0xc(r1)
    lfs f2, 0x10(r1)
    lfs f3, 0x14(r1)
    bl fn_800D6680
    lbz r4, 0x8(r1)
    li r3, 0x0
    lbz r5, 0x9(r1)
    lbz r6, 0xa(r1)
    lbz r7, 0xb(r1)
    bl fn_800D5CB8
    addi r25, r25, 0x1
    addi r27, r27, 0xc
    cmpwi r25, 0x3
    blt @L_8010D32C
    bl fn_800D6728
    addi r26, r26, 0x1
    addi r31, r31, 0x34
@L_8010D380:
    lwz r0, 0x4(r29)
    cmplw r26, r0
    blt @L_8010D278
    psq_l f31, 0xa8(r1), 0, 0
    lfd f31, 0xa0(r1)
    psq_l f30, 0x98(r1), 0, 0
    lfd f30, 0x90(r1)
    psq_l f29, 0x88(r1), 0, 0
    lfd f29, 0x80(r1)
    psq_l f28, 0x78(r1), 0, 0
    lfd f28, 0x70(r1)
    psq_l f27, 0x68(r1), 0, 0
    lfd f27, 0x60(r1)
    lmw r25, 0x44(r1)
    lwz r0, 0xb4(r1)
    mtlr r0
    addi r1, r1, 0xb0
    blr
