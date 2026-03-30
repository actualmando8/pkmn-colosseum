    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stfd f31, 0x20(r1)
    psq_st f31, 0x28(r1), 0, 0
    stw r31, 0x1c(r1)
    stw r30, 0x18(r1)
    mr r31, r3
    mr r30, r4
    addi r3, r1, 0x8
    addi r4, r31, 0x24
    bl fn_800E01D0
    addi r3, r1, 0x8
    mr r5, r30
    mr r4, r3
    bl fn_800E019C
    lwz r0, 0x114(r31)
    cmpwi r0, 0x0
    bne @L_800E3FE8
    addi r3, r31, 0x24
    addi r4, r1, 0x8
    bl fn_800E01D0
    lwz r30, 0x8(r31)
    lfs f31, 0x8(r1)
    cmplwi r30, 0x0
    bne @L_800E3E3C
    la r3, lbl_8047CB60(r2)
    li r4, 0x2a4
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E3E3C:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E3E5C
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E3E5C:
    stfs f31, 0x1c(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E3EB4
    cmplwi r30, 0x0
    beq @L_800E3EB4
    bne @L_800E3E88
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E3E88:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E3EA4
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E3EA4
    li r3, 0x1
    @L_800E3EA4:
    cmpwi r3, 0x0
    bne @L_800E3EB4
    mr r3, r30
    bl fn_8019D620
    @L_800E3EB4:
    lwz r30, 0x8(r31)
    lfs f31, 0xc(r1)
    cmplwi r30, 0x0
    bne @L_800E3ED4
    la r3, lbl_8047CB60(r2)
    li r4, 0x2b8
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E3ED4:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E3EF4
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E3EF4:
    stfs f31, 0x20(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E3F4C
    cmplwi r30, 0x0
    beq @L_800E3F4C
    bne @L_800E3F20
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E3F20:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E3F3C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E3F3C
    li r3, 0x1
    @L_800E3F3C:
    cmpwi r3, 0x0
    bne @L_800E3F4C
    mr r3, r30
    bl fn_8019D620
    @L_800E3F4C:
    lwz r30, 0x8(r31)
    lfs f31, 0x10(r1)
    cmplwi r30, 0x0
    bne @L_800E3F6C
    la r3, lbl_8047CB60(r2)
    li r4, 0x2cc
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E3F6C:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E3F8C
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E3F8C:
    stfs f31, 0x24(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E3FF4
    cmplwi r30, 0x0
    beq @L_800E3FF4
    bne @L_800E3FB8
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E3FB8:
    lwz r4, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E3FD4
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E3FD4
    li r3, 0x1
    @L_800E3FD4:
    cmpwi r3, 0x0
    bne @L_800E3FF4
    mr r3, r30
    bl fn_8019D620
    b @L_800E3FF4
    @L_800E3FE8:
    addi r3, r31, 0x12c
    addi r4, r1, 0x8
    bl fn_800E01D0
    @L_800E3FF4:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
