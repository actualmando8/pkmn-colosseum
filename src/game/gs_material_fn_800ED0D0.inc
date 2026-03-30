    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x10(r1)
    psq_st f31, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    lwz r4, 0x0(r3)
    fmr f31, f1
    mr r31, r3
    rlwinm. r0, r4, 0, 23, 23
    bne @L_800ED1B0
    rlwinm. r0, r4, 0, 17, 17
    beq @L_800ED10C
    li r4, 0x0
    bl fn_800ED6E4
    @L_800ED10C:
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 16, 16
    beq @L_800ED124
    mr r3, r31
    li r4, 0x1
    bl fn_800ED6E4
    @L_800ED124:
    lwz r3, 0x0(r31)
    rlwinm. r0, r3, 0, 24, 24
    beq @L_800ED160
    rlwinm. r0, r3, 0, 26, 26
    beq @L_800ED19C
    lfs f0, 0x94(r31)
    mr r3, r31
    li r4, 0x0
    fmuls f1, f31, f0
    bl fn_800ED7E4
    lfs f1, 0xc8(r31)
    lfs f0, 0xd8(r31)
    fmuls f0, f1, f0
    stfs f0, 0xc4(r31)
    b @L_800ED19C
    @L_800ED160:
    rlwinm. r0, r3, 0, 26, 26
    beq @L_800ED17C
    lfs f0, 0x94(r31)
    mr r3, r31
    li r4, 0x0
    fmuls f1, f31, f0
    bl fn_800ED7E4
    @L_800ED17C:
    lwz r0, 0x0(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_800ED19C
    lfs f0, 0xac(r31)
    mr r3, r31
    li r4, 0x1
    fmuls f1, f31, f0
    bl fn_800ED7E4
    @L_800ED19C:
    lis r3, 0xffff
    lwz r4, 0x0(r31)
    subi r0, r3, 0x1001
    and r0, r4, r0
    stw r0, 0x0(r31)
    @L_800ED1B0:
    psq_l f31, 0x18(r1), 0, 0
    lwz r0, 0x24(r1)
    lfd f31, 0x10(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
