    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    stmw r18, 0x18(r1)
    mr r28, r3
    mr r21, r5
    mulli r23, r4, 0x4c
    lis r3, lbl_80266918@ha
    li r25, 0x0
    addi r0, r3, lbl_80266918@l
    add r3, r0, r23
    li r22, 0x8
    lwz r0, 0x4(r3)
    li r24, 0x0
    cmpwi r0, 0x0
    blt @L_80015610
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0xe
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80015628
    @L_80015610:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xe
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80015628:
    li r26, 0x0
    mr r29, r3
    mr r27, r26
    b @L_80015658
    @L_80015638:
    mr r3, r29
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80015650
    addi r26, r26, 0x1
    @L_80015650:
    addi r27, r27, 0x1
    addi r29, r29, 0x4
    @L_80015658:
    lhz r0, 0xe(r1)
    cmpw r27, r0
    blt @L_80015638
    lfs f0, lbl_8047B748(r2)
    lfs f1, lbl_8047A2D0(r13)
    lbz r29, 0x0(r21)
    fcmpu cr0, f0, f1
    extsb r29, r29
    beq @L_800156AC
    lwz r0, lbl_8047A2C8(r13)
    cmpwi r0, 0x0
    beq @L_800156AC
    fcmpo cr0, f1, f0
    bge @L_8001569C
    subi r29, r29, 0x1
    li r25, -0x1
    b @L_800156A0
    @L_8001569C:
    li r22, 0x9
    @L_800156A0:
    fctiwz f0, f1
    stfd f0, 0x10(r1)
    lwz r24, 0x14(r1)
    @L_800156AC:
    lis r3, lbl_80266918@ha
    mr r27, r25
    addi r0, r3, lbl_80266918@l
    mr r21, r28
    add r3, r0, r23
    mulli r25, r25, 0x1f
    addi r31, r3, 0x4
    b @L_80015974
    @L_800156CC:
    cmpwi r29, 0x0
    blt @L_80015968
    subf r30, r24, r25
    mr r3, r28
    mr r4, r30
    li r5, -0x1
    li r6, 0x2aff
    bl fn_800FB680
    li r3, 0x2aff
    bl fn_800FA444
    lwz r4, 0x0(r31)
    srwi r0, r3, 16
    add r23, r28, r0
    cmpwi r4, 0x0
    blt @L_80015728
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r4, 24
    addi r5, r1, 0xc
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_80015740
    @L_80015728:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0xc
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_80015740:
    mr r18, r3
    li r20, -0x1
    li r19, 0x0
    b @L_80015788
    @L_80015750:
    mr r3, r18
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_80015780
    addi r20, r20, 0x1
    cmpw r20, r29
    blt @L_80015780
    mr r3, r18
    bl fn_80143C68
    mr r20, r3
    b @L_80015798
    @L_80015780:
    addi r19, r19, 0x1
    addi r18, r18, 0x4
    @L_80015788:
    lhz r0, 0xc(r1)
    cmpw r19, r0
    blt @L_80015750
    li r20, 0x0
    @L_80015798:
    mr r3, r20
    bl fn_80143E60
    addi r0, r3, 0x1
    clrlwi r18, r0, 24
    cmplwi r18, 0xa
    bge @L_800157E0
    li r3, 0x34
    li r4, 0x0
    bl fn_80132A38
    mr r3, r23
    mr r4, r30
    li r5, -0x1
    li r6, 0xca
    bl fn_800FB680
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    add r23, r23, r0
    @L_800157E0:
    mr r4, r18
    li r3, 0x34
    bl fn_80132A38
    mr r3, r23
    mr r4, r30
    li r5, -0x1
    li r6, 0xca
    bl fn_800FB680
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    clrlwi r3, r20, 16
    add r23, r23, r0
    bl fn_801440A0
    bl fn_80144088
    mr r6, r3
    mr r4, r30
    addi r3, r23, 0x9
    li r5, -0x1
    bl fn_800FB680
    lwz r0, 0x0(r31)
    addi r6, r1, 0xa
    lwz r3, lbl_8047A2F8(r13)
    li r5, 0x0
    clrlwi r4, r0, 24
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    lhz r4, 0xa(r1)
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r0, r3, 16
    li r3, 0x12e
    extsh r18, r0
    bl fn_800FA444
    srwi r3, r3, 16
    addi r0, r28, 0x11a
    extsh r3, r3
    mr r4, r30
    add r18, r18, r3
    li r5, -0x1
    extsh r3, r18
    li r6, 0x12e
    subf r3, r3, r0
    bl fn_800FB680
    lwz r0, 0x0(r31)
    cmpwi r0, 0x0
    blt @L_800158C8
    lwz r3, lbl_8047A2F8(r13)
    clrlwi r4, r0, 24
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    b @L_800158E0
    @L_800158C8:
    lwz r3, lbl_8047A2F8(r13)
    addi r4, r1, 0x8
    li r5, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_801297D8
    @L_800158E0:
    mr r20, r3
    li r18, -0x1
    li r19, 0x0
    b @L_80015924
    @L_800158F0:
    mr r3, r20
    bl fn_801429E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x0
    beq @L_8001591C
    addi r18, r18, 0x1
    cmpw r18, r29
    blt @L_8001591C
    mr r3, r20
    bl fn_80143C50
    b @L_80015934
    @L_8001591C:
    addi r19, r19, 0x1
    addi r20, r20, 0x4
    @L_80015924:
    lhz r0, 0x8(r1)
    cmpw r19, r0
    blt @L_800158F0
    li r3, 0x0
    @L_80015934:
    clrlwi r4, r3, 16
    li r3, 0x34
    bl fn_80132A38
    li r3, 0xca
    bl fn_800FA444
    srwi r3, r3, 16
    addi r0, r28, 0x11a
    extsh r3, r3
    mr r4, r30
    subf r3, r3, r0
    li r5, -0x1
    li r6, 0xca
    bl fn_800FB680
    @L_80015968:
    addi r25, r25, 0x1f
    addi r27, r27, 0x1
    addi r29, r29, 0x1
    @L_80015974:
    cmpw r27, r22
    bge @L_80015984
    cmpw r29, r26
    blt @L_800156CC
    @L_80015984:
    cmpwi r27, 0x8
    bge @L_800159A4
    mulli r0, r27, 0x1f
    mr r3, r21
    li r5, -0x1
    li r6, 0x2afe
    subf r4, r24, r0
    bl fn_800FB680
    @L_800159A4:
    li r3, 0x0
    lmw r18, 0x18(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
