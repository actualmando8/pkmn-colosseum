    nofralloc
    mflr r0
    cmplwi r3, 0x0
    stw r0, 0x4(r1)
    stwu r1, -0x60(r1)
    stfd f31, 0x58(r1)
    stfd f30, 0x50(r1)
    stfd f29, 0x48(r1)
    stfd f28, 0x40(r1)
    stfd f27, 0x38(r1)
    stfd f26, 0x30(r1)
    bne @L_800BD674
    lfs f0, -29368(r2)
    fsubs f2, f2, f0
@L_800BD674:
    lfs f11, -29368(r2)
    fneg f10, f4
    lfs f0, -29348(r2)
    lfs f9, -29352(r2)
    fmuls f31, f3, f11
    lwz r3, -29632(r2)
    fmuls f7, f4, f11
    stfs f1, 0x43c(r3)
    fadds f8, f1, f31
    fmuls f30, f0, f6
    stfs f2, 0x440(r3)
    fadds f1, f2, f7
    stfs f3, 0x444(r3)
    fmuls f0, f0, f5
    fmuls f29, f10, f11
    stfs f4, 0x448(r3)
    fadds f27, f9, f8
    stfs f5, 0x44c(r3)
    fadds f26, f9, f1
    fsubs f28, f30, f0
    stfs f6, 0x450(r3)
    lbz r0, 0x454(r3)
    cmplwi r0, 0x0
    beq @L_800BD6E0
    fmr f1, f5
    lfs f2, 0x458(r3)
    bl fn_800BD0F8
@L_800BD6E0:
    li r0, 0x10
    lwz r3, -29632(r2)
    lis r5, 0xcc01
    lis r4, 0x5
    stb r0, -0x8000(r5)
    addi r0, r4, 0x101a
    stw r0, -0x8000(r5)
    li r0, 0x1
    stfs f31, -0x8000(r5)
    stfs f29, -0x8000(r5)
    stfs f28, -0x8000(r5)
    stfs f27, -0x8000(r5)
    stfs f26, -0x8000(r5)
    stfs f30, -0x8000(r5)
    sth r0, 0x2(r3)
    lwz r0, 0x64(r1)
    lfd f31, 0x58(r1)
    lfd f30, 0x50(r1)
    lfd f29, 0x48(r1)
    lfd f28, 0x40(r1)
    lfd f27, 0x38(r1)
    lfd f26, 0x30(r1)
    addi r1, r1, 0x60
    mtlr r0
    blr
