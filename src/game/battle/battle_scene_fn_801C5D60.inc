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
    lfs f5, -22208(r2)
    lis r4, -32698
    addi r5, r4, 28720
    li r0, 0x0
    lfs f6, -22200(r2)
    lis r4, -32698
    lfs f4, -22068(r2)
    addi r6, r4, 28240
    lfs f3, -22152(r2)
    addi r7, r1, 0x8
    lfs f2, -22064(r2)
    li r8, 0x0
    lfs f1, -22060(r2)
    lfs f0, -22056(r2)
    stw r3, 0x4(r5)
    sth r0, 0x8(r5)
    sth r0, 0xa(r5)
    sth r0, 0xc(r5)
    sth r0, 0xe(r5)
    stfs f6, 0x10(r5)
    stfs f5, 0x14(r5)
    stfs f4, 0x18(r5)
    stfs f3, 0x8(r1)
    stfs f2, 0xc(r1)
    stfs f5, 0x10(r1)
    stfs f1, 0x14(r1)
    stfs f0, 0x18(r1)
    stfs f5, 0x1c(r1)
@L_801C5E08:
    mr r3, r6
    li r4, 0x0
    li r0, 0x3
    mtctr r0
@L_801C5E18:
    lfs f2, 0x0(r7)
    subfic r0, r4, 0xff
    lfs f1, 0x4(r7)
    addi r4, r4, 0x15
    stfs f2, 0x0(r3)
    lfs f0, 0x8(r7)
    stfs f1, 0x4(r3)
    stfs f0, 0x8(r3)
    stfs f5, 0xc(r3)
    stw r0, 0x10(r3)
    addi r3, r3, 0x14
    subfic r0, r4, 0xff
    addi r4, r4, 0x15
    stfs f2, 0x0(r3)
    stfs f1, 0x4(r3)
    stfs f0, 0x8(r3)
    stfs f5, 0xc(r3)
    stw r0, 0x10(r3)
    addi r3, r3, 0x14
    subfic r0, r4, 0xff
    addi r4, r4, 0x15
    stfs f2, 0x0(r3)
    stfs f1, 0x4(r3)
    stfs f0, 0x8(r3)
    stfs f5, 0xc(r3)
    stw r0, 0x10(r3)
    addi r3, r3, 0x14
    subfic r0, r4, 0xff
    addi r4, r4, 0x15
    stfs f2, 0x0(r3)
    stfs f1, 0x4(r3)
    stfs f0, 0x8(r3)
    stfs f5, 0xc(r3)
    stw r0, 0x10(r3)
    addi r3, r3, 0x14
    bdnz @L_801C5E18
    addi r6, r6, 0xf0
    addi r7, r7, 0xc
    addi r8, r8, 0x1
    cmpwi r8, 0x2
    blt @L_801C5E08
    bl fn_801C68D4
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
