    nofralloc
    lfs f0, -26556(r2)
    lfs f2, -26552(r2)
    lfs f4, 0x0(r5)
    fmr f1, f0
    fmr f3, f2
    fcmpo cr0, f0, f4
    ble @L_8010DF10
    fmr f1, f4
@L_8010DF10:
    lfs f5, 0x8(r5)
    fcmpo cr0, f0, f5
    ble @L_8010DF20
    fmr f0, f5
@L_8010DF20:
    fcmpo cr0, f3, f4
    bge @L_8010DF2C
    fmr f3, f4
@L_8010DF2C:
    fcmpo cr0, f2, f5
    bge @L_8010DF38
    fmr f2, f5
@L_8010DF38:
    lfs f4, 0xc(r5)
    fcmpo cr0, f1, f4
    ble @L_8010DF48
    fmr f1, f4
@L_8010DF48:
    lfs f5, 0x14(r5)
    fcmpo cr0, f0, f5
    ble @L_8010DF58
    fmr f0, f5
@L_8010DF58:
    fcmpo cr0, f3, f4
    bge @L_8010DF64
    fmr f3, f4
@L_8010DF64:
    fcmpo cr0, f2, f5
    bge @L_8010DF70
    fmr f2, f5
@L_8010DF70:
    lfs f4, 0x18(r5)
    fcmpo cr0, f1, f4
    ble @L_8010DF80
    fmr f1, f4
@L_8010DF80:
    lfs f5, 0x20(r5)
    fcmpo cr0, f0, f5
    ble @L_8010DF90
    fmr f0, f5
@L_8010DF90:
    fcmpo cr0, f3, f4
    bge @L_8010DF9C
    fmr f3, f4
@L_8010DF9C:
    fcmpo cr0, f2, f5
    bge @L_8010DFA8
    fmr f2, f5
@L_8010DFA8:
    lfs f6, 0x0(r4)
    fcmpo cr0, f1, f6
    bgt @L_8010DFD0
    lfs f7, 0x8(r4)
    fcmpo cr0, f0, f7
    bgt @L_8010DFD0
    fcmpo cr0, f3, f6
    blt @L_8010DFD0
    fcmpo cr0, f2, f7
    bge @L_8010DFD8
@L_8010DFD0:
    li r3, 0x0
    blr
@L_8010DFD8:
    li r0, 0x1
    lfs f0, -26560(r2)
    cmpwi r0, 0x3
    blt @L_8010DFEC
    li r0, 0x0
@L_8010DFEC:
    mulli r0, r0, 0xc
    lfs f5, 0x0(r5)
    lfs f8, 0x8(r5)
    fsubs f1, f6, f5
    add r7, r5, r0
    fsubs f2, f7, f8
    lfs f3, 0x8(r7)
    lfs f4, 0x0(r7)
    fsubs f3, f3, f8
    fsubs f4, f4, f5
    fmuls f1, f3, f1
    fmsubs f1, f4, f2, f1
    fcmpo cr0, f1, f0
    ble @L_8010E02C
    li r3, 0x0
    blr
@L_8010E02C:
    li r0, 0x2
    cmpwi r0, 0x3
    blt @L_8010E03C
    li r0, 0x0
@L_8010E03C:
    mulli r0, r0, 0xc
    lfs f5, 0xc(r5)
    lfs f8, 0x14(r5)
    fsubs f1, f6, f5
    add r7, r5, r0
    fsubs f2, f7, f8
    lfs f3, 0x8(r7)
    lfs f4, 0x0(r7)
    fsubs f3, f3, f8
    fsubs f4, f4, f5
    fmuls f1, f3, f1
    fmsubs f1, f4, f2, f1
    fcmpo cr0, f1, f0
    ble @L_8010E07C
    li r3, 0x0
    blr
@L_8010E07C:
    li r0, 0x3
    cmpwi r0, 0x3
    blt @L_8010E08C
    li r0, 0x0
@L_8010E08C:
    mulli r0, r0, 0xc
    lfs f5, 0x18(r5)
    lfs f8, 0x20(r5)
    fsubs f1, f6, f5
    add r7, r5, r0
    fsubs f2, f7, f8
    lfs f3, 0x8(r7)
    lfs f4, 0x0(r7)
    fsubs f3, f3, f8
    fsubs f4, f4, f5
    fmuls f1, f3, f1
    fmsubs f1, f4, f2, f1
    fcmpo cr0, f1, f0
    ble @L_8010E0CC
    li r3, 0x0
    blr
@L_8010E0CC:
    lfs f0, -26560(r2)
    lfs f8, 0x4(r6)
    fcmpu cr0, f0, f8
    bne @L_8010E0E4
    li r3, 0x0
    blr
@L_8010E0E4:
    lfs f1, 0x4(r4)
    lfs f0, 0x4(r5)
    lfs f2, 0x0(r5)
    fsubs f0, f1, f0
    lfs f1, 0x8(r5)
    fsubs f2, f6, f2
    lfs f3, 0x0(r6)
    lfs f5, 0x8(r6)
    fsubs f4, f7, f1
    fmuls f1, f8, f0
    stfs f6, 0x0(r3)
    lfs f0, 0x4(r4)
    fmadds f1, f3, f2, f1
    fnmadds f1, f5, f4, f1
    fdivs f1, f1, f8
    fadds f0, f0, f1
    stfs f0, 0x4(r3)
    lfs f0, 0x8(r4)
    stfs f0, 0x8(r3)
    li r3, 0x1
    blr
