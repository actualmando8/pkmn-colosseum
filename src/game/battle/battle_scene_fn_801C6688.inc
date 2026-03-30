    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    fmr f31, f1
    bl fn_801C6760
    lfs f0, -22192(r2)
    li r3, 0x2
    fmuls f0, f0, f31
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    subfic r31, r0, 0xff
    bl fn_800D67BC
    lfs f1, -22208(r2)
    fmr f2, f1
    fmr f3, f1
    bl fn_800D6680
    rlwinm r7, r31, 0, 24, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    lfs f1, -22188(r2)
    lfs f2, -22184(r2)
    lfs f3, -22208(r2)
    bl fn_800D6680
    rlwinm r7, r31, 0, 24, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_800D5CB8
    bl fn_800D6728
    bl fn_801C673C
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
