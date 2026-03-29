    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    mr r31, r3
    lbz r0, 0x1(r31)
    extsb r0, r0
    cmpwi r0, 0x2
    beq @L_80017B8C
    bge @L_80017A40
    cmpwi r0, 0x0
    beq @L_80017A4C
    b @L_80017CA0
    @L_80017A40:
    cmpwi r0, 0x4
    bge @L_80017CA0
    b @L_80017C7C
    @L_80017A4C:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80017CA0
    li r3, 0x59
    li r4, 0x5e
    bl fn_801080CC
    lwz r0, lbl_8047A2D8(r13)
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    mulli r0, r0, 0x4c
    add r3, r3, r0
    lwz r0, 0x1c(r3)
    clrlwi r3, r0, 16
    bl fn_80103E68
    lbz r0, 0x95(r31)
    lis r4, lbl_80266918@ha
    srwi r3, r3, 16
    extsb r0, r0
    sth r3, 0xc(r1)
    mulli r0, r0, 0x4c
    addi r3, r4, lbl_80266918@l
    lbz r5, 0xc(r1)
    lbz r4, 0xd(r1)
    add r3, r3, r0
    extsb r5, r5
    lwz r6, 0x4(r3)
    extsb r0, r4
    add r27, r5, r0
    cmpwi r6, 0x0
    blt @L_80017AE8
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r6, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80017B00
    @L_80017AE8:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80017B00:
    mr r28, r3
    li r30, -0x1
    li r29, 0x0
    b @L_80017B44
    @L_80017B10:
    mr r3, r28
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80017B3C
    addi r30, r30, 0x1
    cmpw r30, r27
    blt @L_80017B3C
    mr r3, r28
    bl fn_80143C68
    b @L_80017B54
    @L_80017B3C:
    addi r29, r29, 0x1
    addi r28, r28, 0x4
    @L_80017B44:
    lhz r0, 0x8(r1)
    cmpw r29, r0
    blt @L_80017B10
    li r3, 0x0
    @L_80017B54:
    clrlwi r3, r3, 16
    bl fn_801440A0
    bl fn_80143F84
    lfs f0, lbl_8047B748(r2)
    li r4, 0x0
    stw r3, lbl_8047A2DC(r13)
    li r0, 0x1
    stfs f0, lbl_8047A2D4(r13)
    stfs f0, lbl_8047A2D0(r13)
    stw r4, lbl_8047A2C8(r13)
    stfs f0, lbl_8047A2C4(r13)
    stfs f0, lbl_8047A2CC(r13)
    stb r0, 0x2(r31)
    b @L_80017CA0
    @L_80017B8C:
    lfs f2, lbl_8047A2D4(r13)
    lfs f1, lbl_8047B748(r2)
    fcmpo cr0, f2, f1
    ble @L_80017BB4
    lfs f0, lbl_8047B794(r2)
    fsubs f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A2D4(r13)
    bge @L_80017BB4
    stfs f1, lbl_8047A2D4(r13)
    @L_80017BB4:
    lfs f2, lbl_8047A2D4(r13)
    lfs f1, lbl_8047B748(r2)
    fcmpo cr0, f2, f1
    bge @L_80017BDC
    lfs f0, lbl_8047B794(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A2D4(r13)
    ble @L_80017BDC
    stfs f1, lbl_8047A2D4(r13)
    @L_80017BDC:
    lfs f2, lbl_8047A2D0(r13)
    lfs f1, lbl_8047B748(r2)
    fcmpo cr0, f2, f1
    ble @L_80017C04
    lfs f0, lbl_8047B798(r2)
    fsubs f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A2D0(r13)
    bge @L_80017C04
    stfs f1, lbl_8047A2D0(r13)
    @L_80017C04:
    lfs f2, lbl_8047A2D0(r13)
    lfs f1, lbl_8047B748(r2)
    fcmpo cr0, f2, f1
    bge @L_80017C2C
    lfs f0, lbl_8047B798(r2)
    fadds f0, f2, f0
    fcmpo cr0, f0, f1
    stfs f0, lbl_8047A2D0(r13)
    ble @L_80017C2C
    stfs f1, lbl_8047A2D0(r13)
    @L_80017C2C:
    lfs f2, lbl_8047A2C4(r13)
    lfs f1, lbl_8047B74C(r2)
    lfs f0, lbl_8047B744(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A2C4(r13)
    ble @L_80017C50
    lfs f0, lbl_8047B748(r2)
    stfs f0, lbl_8047A2C4(r13)
    @L_80017C50:
    lfs f2, lbl_8047A2CC(r13)
    lfs f1, lbl_8047B79C(r2)
    lfs f0, lbl_8047B744(r2)
    fadds f1, f2, f1
    fcmpo cr0, f1, f0
    stfs f1, lbl_8047A2CC(r13)
    cror eq, gt, eq
    bne @L_80017CA0
    fsubs f0, f1, f0
    stfs f0, lbl_8047A2CC(r13)
    b @L_80017CA0
    @L_80017C7C:
    lbz r0, 0x2(r31)
    extsb r0, r0
    cmpwi r0, 0x0
    bne @L_80017CA0
    li r3, 0x59
    li r4, 0x62
    bl fn_801080CC
    li r0, 0x1
    stb r0, 0x2(r31)
    @L_80017CA0:
    li r3, 0x0
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
