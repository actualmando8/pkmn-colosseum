    nofralloc
    lfs f0, -22608(r2)
    lis r7, -32698
    stwu r1, -0x10(r1)
    lis r6, -32698
    lwz r8, 0x0(r3)
    fcmpo cr0, f1, f0
    lis r9, -32698
    lwz r4, 0x0(r4)
    addi r7, r7, 22288
    lwz r0, 0x0(r5)
    addi r3, r6, 22288
    stw r8, 22288(r9)
    stw r4, 0x4(r7)
    stw r0, 0x8(r3)
    cror eq, lt, eq
    bne @L_801B2910
    lfs f1, -22608(r2)
    b @L_801B2924
@L_801B2910:
    lfs f0, -22604(r2)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_801B2924
    lfs f1, -22604(r2)
@L_801B2924:
    lfs f0, -22600(r2)
    lis r3, -32698
    addi r3, r3, 22288
    fmuls f0, f0, f1
    fctiwz f0, f0
    stfd f0, 0x8(r1)
    lwz r0, 0xc(r1)
    stb r0, 0xc(r3)
    addi r1, r1, 0x10
    blr
