    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    li r3, 0x1
    bl fn_800D3074
    lis r3, 0xf86
    addi r3, r3, 0x1200
    bl fn_800F92D4
    lis r5, -32698
    lis r4, 0xf87
    addi r5, r5, 28720
    stw r3, 0x0(r5)
    addi r3, r4, 0x1200
    bl fn_800F92D4
    lfs f1, -22204(r2)
    lis r4, -32698
    lfs f6, -22140(r2)
    addi r5, r4, 28720
    li r0, 0x0
    lfs f5, -22136(r2)
    lfs f0, -22208(r2)
    fmr f2, f1
    stw r3, 0x4(r5)
    li r3, 0x28
    lfs f3, -22132(r2)
    li r4, 0x1e
    sth r0, 0x8(r5)
    lfs f4, -22128(r2)
    sth r0, 0xa(r5)
    sth r0, 0xc(r5)
    sth r0, 0xe(r5)
    stfs f6, 0x10(r5)
    stfs f5, 0x18(r5)
    stfs f0, 0x14(r5)
    bl fn_801C71B0
    lfs f1, -22124(r2)
    addi r3, r1, 0x8
    lfs f2, -22120(r2)
    lfs f0, -22208(r2)
    stfs f1, 0x8(r1)
    lfs f1, -22144(r2)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_801C6D78
    lfs f2, -22116(r2)
    addi r3, r1, 0x8
    lfs f0, -22208(r2)
    stfs f2, 0x8(r1)
    lfs f1, -22112(r2)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_801C6D78
    lfs f1, -22108(r2)
    addi r3, r1, 0x8
    lfs f2, -22104(r2)
    lfs f0, -22208(r2)
    stfs f1, 0x8(r1)
    lfs f1, -22112(r2)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_801C6D78
    lfs f1, -22108(r2)
    addi r3, r1, 0x8
    lfs f2, -22116(r2)
    lfs f0, -22208(r2)
    stfs f1, 0x8(r1)
    lfs f1, -22112(r2)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_801C6D78
    lfs f1, -22116(r2)
    addi r3, r1, 0x8
    lfs f2, -22104(r2)
    lfs f0, -22208(r2)
    stfs f1, 0x8(r1)
    lfs f1, -22112(r2)
    stfs f2, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_801C6D78
    bl fn_801C68D4
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
