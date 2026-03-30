    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    lwz r0, 0x0(r3)
    rlwinm. r0, r0, 0, 28, 28
    beq @L_800EC348
    stfs f1, 0xac(r31)
    bl fn_800D37CC
    cmpwi r3, 0x32
    bne @L_800EC348
    lfs f1, 0xac(r31)
    lfs f0, lbl_8047CC58(r2)
    fmuls f0, f1, f0
    stfs f0, 0xac(r31)
    @L_800EC348:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
