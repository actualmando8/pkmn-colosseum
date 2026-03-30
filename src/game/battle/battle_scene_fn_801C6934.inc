    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    stw r0, 0x74(r1)
    stfd f31, 0x60(r1)
    psq_st f31, 0x68(r1), 0, 0
    stw r31, 0x5c(r1)
    stw r30, 0x58(r1)
    stw r29, 0x54(r1)
    stw r28, 0x50(r1)
    fmr f31, f1
    mr r28, r3
    lfs f3, -22008(r2)
    fmr f1, f2
    lfs f0, -22004(r2)
    addi r3, r1, 0x20
    stfs f3, 0x14(r1)
    addi r4, r1, 0x14
    addi r5, r1, 0x8
    stfs f3, 0x18(r1)
    stfs f3, 0x1c(r1)
    stfs f0, 0x8(r1)
    stfs f0, 0xc(r1)
    stfs f0, 0x10(r1)
    bl fn_800E090C
    lfs f2, -22000(r2)
    lis r0, 0x4330
    lfs f1, 0x20(r1)
    li r3, 0x1
    lfs f0, -22008(r2)
    fmuls f1, f2, f1
    stw r0, 0x38(r1)
    lfd f2, -21992(r2)
    fsubs f0, f0, f31
    fctiwz f1, f1
    stfd f1, 0x30(r1)
    lwz r0, 0x34(r1)
    xoris r0, r0, 0x8000
    stw r0, 0x3c(r1)
    lfd f1, 0x38(r1)
    fsubs f1, f1, f2
    fmuls f0, f1, f0
    fctiwz f0, f0
    stfd f0, 0x40(r1)
    lwz r30, 0x44(r1)
    bl fn_800D9ED8
    li r3, 0x3
    bl fn_800D88DC
    li r3, 0x4
    bl fn_800D888C
    bl fn_800FE4D4
    li r3, 0x1
    li r4, 0x6
    li r5, 0x7
    bl fn_800DA4C4
    li r3, 0x1
    li r4, 0x1
    li r5, 0x0
    bl fn_800DA2BC
    li r3, 0x0
    li r4, 0x1
    li r5, 0x1
    bl fn_800DA1E8
    li r3, 0x0
    li r4, 0x7
    li r5, 0x0
    li r6, 0x1
    li r7, 0x7
    li r8, 0x0
    bl fn_800DA100
    li r3, 0x0
    bl fn_800DA028
    li r3, 0x4
    bl fn_800D6A00
    lis r3, -32719
    addi r3, r3, 20776
    bl fn_800D7820
    mr r4, r28
    li r3, 0x0
    bl fn_800D85D4
    lis r3, -32698
    li r28, 0x0
    addi r31, r3, 28752
    b @L_801C6AAC
@L_801C6A80:
    li r29, 0x0
    b @L_801C6A9C
@L_801C6A88:
    mr r3, r29
    mr r4, r28
    rlwinm r5, r30, 0, 24, 31
    bl fn_801C6AE8
    addi r29, r29, 0x1
@L_801C6A9C:
    lwz r0, 0x0(r31)
    cmplw r29, r0
    blt @L_801C6A88
    addi r28, r28, 0x1
@L_801C6AAC:
    lwz r0, 0x4(r31)
    cmplw r28, r0
    blt @L_801C6A80
    li r3, 0x0
    bl fn_800D9ED8
    psq_l f31, 0x68(r1), 0, 0
    lwz r0, 0x74(r1)
    lfd f31, 0x60(r1)
    lwz r31, 0x5c(r1)
    lwz r30, 0x58(r1)
    lwz r29, 0x54(r1)
    lwz r28, 0x50(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
