    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    stw r29, 0x14(r1)
    mr r30, r3
    mr r31, r4
    lwz r0, 0x114(r3)
    cmpwi r0, 0x0
    bne @L_800E4378
    addi r3, r30, 0x24
    bl fn_800E01D0
    lwz r29, 0x8(r30)
    lfs f31, 0x0(r31)
    cmplwi r29, 0x0
    bne @L_800E41CC
    la r3, lbl_8047CB60(r2)
    li r4, 0x2a4
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E41CC:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E41EC
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E41EC:
    stfs f31, 0x1c(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4244
    cmplwi r29, 0x0
    beq @L_800E4244
    bne @L_800E4218
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4218:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4234
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4234
    li r3, 0x1
    @L_800E4234:
    cmpwi r3, 0x0
    bne @L_800E4244
    mr r3, r29
    bl fn_8019D620
    @L_800E4244:
    lwz r29, 0x8(r30)
    lfs f31, 0x4(r31)
    cmplwi r29, 0x0
    bne @L_800E4264
    la r3, lbl_8047CB60(r2)
    li r4, 0x2b8
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4264:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E4284
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E4284:
    stfs f31, 0x20(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E42DC
    cmplwi r29, 0x0
    beq @L_800E42DC
    bne @L_800E42B0
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E42B0:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E42CC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E42CC
    li r3, 0x1
    @L_800E42CC:
    cmpwi r3, 0x0
    bne @L_800E42DC
    mr r3, r29
    bl fn_8019D620
    @L_800E42DC:
    lwz r29, 0x8(r30)
    lfs f31, 0x8(r31)
    cmplwi r29, 0x0
    bne @L_800E42FC
    la r3, lbl_8047CB60(r2)
    li r4, 0x2cc
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E42FC:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E431C
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E431C:
    stfs f31, 0x24(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4380
    cmplwi r29, 0x0
    beq @L_800E4380
    bne @L_800E4348
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4348:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4364
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4364
    li r3, 0x1
    @L_800E4364:
    cmpwi r3, 0x0
    bne @L_800E4380
    mr r3, r29
    bl fn_8019D620
    b @L_800E4380
    @L_800E4378:
    addi r3, r30, 0x12c
    bl fn_800E01D0
    @L_800E4380:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
