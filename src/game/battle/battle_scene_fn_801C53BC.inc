    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
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
    lis r4, -32698
    lfs f2, -22148(r2)
    addi r6, r4, 28720
    li r0, 0x0
    lfs f1, -22144(r2)
    lis r4, -32740
    lfs f0, -22208(r2)
    addi r4, r4, 25528
    stw r3, 0x4(r6)
    mr r3, r31
    li r5, 0x0
    sth r0, 0x8(r6)
    sth r0, 0xa(r6)
    sth r0, 0xc(r6)
    sth r0, 0xe(r6)
    stfs f2, 0x10(r6)
    stfs f1, 0x18(r6)
    stfs f0, 0x14(r6)
    bl fn_800DC390
    bl fn_800D7894
    cmplwi r3, 0x0
    stw r3, -21612(r13)
    beq @L_801C54E4
    li r4, 0x1
    li r5, 0x0
    li r6, 0x1
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, -21612(r13)
    li r4, 0x4
    li r5, 0x0
    li r6, 0x6
    li r7, 0xa
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, -21612(r13)
    li r4, 0x6
    li r5, 0x0
    li r6, 0x8
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
    lwz r3, -21612(r13)
    li r4, 0x7
    li r5, 0x0
    li r6, 0x8
    li r7, 0x4
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_800D7868
@L_801C54E4:
    bl fn_801C68D4
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
