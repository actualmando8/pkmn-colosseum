    nofralloc
    cmplwi r4, 0x40
    blt @L_800BD5A4
    subi r0, r4, 0x40
    rlwinm r4, r0, 2, 0, 29
    addi r7, r4, 0x500
    b @L_800BD5A8
@L_800BD5A4:
    rlwinm r7, r4, 2, 0, 29
@L_800BD5A8:
    cmpwi r5, 0x1
    bne @L_800BD5B8
    li r4, 0x8
    b @L_800BD5BC
@L_800BD5B8:
    li r4, 0xc
@L_800BD5BC:
    subi r0, r4, 0x1
    rlwinm r6, r0, 16, 0, 15
    li r0, 0x10
    lis r4, 0xcc01
    stb r0, -0x8000(r4)
    or r0, r7, r6
    cmpwi r5, 0x0
    stw r0, -0x8000(r4)
    bne @L_800BD618
    addi r4, r4, -0x8000
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
@L_800BD618:
    addi r4, r4, -0x8000
    psq_l f3, 0x0(r3), 0, 0
    psq_l f2, 0x8(r3), 0, 0
    psq_l f1, 0x10(r3), 0, 0
    psq_l f0, 0x18(r3), 0, 0
    psq_st f3, 0x0(r4), 0, 0
    psq_st f2, 0x0(r4), 0, 0
    psq_st f1, 0x0(r4), 0, 0
    psq_st f0, 0x0(r4), 0, 0
    blr
