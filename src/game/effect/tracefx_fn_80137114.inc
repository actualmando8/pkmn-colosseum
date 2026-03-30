    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stfd f30, 0x40(r1)
    psq_st f30, 0x48(r1), 0, 0
    stmw r27, 0x2c(r1)
    mr r28, r4
    mr r27, r5
    lwz r31, 0x0(r4)
    mr r29, r3
    li r4, 0x0
    li r5, 0x20
    bl memset
    lwz r0, 0x4(r28)
    cmpwi r0, 0x2
    beq @L_801371FC
    bge @L_8013716C
    cmpwi r0, 0x1
    bge @L_80137284
    b @L_80137334
@L_8013716C:
    cmpwi r0, 0x4
    bge @L_80137334
    lwz r0, 0x8(r28)
    addi r30, r28, 0xc
    lfd f31, -25976(r2)
    li r27, 0x0
    stw r0, 0x10(r29)
    lis r28, 0x4330
    lfs f30, -25992(r2)
    b @L_801371F0
@L_80137194:
    bl fn_800D37CC
    lwz r0, 0x8(r30)
    xoris r3, r3, 0x8000
    stw r3, 0x14(r1)
    mr r3, r29
    xoris r0, r0, 0x8000
    lfs f1, 0x0(r30)
    stw r28, 0x10(r1)
    lfs f2, 0x4(r30)
    lfd f0, 0x10(r1)
    stw r0, 0x1c(r1)
    fsubs f3, f0, f31
    stw r28, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f31
    fmuls f0, f0, f3
    fdivs f0, f0, f30
    fctiwz f0, f0
    stfd f0, 0x20(r1)
    lwz r4, 0x24(r1)
    bl fn_8013DB64
    addi r27, r27, 0x1
    addi r30, r30, 0x10
@L_801371F0:
    cmpw r27, r31
    blt @L_80137194
    b @L_80137334
@L_801371FC:
    li r0, 0x1
    lfd f30, -25976(r2)
    stw r0, 0x10(r29)
    addi r30, r28, 0x8
    lfs f31, -25992(r2)
    li r27, 0x0
    lis r28, 0x4330
    b @L_80137278
@L_8013721C:
    bl fn_800D37CC
    lwz r0, 0x8(r30)
    xoris r3, r3, 0x8000
    stw r3, 0x24(r1)
    mr r3, r29
    xoris r0, r0, 0x8000
    lfs f1, 0x0(r30)
    stw r28, 0x20(r1)
    lfs f2, 0x4(r30)
    lfd f0, 0x20(r1)
    stw r0, 0x1c(r1)
    fsubs f3, f0, f30
    stw r28, 0x18(r1)
    lfd f0, 0x18(r1)
    fsubs f0, f0, f30
    fmuls f0, f0, f3
    fdivs f0, f0, f31
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r4, 0x14(r1)
    bl fn_8013DB64
    addi r27, r27, 0x1
    addi r30, r30, 0x10
@L_80137278:
    cmpw r27, r31
    blt @L_8013721C
    b @L_80137334
@L_80137284:
    li r0, 0x1
    stw r0, 0x10(r29)
    lwz r0, 0x0(r28)
    stw r0, 0x8(r1)
    lfs f30, 0x8(r1)
    bl fn_800D37CC
    xoris r0, r3, 0x8000
    lis r5, 0x4330
    xoris r4, r27, 0x8000
    stw r0, 0x24(r1)
    lis r3, 0x5555
    lfd f5, -25976(r2)
    stw r5, 0x20(r1)
    addi r0, r3, 0x5556
    lfs f0, -25992(r2)
    fmr f2, f30
    lfd f1, 0x20(r1)
    mr r3, r29
    stw r4, 0x1c(r1)
    fsubs f4, f1, f5
    lfs f1, -25988(r2)
    stw r5, 0x18(r1)
    lfd f3, 0x18(r1)
    fsubs f3, f3, f5
    fmuls f3, f3, f4
    fdivs f0, f3, f0
    fctiwz f0, f0
    stfd f0, 0x10(r1)
    lwz r27, 0x14(r1)
    mulhw r4, r0, r27
    rlwinm r0, r4, 1, 31, 31
    add r28, r4, r0
    mr r4, r28
    bl fn_8013DB64
    fmr f1, f30
    mr r3, r29
    fmr f2, f30
    mr r4, r28
    bl fn_8013DB64
    lfs f1, -25988(r2)
    mr r3, r29
    mr r4, r28
    fmr f2, f1
    bl fn_8013DB64
@L_80137334:
    mr r3, r30
    psq_l f31, 0x58(r1), 0, 0
    lfd f31, 0x50(r1)
    psq_l f30, 0x48(r1), 0, 0
    lfd f30, 0x40(r1)
    lmw r27, 0x2c(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
