    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r3
    mr r31, r4
    addi r3, r30, 0x70
    addi r4, r31, 0x4
    bl fn_800E01D0
    addi r3, r30, 0x7c
    addi r4, r31, 0x10
    bl fn_800E01D0
    addi r3, r30, 0x88
    addi r4, r31, 0x1c
    bl fn_800E01D0
    addi r3, r30, 0x94
    addi r4, r31, 0x28
    bl fn_800E0628
    addi r3, r30, 0xc4
    addi r4, r31, 0x58
    bl fn_800E0628
    addi r3, r30, 0xf4
    addi r4, r31, 0x88
    bl fn_800E01D0
    addi r3, r30, 0x100
    addi r4, r31, 0x94
    bl fn_800E01D0
    lbz r0, 0x1(r31)
    stb r0, 0x124(r30)
    lbz r0, 0x1(r31)
    cmplwi r0, 0x0
    bne @L_800D1650
    lbz r0, 0x0(r31)
    cmplwi r0, 0x0
    beq @L_800D1650
    lbz r0, 0x3(r30)
    lwz r29, 0xa4(r31)
    cmplwi r0, 0x0
    beq @L_800D14E8
    lwz r3, 0xc(r30)
    bl fn_80196BB8
    lwz r0, 0x110(r30)
    cmplw r29, r0
    bgt @L_800D14E8
    stw r29, 0x114(r30)
    lwz r3, 0x8(r30)
    lwz r0, 0x114(r30)
    lwz r4, 0x4(r3)
    rlwinm r0, r0, 2, 0, 29
    lwz r3, 0xc(r30)
    lwzx r4, r4, r0
    bl fn_80196B10
    lwz r3, 0xc(r30)
    lfs f1, -27912(r2)
    bl fn_80196698
    lfs f0, -27912(r2)
    lis r3, -32755
    addi r6, r3, 11076
    li r4, 0x2
    stfs f0, -23976(r13)
    lis r3, 0x1
    subi r5, r3, 0x1
    li r7, 0x0
    lwz r3, 0xc(r30)
    crxor 6, 6, 6
    bl fn_801C028C
    lfs f0, -23976(r13)
    stfs f0, 0x120(r30)
@L_800D14E8:
    lwz r0, 0xa0(r31)
    stw r0, 0x10c(r30)
    lbz r0, 0x3(r30)
    lfs f31, 0xa8(r31)
    cmplwi r0, 0x0
    beq @L_800D1540
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_800D1514
    lfs f0, -27888(r2)
    fmuls f31, f31, f0
@L_800D1514:
    stfs f31, 0x118(r30)
    lis r3, -32740
    addi r6, r3, 636
    lis r4, 0x1
    lwz r3, 0xc(r30)
    subi r5, r4, 0x1
    lfs f1, 0x118(r30)
    li r4, 0x2
    li r7, 0x1
    cror 6, 6, 6
    bl fn_801C028C
@L_800D1540:
    lbz r0, 0x3(r30)
    lfs f0, 0xac(r31)
    cmplwi r0, 0x0
    beq @L_800D15B8
    stfs f0, 0x11c(r30)
    lis r3, -32740
    addi r6, r3, 636
    lis r4, 0x1
    lwz r3, 0xc(r30)
    subi r5, r4, 0x1
    lfd f1, -27880(r2)
    li r4, 0x2
    li r7, 0x1
    cror 6, 6, 6
    bl fn_801C028C
    lwz r3, 0xc(r30)
    lfs f1, 0x11c(r30)
    bl fn_80196698
    lwz r3, 0xc(r30)
    bl fn_801966FC
    lis r3, -32740
    lis r4, 0x1
    addi r6, r3, 636
    lwz r3, 0xc(r30)
    subi r5, r4, 0x1
    lfs f1, 0x118(r30)
    li r4, 0x2
    li r7, 0x1
    cror 6, 6, 6
    bl fn_801C028C
@L_800D15B8:
    lbz r0, 0x3(r30)
    cmplwi r0, 0x0
    beq @L_800D1650
    li r3, 0x1
    li r0, 0x0
    stb r3, 0x4(r30)
    stb r0, 0x124(r30)
    stb r3, 0x125(r30)
    lbz r0, 0x3(r30)
    lfs f0, 0x11c(r30)
    cmplwi r0, 0x0
    beq @L_800D1650
    stfs f0, 0x11c(r30)
    lis r3, -32740
    addi r6, r3, 636
    lis r4, 0x1
    lwz r3, 0xc(r30)
    subi r5, r4, 0x1
    lfd f1, -27880(r2)
    li r4, 0x2
    li r7, 0x1
    cror 6, 6, 6
    bl fn_801C028C
    lwz r3, 0xc(r30)
    lfs f1, 0x11c(r30)
    bl fn_80196698
    lwz r3, 0xc(r30)
    bl fn_801966FC
    lis r3, -32740
    lis r4, 0x1
    addi r6, r3, 636
    lwz r3, 0xc(r30)
    subi r5, r4, 0x1
    lfs f1, 0x118(r30)
    li r4, 0x2
    li r7, 0x1
    cror 6, 6, 6
    bl fn_801C028C
@L_800D1650:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
