    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x10(r1)
    psq_st f31, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    lwz r0, 0x0(r3)
    fmr f31, f1
    mr r31, r3
    rlwinm. r0, r0, 0, 29, 29
    beq @L_800ECA5C
    stfs f31, 0x94(r31)
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_800ECA28
    lfs f1, 0x94(r31)
    lfs f0, lbl_8047CC58(r2)
    fmuls f0, f1, f0
    stfs f0, 0x94(r31)
    @L_800ECA28:
    lwz r3, 0x0(r31)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_800ECA5C
    rlwinm. r0, r3, 0, 28, 28
    beq @L_800ECA5C
    stfs f31, 0xac(r31)
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_800ECA5C
    lfs f1, 0xac(r31)
    lfs f0, lbl_8047CC58(r2)
    fmuls f0, f1, f0
    stfs f0, 0xac(r31)
    @L_800ECA5C:
    psq_l f31, 0x18(r1), 0, 0
    lwz r0, 0x24(r1)
    lfd f31, 0x10(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
