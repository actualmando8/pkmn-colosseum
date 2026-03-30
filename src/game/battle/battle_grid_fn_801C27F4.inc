    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stw r31, 0x1c(r1)
    mr r31, r5
    stw r30, 0x18(r1)
    mr r30, r4
    stw r29, 0x14(r1)
    mr. r29, r3
    beq @L_801C29A8
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_801C282C
    b @L_801C29A8
    @L_801C282C:
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 4, 4
    beq @L_801C284C
    lwz r0, 0x0(r29)
    lfs f1, lbl_8047DF54(r2)
    rlwinm r0, r0, 0, 5, 3
    stw r0, 0x0(r29)
    b @L_801C285C
    @L_801C284C:
    lfs f1, 0x10(r29)
    lfs f0, 0x4(r29)
    fadds f0, f0, f1
    stfs f0, 0x4(r29)
    @L_801C285C:
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 2, 2
    beq @L_801C28F4
    lfs f2, 0xc(r29)
    lfs f0, 0x4(r29)
    fcmpo cr0, f2, f0
    cror eq, lt, eq
    bne @L_801C28F4
    lfs f2, 0x8(r29)
    lfs f0, 0xc(r29)
    fcmpo cr0, f2, f0
    bge @L_801C28D8
    lwz r3, 0x14(r29)
    mr r4, r30
    mr r5, r31
    bl fn_8019B490
    lfs f3, 0x4(r29)
    lfs f1, 0x8(r29)
    lfs f2, 0xc(r29)
    lfs f0, 0x8(r29)
    fsubs f1, f3, f1
    fsubs f2, f2, f0
    bl fn_800CE318
    lfs f0, 0x8(r29)
    fadd f0, f0, f1
    frsp f0, f0
    stfs f0, 0x4(r29)
    lwz r3, 0x14(r29)
    lfs f1, 0x4(r29)
    bl fn_8019B528
    b @L_801C28E0
    @L_801C28D8:
    lfs f0, 0xc(r29)
    stfs f0, 0x4(r29)
    @L_801C28E0:
    lwz r0, 0x0(r29)
    lfs f1, lbl_8047DF54(r2)
    oris r0, r0, 0x400
    stw r0, 0x0(r29)
    b @L_801C2900
    @L_801C28F4:
    lwz r0, 0x0(r29)
    rlwinm r0, r0, 0, 6, 4
    stw r0, 0x0(r29)
    @L_801C2900:
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 3, 3
    beq @L_801C2920
    lwz r3, 0x14(r29)
    mr r4, r30
    li r5, 0x0
    bl fn_80199A88
    b @L_801C2930
    @L_801C2920:
    lwz r3, 0x14(r29)
    mr r4, r30
    mr r5, r31
    bl fn_80199A88
    @L_801C2930:
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 2, 2
    bne @L_801C2978
    lfs f1, 0xc(r29)
    lfs f0, 0x4(r29)
    fcmpo cr0, f1, f0
    cror eq, lt, eq
    bne @L_801C2978
    cmplwi r29, 0x0
    beq @L_801C2978
    lwz r3, 0x14(r29)
    mr r4, r30
    lfs f1, 0x10(r29)
    mr r5, r31
    bl fn_8019B490
    lwz r0, 0x0(r29)
    oris r0, r0, 0x4000
    stw r0, 0x0(r29)
    @L_801C2978:
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 1, 1
    beq @L_801C2990
    lwz r3, lbl_8047B390(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B390(r13)
    @L_801C2990:
    lwz r0, 0x0(r29)
    rlwinm. r0, r0, 0, 1, 1
    bne @L_801C29A8
    lwz r3, lbl_8047B38C(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B38C(r13)
    @L_801C29A8:
    lwz r0, 0x24(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
