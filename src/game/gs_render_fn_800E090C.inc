    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stfd f31, 0x10(r1)
    psq_st f31, 0x18(r1), 0, 0
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    fmr f31, f1
    lfs f0, lbl_8047CAF0(r2)
    mr r30, r3
    mr r31, r4
    fcmpo cr0, f31, f0
    cror eq, lt, eq
    bne @L_800E094C
    bl fn_800E01D0
    b @L_800E0994
    @L_800E094C:
    lfs f0, lbl_8047CAF4(r2)
    fcmpo cr0, f31, f0
    cror eq, gt, eq
    bne @L_800E0968
    mr r4, r5
    bl fn_800E01D0
    b @L_800E0994
    @L_800E0968:
    mr r4, r5
    mr r5, r31
    bl fn_800E0168
    fmr f1, f31
    mr r3, r30
    mr r4, r30
    bl fn_800E013C
    mr r3, r30
    mr r4, r30
    mr r5, r31
    bl fn_800E019C
    @L_800E0994:
    psq_l f31, 0x18(r1), 0, 0
    lwz r0, 0x24(r1)
    lfd f31, 0x10(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
