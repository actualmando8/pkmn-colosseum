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
    stfd f27, 0x50(r1)
    psq_st f27, 0x58(r1), 0, 0
    stmw r27, 0x3c(r1)
    mr. r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    blt @L_8010FE4C
    cmpwi r27, 0x30
    blt @L_8010FE54
@L_8010FE4C:
    li r3, 0x4
    b @L_8010FE90
@L_8010FE54:
    bl fn_8010CBD0
    cmplwi r3, 0x0
    bne @L_8010FE68
    li r3, 0x1
    b @L_8010FE90
@L_8010FE68:
    mulli r4, r27, 0x14
    addi r4, r4, 0xa00
    add r4, r3, r4
    lhz r0, 0x10(r4)
    rlwinm. r0, r0, 0, 31, 31
    bne @L_8010FE88
    li r3, 0x4
    b @L_8010FE90
@L_8010FE88:
    mr r31, r4
    li r3, 0x0
@L_8010FE90:
    cmpwi r3, 0x0
    beq @L_8010FE9C
    b @L_8010FF88
@L_8010FE9C:
    mr r3, r28
    mr r4, r29
    bl fn_800A3C00
    lfs f0, -26496(r2)
    fcmpo cr0, f1, f0
    ble @L_8010FED0
    lfs f2, 0x8(r31)
    lfs f0, -26464(r2)
    fdivs f29, f2, f1
    fcmpo cr0, f29, f0
    ble @L_8010FED4
    fmr f29, f0
    b @L_8010FED4
@L_8010FED0:
    fmr f29, f0
@L_8010FED4:
    mr r3, r29
    mr r4, r28
    addi r5, r1, 0x20
    bl fn_800A3A9C
    lfs f28, -26496(r2)
    lfs f30, -26464(r2)
    fmr f31, f28
    b @L_8010FF7C
@L_8010FEF4:
    fadds f27, f28, f29
    fcmpo cr0, f27, f30
    ble @L_8010FF04
    fmr f27, f30
@L_8010FF04:
    fmr f1, f28
    addi r3, r1, 0x20
    addi r4, r1, 0x14
    bl fn_800A3AC0
    addi r3, r1, 0x14
    mr r4, r28
    mr r5, r3
    bl fn_800A3A78
    fmr f1, f27
    addi r3, r1, 0x20
    addi r4, r1, 0x8
    bl fn_800A3AC0
    addi r3, r1, 0x8
    mr r4, r28
    mr r5, r3
    bl fn_800A3A78
    mr r3, r31
    mr r4, r27
    mr r7, r30
    addi r5, r1, 0x14
    addi r6, r1, 0x8
    bl fn_8010FAF4
    cmpwi r3, 0x0
    beq @L_8010FF6C
    li r3, 0x6
    b @L_8010FF88
@L_8010FF6C:
    fcmpo cr0, f29, f31
    cror eq, lt, eq
    beq @L_8010FF84
    fadds f28, f28, f29
@L_8010FF7C:
    fcmpo cr0, f28, f30
    blt @L_8010FEF4
@L_8010FF84:
    li r3, 0x7
@L_8010FF88:
    psq_l f31, 0x98(r1), 0, 0
    lfd f31, 0x90(r1)
    psq_l f30, 0x88(r1), 0, 0
    lfd f30, 0x80(r1)
    psq_l f29, 0x78(r1), 0, 0
    lfd f29, 0x70(r1)
    psq_l f28, 0x68(r1), 0, 0
    lfd f28, 0x60(r1)
    psq_l f27, 0x58(r1), 0, 0
    lfd f27, 0x50(r1)
    lmw r27, 0x3c(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr

