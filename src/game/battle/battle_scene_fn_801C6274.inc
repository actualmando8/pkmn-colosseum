    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r29, r3
    mr r30, r4
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
    lfs f2, -22036(r2)
    addi r31, r4, 28720
    li r0, 0x0
    lfs f1, -22156(r2)
    lfs f0, -22208(r2)
    stw r3, 0x4(r31)
    li r3, 0x2
    sth r0, 0x8(r31)
    sth r0, 0xa(r31)
    sth r30, 0xc(r31)
    sth r0, 0xe(r31)
    stfs f2, 0x10(r31)
    stfs f1, 0x18(r31)
    stfs f0, 0x14(r31)
    bl fn_801C6908
    cmplwi r3, 0x0
    bne @L_801C6384
    lfs f1, 0x10(r31)
    rlwinm r0, r30, 0, 16, 31
    lfs f2, -22032(r2)
    cmpwi r0, 0x4
    lfs f0, 0x18(r31)
    fmuls f1, f1, f2
    fmuls f0, f0, f2
    stfs f1, 0x10(r31)
    stfs f0, 0x18(r31)
    beq @L_801C6370
    bge @L_801C634C
    cmpwi r0, 0x2
    beq @L_801C6364
    bge @L_801C6384
    cmpwi r0, 0x1
    bge @L_801C6358
    b @L_801C6384
@L_801C634C:
    cmpwi r0, 0x8
    beq @L_801C637C
    b @L_801C6384
@L_801C6358:
    li r0, 0x2
    sth r0, 0xc(r31)
    b @L_801C6384
@L_801C6364:
    li r0, 0x1
    sth r0, 0xc(r31)
    b @L_801C6384
@L_801C6370:
    li r0, 0x8
    sth r0, 0xc(r31)
    b @L_801C6384
@L_801C637C:
    li r0, 0x4
    sth r0, 0xc(r31)
@L_801C6384:
    bl fn_801C68D4
    lis r4, -32740
    mr r3, r29
    addi r4, r4, 25528
    li r5, 0x0
    bl fn_800DC390
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
