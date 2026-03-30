    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x10(r1)
    psq_st f31, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    lwz r4, 0x0(r3)
    mr r31, r3
    fmr f31, f1
    lwz r3, 0x8(r3)
    rlwinm. r0, r4, 0, 29, 29
    beq @L_800ECB58
    rlwinm. r0, r4, 0, 24, 24
    bne @L_800ECAD4
    rlwinm. r0, r4, 0, 14, 14
    beq @L_800ECABC
    lwz r3, 0x10(r3)
    @L_800ECABC:
    stfs f31, 0x98(r31)
    li r4, 0x1cb
    stfs f31, 0x9c(r31)
    lfs f1, 0x9c(r31)
    bl fn_801A32A0
    b @L_800ECB04
    @L_800ECAD4:
    lfs f0, 0xd8(r31)
    li r4, 0x1cb
    fmuls f0, f31, f0
    stfs f0, 0xc4(r31)
    stfs f31, 0xc8(r31)
    lwz r3, 0x10(r31)
    lfs f1, 0xc4(r31)
    bl fn_801A32A0
    lwz r3, 0x14(r31)
    li r4, 0x1cb
    lfs f1, 0xc8(r31)
    bl fn_801A32A0
    @L_800ECB04:
    lwz r3, 0x0(r31)
    li r0, -0x5001
    and r0, r3, r0
    stw r0, 0x0(r31)
    lwz r4, 0x0(r31)
    rlwinm. r0, r4, 0, 18, 18
    beq @L_800ECB58
    rlwinm. r0, r4, 0, 28, 28
    lwz r3, 0x8(r31)
    beq @L_800ECB58
    rlwinm. r0, r4, 0, 14, 14
    beq @L_800ECB38
    lwz r3, 0x10(r3)
    @L_800ECB38:
    stfs f31, 0xb0(r31)
    li r4, 0x634
    stfs f31, 0xb4(r31)
    lfs f1, 0xb4(r31)
    bl fn_801A32A0
    lwz r0, 0x0(r31)
    rlwinm r0, r0, 0, 17, 15
    stw r0, 0x0(r31)
    @L_800ECB58:
    psq_l f31, 0x18(r1), 0, 0
    lwz r0, 0x24(r1)
    lfd f31, 0x10(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
