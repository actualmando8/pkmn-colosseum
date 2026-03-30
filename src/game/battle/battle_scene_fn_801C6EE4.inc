    nofralloc
    lis r3, -32698
    li r10, 0x1
    addi r3, r3, 28752
    lwz r4, 0x0(r3)
    lfs f4, 0x8(r3)
    lfs f5, 0xc(r3)
    addi r0, r4, 0x1
    lfs f6, 0x10(r3)
    b @L_801C6FB0
@L_801C6F08:
    mullw r5, r10, r0
    lwz r4, -21608(r13)
    li r9, 0x1
    rlwinm r6, r4, 2, 0, 29
    subfic r4, r4, 0x1
    add r6, r3, r6
    mulli r7, r5, 0xc
    rlwinm r4, r4, 2, 0, 29
    lwz r5, 0x28(r6)
    add r4, r3, r4
    add r8, r5, r7
    lwz r4, 0x28(r4)
    addi r7, r7, 0xc
    addi r6, r8, 0xc
    add r7, r4, r7
    b @L_801C6FA0
@L_801C6F48:
    add r4, r9, r0
    subf r5, r0, r9
    mulli r4, r4, 0xc
    lfs f0, 0x8(r7)
    lfs f2, 0x14(r6)
    addi r9, r9, 0x1
    lfs f1, -0x4(r6)
    fmuls f0, f5, f0
    mulli r5, r5, 0xc
    addi r4, r4, 0x8
    fadds f3, f2, f1
    lfsx f2, r8, r4
    lfs f1, 0x8(r6)
    addi r6, r6, 0xc
    addi r4, r5, 0x8
    fadds f2, f3, f2
    lfsx f3, r8, r4
    fmadds f0, f4, f1, f0
    fadds f1, f3, f2
    fmadds f0, f6, f1, f0
    stfs f0, 0x8(r7)
    addi r7, r7, 0xc
@L_801C6FA0:
    lwz r4, 0x0(r3)
    cmplw r9, r4
    blt @L_801C6F48
    addi r10, r10, 0x1
@L_801C6FB0:
    lwz r4, 0x4(r3)
    cmplw r10, r4
    blt @L_801C6F08
    lwz r5, -21608(r13)
    lis r4, -32698
    addi r8, r4, 28752
    li r10, 0x1
    subfic r4, r5, 0x1
    stw r4, -21608(r13)
    b @L_801C7080
@L_801C6FD8:
    mullw r6, r10, r0
    lwz r4, -21608(r13)
    lwz r5, 0x30(r8)
    li r11, 0x1
    rlwinm r7, r4, 2, 0, 29
    lwz r4, 0x34(r8)
    mulli r9, r6, 0xc
    add r6, r8, r7
    lwz r6, 0x28(r6)
    add r12, r6, r9
    addi r7, r9, 0xc
    addi r9, r9, 0xc
    addi r6, r12, 0xc
    add r7, r5, r7
    add r9, r4, r9
    b @L_801C7070
@L_801C7018:
    lfs f1, -0x4(r6)
    subf r5, r0, r11
    lfs f0, 0x14(r6)
    add r4, r11, r0
    mulli r5, r5, 0xc
    addi r11, r11, 0x1
    fsubs f0, f1, f0
    mulli r4, r4, 0xc
    stfs f0, 0x0(r7)
    addi r5, r5, 0x8
    addi r4, r4, 0x8
    lfsx f1, r12, r5
    lfsx f0, r12, r4
    fsubs f0, f1, f0
    stfs f0, 0x4(r7)
    addi r7, r7, 0xc
    lfs f1, 0x14(r6)
    lfs f0, -0x4(r6)
    addi r6, r6, 0xc
    fsubs f0, f1, f0
    stfs f0, 0x8(r9)
    addi r9, r9, 0xc
@L_801C7070:
    lwz r4, 0x0(r8)
    cmplw r11, r4
    blt @L_801C7018
    addi r10, r10, 0x1
@L_801C7080:
    lwz r4, 0x4(r3)
    cmplw r10, r4
    blt @L_801C6FD8
    blr
