    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    stw r0, 0x14(r1)
    stw r31, 0xc(r1)
    mr r31, r3
    stw r30, 0x8(r1)
    mr r30, r4
    lwz r0, 0x114(r3)
    cmpwi r0, 0x0
    bne @L_800E4150
    addi r3, r31, 0x30
    bl fn_800E01D0
    lwz r31, 0x8(r31)
    cmplwi r31, 0x0
    bne @L_800E40C8
    la r3, lbl_8047CB60(r2)
    li r4, 0x316
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E40C8:
    cmplwi r30, 0x0
    bne @L_800E40E0
    la r3, lbl_8047CB60(r2)
    li r4, 0x317
    la r5, lbl_8047CB70(r2)
    bl fn_80196E10
    @L_800E40E0:
    lwz r3, 0x0(r30)
    lwz r0, 0x4(r30)
    stw r3, 0x2c(r31)
    stw r0, 0x30(r31)
    lwz r0, 0x8(r30)
    stw r0, 0x34(r31)
    lwz r0, 0x14(r31)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_800E4158
    cmplwi r31, 0x0
    beq @L_800E4158
    bne @L_800E4120
    la r3, lbl_8047CB60(r2)
    li r4, 0x25d
    la r5, lbl_8047CB68(r2)
    bl fn_80196E10
    @L_800E4120:
    lwz r4, 0x14(r31)
    li r3, 0x0
    rlwinm. r0, r4, 0, 8, 8
    bne @L_800E413C
    rlwinm. r0, r4, 0, 25, 25
    beq @L_800E413C
    li r3, 0x1
    @L_800E413C:
    cmpwi r3, 0x0
    bne @L_800E4158
    mr r3, r31
    bl fn_8019D620
    b @L_800E4158
    @L_800E4150:
    addi r3, r31, 0x138
    bl fn_800E01D0
    @L_800E4158:
    lwz r0, 0x14(r1)
    lwz r31, 0xc(r1)
    lwz r30, 0x8(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
