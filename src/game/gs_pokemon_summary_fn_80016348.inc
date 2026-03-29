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
    mr r29, r4
    lha r5, 0x54(r29)
    li r3, 0x0
    lha r6, 0x56(r29)
    li r4, 0x0
    bl fn_800FE38C
    lbz r0, 0x95(r30)
    lis r3, lbl_80266918@ha
    lha r5, 0x54(r29)
    addi r3, r3, lbl_80266918@l
    extsb r0, r0
    mulli r0, r0, 0x4c
    srwi r4, r5, 31
    add r4, r4, r5
    add r3, r3, r0
    lwz r29, 0x20(r3)
    srawi r31, r4, 1
    mr r3, r29
    bl fn_800FA444
    lfs f0, lbl_8047A2D4(r13)
    srwi r0, r3, 16
    extsh r3, r0
    mr r6, r29
    fctiwz f0, f0
    srwi r0, r3, 31
    add r0, r0, r3
    li r4, 0x0
    srawi r0, r0, 1
    li r5, -0x1
    stfd f0, 0x8(r1)
    subf r0, r0, r31
    lwz r3, 0xc(r1)
    add r3, r3, r0
    bl fn_800FB680
    lfs f0, lbl_8047B748(r2)
    lfs f1, lbl_8047A2D4(r13)
    fcmpu cr0, f0, f1
    beq @L_800164A4
    fcmpo cr0, f1, f0
    ble @L_80016430
    lbz r0, 0x95(r30)
    extsb r3, r0
    addi r0, r3, 0x1
    cmpwi r0, 0x6
    blt @L_80016424
    li r0, 0x1
    @L_80016424:
    lfs f0, lbl_8047B750(r2)
    fsubs f31, f1, f0
    b @L_80016450
    @L_80016430:
    lbz r0, 0x95(r30)
    extsb r3, r0
    subi r0, r3, 0x1
    cmpwi r0, 0x1
    bge @L_80016448
    li r0, 0x5
    @L_80016448:
    lfs f0, lbl_8047B750(r2)
    fadds f31, f0, f1
    @L_80016450:
    mulli r0, r0, 0x4c
    lis r3, lbl_80266918@ha
    addi r3, r3, lbl_80266918@l
    add r3, r3, r0
    lwz r30, 0x20(r3)
    mr r3, r30
    bl fn_800FA444
    fctiwz f0, f31
    srwi r0, r3, 16
    extsh r3, r0
    mr r6, r30
    srwi r0, r3, 31
    li r4, 0x0
    stfd f0, 0x8(r1)
    add r0, r0, r3
    srawi r0, r0, 1
    li r5, -0x1
    lwz r3, 0xc(r1)
    subf r0, r0, r31
    add r3, r3, r0
    bl fn_800FB680
    @L_800164A4:
    bl fn_800FE35C
    li r3, 0x0
    psq_l f31, 0x28(r1), 0, 0
    lwz r0, 0x34(r1)
    lfd f31, 0x20(r1)
    lwz r31, 0x1c(r1)
    lwz r30, 0x18(r1)
    lwz r29, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
