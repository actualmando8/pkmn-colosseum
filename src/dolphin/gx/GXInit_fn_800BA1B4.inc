    nofralloc
    mflr r0
    stw r0, 0x4(r1)
    stwu r1, -0x20(r1)
    stw r31, 0x1c(r1)
    addi r31, r4, 0x0
    stw r30, 0x18(r1)
    addi r30, r3, 0x0
    lfs f0, -29576(r2)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    beq @L_800BA1EC
    lfs f0, -29572(r2)
    fcmpo cr0, f1, f0
    ble @L_800BA1F0
@L_800BA1EC:
    li r31, 0x0
@L_800BA1F0:
    lfs f2, -29568(r2)
    lfs f0, -29564(r2)
    fmuls f1, f2, f1
    fdivs f1, f1, f0
    bl fn_800CE6D0
    cmplwi r31, 0x6
    bgt @L_800BA314
    lis r3, -32719
    addi r3, r3, 13128
    rlwinm r0, r31, 2, 0, 29
    lwzx r0, r3, r0
    mtctr r0
    bctr
    lfs f0, -29560(r2)
    lfs f4, -29556(r2)
    fmuls f3, f0, f1
    lfs f6, -29576(r2)
    b @L_800BA320
    lfs f2, -29552(r2)
    fneg f0, f1
    lfs f6, -29576(r2)
    fsubs f1, f2, f1
    fdivs f1, f2, f1
    fmr f4, f1
    fmuls f3, f0, f1
    b @L_800BA320
    lfs f2, -29552(r2)
    fneg f0, f1
    lfs f3, -29576(r2)
    fsubs f1, f2, f1
    fdivs f1, f2, f1
    fmr f6, f1
    fmuls f4, f0, f1
    b @L_800BA320
    lfs f4, -29552(r2)
    lfs f2, -29548(r2)
    fsubs f3, f4, f1
    fsubs f0, f1, f2
    fmuls f3, f3, f3
    fmuls f0, f1, f0
    fdivs f1, f4, f3
    fmuls f3, f1, f0
    fmuls f4, f2, f1
    fneg f6, f1
    b @L_800BA320
    lfs f5, -29552(r2)
    lfs f3, -29544(r2)
    fsubs f4, f5, f1
    lfs f2, -29540(r2)
    fadds f0, f5, f1
    fmuls f4, f4, f4
    fmuls f0, f2, f0
    fdivs f4, f5, f4
    fmuls f2, f3, f4
    fmuls f4, f0, f4
    fmr f6, f2
    fmuls f3, f2, f1
    b @L_800BA320
    lfs f5, -29552(r2)
    lfs f0, -29548(r2)
    fsubs f4, f5, f1
    lfs f2, -29540(r2)
    fmuls f3, f0, f1
    lfs f0, -29536(r2)
    fmuls f2, f2, f1
    fmuls f4, f4, f4
    fmuls f1, f3, f1
    fdivs f3, f5, f4
    fmuls f1, f3, f1
    fmuls f4, f2, f3
    fmuls f6, f0, f3
    fsubs f3, f5, f1
    b @L_800BA320
@L_800BA314:
    lfs f4, -29576(r2)
    lfs f3, -29552(r2)
    fmr f6, f4
@L_800BA320:
    stfs f3, 0x10(r30)
    stfs f4, 0x14(r30)
    stfs f6, 0x18(r30)
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    addi r1, r1, 0x20
    mtlr r0
    blr
