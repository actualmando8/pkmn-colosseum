    nofralloc
    mulli r5, r4, 0x3
    lis r4, 0xcc01
    li r0, 0x10
    addi r5, r5, 0x400
    stb r0, -0x8000(r4)
    oris r0, r5, 0x8
    stwu r0, -0x8000(r4)
    psq_l f5, 0x0(r3), 0, 0
    lfs f4, 0x8(r3)
    psq_l f3, 0x10(r3), 0, 0
    lfs f2, 0x18(r3)
    psq_l f1, 0x20(r3), 0, 0
    lfs f0, 0x28(r3)
    psq_st f5, 0x0(r4), 0, 0
    stfs f4, 0x0(r4)
    psq_st f3, 0x0(r4), 0, 0
    stfs f2, 0x0(r4)
    psq_st f1, 0x0(r4), 0, 0
    stfs f0, 0x0(r4)
    blr
