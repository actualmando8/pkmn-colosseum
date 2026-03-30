    nofralloc
    lwz r0, 0x0(r3)
    rlwinm. r0, r0, 0, 24, 24
    bne @L_800ED82C
    clrlwi. r0, r4, 24
    bne @L_800ED810
    lfs f3, 0xa0(r3)
    addi r5, r3, 0x98
    lwz r7, 0x8c(r3)
    addi r6, r3, 0x9c
    li r4, 0x4000
    b @L_800ED848
    @L_800ED810:
    lis r4, 0x1
    lfs f3, 0xb8(r3)
    lwz r7, 0xa4(r3)
    addi r5, r3, 0xb0
    addi r6, r3, 0xb4
    addi r4, r4, -0x8000
    b @L_800ED848
    @L_800ED82C:
    clrlwi. r0, r4, 24
    bne @L_800ED848
    addi r5, r3, 0xc8
    lfs f3, 0xd0(r3)
    lwz r7, 0x8c(r3)
    mr r6, r5
    li r4, 0x4000
    @L_800ED848:
    lfs f2, 0x0(r5)
    lfs f0, lbl_8047CC78(r2)
    fadds f1, f2, f1
    fsubs f0, f3, f0
    stfs f1, 0x0(r5)
    lfs f1, 0x0(r5)
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_800ED894
    cmpwi r7, 0x1
    beq @L_800ED88C
    bge @L_800ED894
    cmpwi r7, 0x0
    bge @L_800ED884
    b @L_800ED894
    @L_800ED884:
    stfs f3, 0x0(r5)
    b @L_800ED894
    @L_800ED88C:
    fsubs f0, f1, f3
    stfs f0, 0x0(r5)
    @L_800ED894:
    lfs f0, lbl_8047CC78(r2)
    lfs f1, 0x0(r6)
    fsubs f0, f3, f0
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bnelr
    cmpwi r7, 0x0
    bnelr
    lwz r0, 0x0(r3)
    or r0, r0, r4
    stw r0, 0x0(r3)
    blr
