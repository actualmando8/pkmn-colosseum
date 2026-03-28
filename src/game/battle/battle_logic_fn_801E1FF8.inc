    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    li r4, 0x0
    stw r0, 0xc4(r1)
    stmw r30, 0xb8(r1)
    lhz r31, 0x4(r3)
    lhz r30, 0x6(r3)
    li r3, 0x0
    bl fn_800BCEF4
    lis r4, 0x4330
    xoris r3, r30, 0x8000
    xoris r0, r31, 0x8000
    stw r3, 0x94(r1)
    lfs f1, -21004(r2)
    addi r3, r1, 0x4c
    stw r4, 0x90(r1)
    lfd f4, -20992(r2)
    fmr f3, f1
    lfd f0, 0x90(r1)
    fmr f5, f1
    stw r0, 0x9c(r1)
    fsubs f2, f0, f4
    lfs f6, -21000(r2)
    stw r4, 0x98(r1)
    lfd f0, 0x98(r1)
    fsubs f4, f0, f4
    bl fn_800A39E0
    addi r3, r1, 0x4c
    li r4, 0x1
    bl fn_800BD2E0
    lis r3, 0x4330
    xoris r4, r31, 0x8000
    xoris r0, r30, 0x8000
    stw r4, 0xa4(r1)
    lfs f1, -21004(r2)
    stw r3, 0xa0(r1)
    lfd f4, -20992(r2)
    fmr f2, f1
    lfd f0, 0xa0(r1)
    fmr f5, f1
    stw r0, 0xac(r1)
    fsubs f3, f0, f4
    lfs f6, -20996(r2)
    stw r3, 0xa8(r1)
    lfd f0, 0xa8(r1)
    fsubs f4, f0, f4
    bl fn_800BD744
    mr r5, r31
    mr r6, r30
    li r3, 0x0
    li r4, 0x0
    bl fn_800BD7A0
    addi r3, r1, 0x1c
    bl fn_800A2D38
    addi r3, r1, 0x1c
    li r4, 0x0
    bl fn_800BD4B4
    li r3, 0x0
    bl fn_800BD554
    li r3, 0x1
    li r4, 0x7
    li r5, 0x0
    bl fn_800BCE88
    li r3, 0x0
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    bl fn_800BCDDC
    li r3, 0x1
    bl fn_800BCE30
    li r3, 0x0
    bl fn_800BCE5C
    li r3, 0x0
    bl fn_800B9E6C
    li r3, 0x0
    bl fn_800BA6B0
    li r3, 0x2
    bl fn_800B884C
    li r3, 0x0
    li r4, 0x1
    li r5, 0x4
    li r6, 0x3c
    li r7, 0x0
    li r8, 0x7d
    bl fn_800B857C
    li r3, 0x1
    li r4, 0x1
    li r5, 0x4
    li r6, 0x3c
    li r7, 0x0
    li r8, 0x7d
    bl fn_800B857C
    bl fn_800BB29C
    bl fn_800B7D3C
    li r3, 0x9
    li r4, 0x1
    bl fn_800B7874
    li r3, 0xd
    li r4, 0x1
    bl fn_800B7874
    li r3, 0x7
    li r4, 0x9
    li r5, 0x1
    li r6, 0x3
    li r7, 0x0
    bl fn_800B7D74
    li r3, 0x7
    li r4, 0xd
    li r5, 0x1
    li r6, 0x2
    li r7, 0x0
    bl fn_800B7D74
    li r3, 0x4
    bl fn_800BC8C8
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    li r6, 0xff
    bl fn_800BC6F0
    li r3, 0x0
    li r4, 0xf
    li r5, 0x8
    li r6, 0xe
    li r7, 0x2
    bl fn_800BC1A0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_800BC228
    li r3, 0x0
    li r4, 0x7
    li r5, 0x4
    li r6, 0x6
    li r7, 0x1
    bl fn_800BC1E4
    li r3, 0x0
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_800BC290
    li r3, 0x0
    li r4, 0xc
    bl fn_800BC454
    li r3, 0x0
    li r4, 0x1c
    bl fn_800BC4C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    bl fn_800BC52C
    li r3, 0x1
    li r4, 0x1
    li r5, 0x2
    li r6, 0xff
    bl fn_800BC6F0
    li r3, 0x1
    li r4, 0xf
    li r5, 0x8
    li r6, 0xe
    li r7, 0x0
    bl fn_800BC1A0
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    li r6, 0x1
    li r7, 0x0
    li r8, 0x0
    bl fn_800BC228
    li r3, 0x1
    li r4, 0x7
    li r5, 0x4
    li r6, 0x6
    li r7, 0x0
    bl fn_800BC1E4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_800BC290
    li r3, 0x1
    li r4, 0xd
    bl fn_800BC454
    li r3, 0x1
    li r4, 0x1d
    bl fn_800BC4C0
    li r3, 0x1
    li r4, 0x0
    li r5, 0x0
    bl fn_800BC52C
    li r3, 0x2
    li r4, 0x0
    li r5, 0x0
    li r6, 0xff
    bl fn_800BC6F0
    li r3, 0x2
    li r4, 0xf
    li r5, 0x8
    li r6, 0xc
    li r7, 0x0
    bl fn_800BC1A0
    li r3, 0x2
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800BC228
    li r3, 0x2
    li r4, 0x4
    li r5, 0x7
    li r6, 0x7
    li r7, 0x0
    bl fn_800BC1E4
    li r3, 0x2
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800BC290
    li r3, 0x2
    li r4, 0x0
    li r5, 0x0
    bl fn_800BC52C
    li r3, 0x3
    li r4, 0xff
    li r5, 0xff
    li r6, 0xff
    bl fn_800BC6F0
    li r3, 0x3
    li r4, 0x1
    li r5, 0x0
    li r6, 0xe
    li r7, 0xf
    bl fn_800BC1A0
    li r3, 0x3
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800BC228
    li r3, 0x3
    li r4, 0x7
    li r5, 0x7
    li r6, 0x7
    li r7, 0x7
    bl fn_800BC1E4
    li r3, 0x3
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    bl fn_800BC290
    li r3, 0x3
    li r4, 0x0
    li r5, 0x0
    bl fn_800BC52C
    li r3, 0x3
    li r4, 0xe
    bl fn_800BC454
    lwz r5, -21024(r2)
    addi r4, r1, 0x14
    lwz r0, -21020(r2)
    li r3, 0x1
    stw r5, 0x14(r1)
    stw r0, 0x18(r1)
    bl fn_800BC36C
    lwz r0, -21016(r2)
    addi r4, r1, 0x10
    li r3, 0x0
    stw r0, 0x10(r1)
    bl fn_800BC3E0
    lwz r0, -21012(r2)
    addi r4, r1, 0xc
    li r3, 0x1
    stw r0, 0xc(r1)
    bl fn_800BC3E0
    lwz r0, -21008(r2)
    addi r4, r1, 0x8
    li r3, 0x2
    stw r0, 0x8(r1)
    bl fn_800BC3E0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    li r7, 0x3
    bl fn_800BC580
    lmw r30, 0xb8(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
