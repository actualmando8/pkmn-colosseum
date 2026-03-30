    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    stw r30, 0x8(r1)
    mr r30, r4
    lwz r31, 0x8(r3)
    cmplwi r31, 0x0
    bne @L_800E45CC
    la r3, lbl_8047CB60(r2)
    li r4, 0x495
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E45CC:
    cmplwi r30, 0x0
    bne @L_800E45E4
    la r3, lbl_8047CB60(r2)
    li r4, 0x496
    la r5, lbl_8047CB78(r2)
    bl fn_80196E10
    @L_800E45E4:
    mr r3, r30
    addi r4, r31, 0x44
    bl fn_800A2D64
    lwz r0, 0x14(r31)
    cmplwi r31, 0x0
    oris r0, r0, 0x380
    stw r0, 0x14(r31)
    beq @L_800E4644
    bne @L_800E4618
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4618:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4634
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4634
    li r3, 0x1
    @L_800E4634:
    cmpwi r3, 0x0
    bne @L_800E4644
    mr r3, r31
    bl fn_8019D620
    @L_800E4644:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
