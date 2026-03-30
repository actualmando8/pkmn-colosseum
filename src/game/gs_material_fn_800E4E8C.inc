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
    addi r3, r30, 0x24
    bl fn_800E01D0
    lwz r29, 0x8(r30)
    lfs f31, 0x0(r31)
    cmplwi r29, 0x0
    bne @L_800E4EDC
    la r3, lbl_8047CB60(r2)
    li r4, 0x2a4
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4EDC:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E4EFC
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2a5
    bl fn_80196E10
    @L_800E4EFC:
    stfs f31, 0x1c(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4F54
    cmplwi r29, 0x0
    beq @L_800E4F54
    bne @L_800E4F28
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4F28:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4F44
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4F44
    li r3, 0x1
    @L_800E4F44:
    cmpwi r3, 0x0
    bne @L_800E4F54
    mr r3, r29
    bl fn_8019D620
    @L_800E4F54:
    lwz r29, 0x8(r30)
    lfs f31, 0x4(r31)
    cmplwi r29, 0x0
    bne @L_800E4F74
    la r3, lbl_8047CB60(r2)
    li r4, 0x2b8
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4F74:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E4F94
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2b9
    bl fn_80196E10
    @L_800E4F94:
    stfs f31, 0x20(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4FEC
    cmplwi r29, 0x0
    beq @L_800E4FEC
    bne @L_800E4FC0
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4FC0:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E4FDC
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E4FDC
    li r3, 0x1
    @L_800E4FDC:
    cmpwi r3, 0x0
    bne @L_800E4FEC
    mr r3, r29
    bl fn_8019D620
    @L_800E4FEC:
    lwz r29, 0x8(r30)
    lfs f31, 0x8(r31)
    cmplwi r29, 0x0
    bne @L_800E500C
    la r3, lbl_8047CB60(r2)
    li r4, 0x2cc
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E500C:
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_800E502C
    lis r4, lbl_80270E28@ha
    la r3, lbl_8047CB60(r2)
    addi r5, r4, lbl_80270E28@l
    li r4, 0x2cd
    bl fn_80196E10
    @L_800E502C:
    stfs f31, 0x24(r29)
    lwz r0, 0x14(r29)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E5084
    cmplwi r29, 0x0
    beq @L_800E5084
    bne @L_800E5058
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E5058:
    lwz r4, 0x14(r29)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E5074
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E5074
    li r3, 0x1
    @L_800E5074:
    cmpwi r3, 0x0
    bne @L_800E5084
    mr r3, r29
    bl fn_8019D620
    @L_800E5084:
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
