    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    stw r0, 0x64(r1)
    stfd f31, 0x50(r1)
    psq_st f31, 0x58(r1), 0, 0
    stw r31, 0x4c(r1)
    stw r30, 0x48(r1)
    stw r29, 0x44(r1)
    fmr f31, f1
    mr r29, r3
    mr r30, r4
    mr r31, r5
    mr r4, r29
    addi r3, r1, 0x30
    mr r5, r30
    bl fn_800E019C
    addi r3, r1, 0x30
    lfs f1, lbl_8047D7A4(r2)
    mr r4, r3
    bl fn_800E013C
    mr r4, r30
    mr r5, r29
    addi r3, r1, 0x8
    bl fn_800E0168
    lfs f1, 0x8(r1)
    lfs f2, 0x10(r1)
    bl fn_800CE2D8
    frsp f1, f1
    lfs f0, lbl_8047D7A0(r2)
    lfd f2, lbl_8047D7F0(r2)
    fsubs f0, f1, f0
    fadd f1, f2, f0
    bl fn_800CE318
    frsp f1, f1
    lfd f0, lbl_8047D7A8(r2)
    fcmpo cr0, f1, f0
    ble @L_8018D724
    lfd f0, lbl_8047D7F0(r2)
    fsub f1, f1, f0
    frsp f1, f1
    b @L_8018D73C
    @L_8018D724:
    lfd f0, lbl_8047D820(r2)
    fcmpo cr0, f1, f0
    bge @L_8018D73C
    lfd f0, lbl_8047D7F0(r2)
    fadd f1, f0, f1
    frsp f1, f1
    @L_8018D73C:
    lis r3, lbl_8031554C@ha
    addi r4, r3, lbl_8031554C@l
    addi r3, r1, 0x14
    bl fn_800E0718
    mr r3, r31
    mr r4, r31
    addi r5, r1, 0x30
    bl fn_800E0168
    mr r5, r31
    addi r3, r1, 0x24
    addi r4, r1, 0x14
    bl fn_800DFEEC
    mr r4, r30
    addi r3, r1, 0x30
    bl fn_800E0040
    lfs f0, 0x24(r1)
    fabs f0, f0
    fcmpo cr0, f31, f0
    cror eq, gt, eq
    bne @L_8018D7A8
    lfs f0, 0x2c(r1)
    fabs f0, f0
    fcmpo cr0, f1, f0
    cror eq, gt, eq
    bne @L_8018D7A8
    li r3, 0x1
    b @L_8018D7AC
    @L_8018D7A8:
    li r3, 0x0
    @L_8018D7AC:
    psq_l f31, 0x58(r1), 0, 0
    lwz r0, 0x64(r1)
    lfd f31, 0x50(r1)
    lwz r31, 0x4c(r1)
    lwz r30, 0x48(r1)
    lwz r29, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
