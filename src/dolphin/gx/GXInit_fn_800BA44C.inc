    nofralloc
    cntlzw r0, r4
    subfic r0, r0, 0x1f
    clrlslwi r5, r0, 29, 4
    lis r4, 0xcc01
    li r0, 0x10
    addi r5, r5, 0x600
    stb r0, -0x8000(r4)
    oris r0, r5, 0xf
    stwu r0, -0x8000(r4)
    lwz r0, 0xc(r3)
    xor r6, r6, r6
    psq_l f5, 0x10(r3), 0, 0
    psq_l f4, 0x18(r3), 0, 0
    psq_l f3, 0x20(r3), 0, 0
    psq_l f2, 0x28(r3), 0, 0
    psq_l f1, 0x30(r3), 0, 0
    psq_l f0, 0x38(r3), 0, 0
    stw r6, 0x0(r4)
    stw r6, 0x0(r4)
    stw r6, 0x0(r4)
    stw r0, 0x0(r4)
    psq_st f5, 0x0(r4), 0, 0
    psq_st f4, 0x0(r4), 0, 0
    psq_st f3, 0x0(r4), 0, 0
    psq_st f2, 0x0(r4), 0, 0
    psq_st f1, 0x0(r4), 0, 0
    psq_st f0, 0x0(r4), 0, 0
    lwz r3, -29632(r2)
    li r0, 0x1
    sth r0, 0x2(r3)
    blr
