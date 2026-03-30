    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stfd f31, 0x30(r1)
    psq_st f31, 0x38(r1), 0, 0
    stmw r27, 0x1c(r1)
    mr r31, r7
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r3, r8
    mr r4, r31
    addi r5, r1, 0x8
    bl fn_800A3A9C
    lfs f4, 0x4(r29)
    lfs f0, 0xc(r1)
    lfs f5, 0x0(r29)
    fmuls f0, f4, f0
    lfs f1, 0x8(r1)
    lfs f6, 0x8(r29)
    lfs f2, 0x10(r1)
    fmadds f0, f5, f1, f0
    lfs f1, -26512(r2)
    fmadds f7, f6, f2, f0
    fcmpu cr0, f1, f7
    bne @L_8010F52C
    li r3, 0x0
    b @L_8010F588
@L_8010F52C:
    lfs f1, 0x4(r30)
    addi r3, r1, 0x8
    lfs f0, 0x4(r31)
    mr r4, r3
    lfs f2, 0x0(r30)
    fsubs f0, f1, f0
    lfs f1, 0x0(r31)
    lfs f3, 0x8(r30)
    fsubs f1, f2, f1
    lfs f2, 0x8(r31)
    fmuls f0, f4, f0
    fsubs f2, f3, f2
    fmadds f0, f5, f1, f0
    fmadds f0, f6, f2, f0
    fdivs f31, f0, f7
    fmr f1, f31
    bl fn_800A3AC0
    mr r4, r31
    mr r5, r27
    addi r3, r1, 0x8
    bl fn_800A3A78
    stfs f31, 0x0(r28)
    li r3, 0x1
@L_8010F588:
    psq_l f31, 0x38(r1), 0, 0
    lfd f31, 0x30(r1)
    lmw r27, 0x1c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr

