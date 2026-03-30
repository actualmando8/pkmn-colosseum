    nofralloc
    lis r5, 0xcc01
    li r0, 0x10
    rlwinm r4, r4, 2, 0, 29
    stb r0, -0x8000(r5)
    oris r0, r4, 0xb
    stw r0, -0x8000(r5)
    addi r4, r5, -0x8000
    psq_l f5, 0x0(r3), 0, 0
    psq_l f4, 0x8(r3), 0, 0
    psq_l f3, 0x10(r3), 0, 0
    psq_l f2, 0x18(r3), 0, 0
    psq_l f1, 0x20(r3), 0, 0
    psq_l f0, 0x28(r3), 0, 0
    psq_st f5, 0x0(r4), 0, 0
    psq_st f4, 0x0(r4), 0, 0
    psq_st f3, 0x0(r4), 0, 0
    psq_st f2, 0x0(r4), 0, 0
    psq_st f1, 0x0(r4), 0, 0
    psq_st f0, 0x0(r4), 0, 0
    blr
