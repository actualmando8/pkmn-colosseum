    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stmw r27, 0x5c(r1)
    mr. r30, r3
    lis r6, 0x8027
    mr r27, r4
    mr r29, r5
    addi r31, r6, 19104
    beq @L_801A2B40
    cmplwi r27, 0x39
    bgt @L_801A2B40
    lis r3, 0x8037
    slwi r0, r27, 2
    addi r3, r3, -14028
    lwzx r3, r3, r0
    mtctr r3
    bctr
    lfs f1, 0x0(r29)
    lfd f0, -23360(r2)
    fcmpo cr0, f1, f0
    bge @L_801A2130
    lfs f0, -23384(r2)
    stfs f0, 0x0(r29)
@L_801A2130:
    lfd f1, -23312(r2)
    lfs f0, 0x0(r29)
    fcmpo cr0, f1, f0
    bge @L_801A2148
    lfs f0, -23408(r2)
    stfs f0, 0x0(r29)
@L_801A2148:
    lwz r0, 0x7c(r30)
    cmplwi r0, 0x0
    bne @L_801A2164
    addi r5, r31, 0x260
    addi r3, r2, -23424
    li r4, 0x24e
    bl fn_80196E10
@L_801A2164:
    lwz r3, 0x7c(r30)
    lwz r28, 0x18(r3)
    cmplwi r28, 0x0
    bne @L_801A2184
    addi r3, r2, -23424
    li r4, 0x250
    addi r5, r2, -23320
    bl fn_80196E10
@L_801A2184:
    lwz r0, 0x18(r28)
    cmplwi r0, 0x0
    bne @L_801A21A0
    addi r5, r31, 0x26c
    addi r3, r2, -23424
    li r4, 0x251
    bl fn_80196E10
@L_801A21A0:
    lwz r4, 0x18(r28)
    addi r3, r1, 0x8
    lfs f1, 0x0(r29)
    bl fn_801B1890
    cmplwi r30, 0x0
    lfs f31, 0x8(r1)
    bne @L_801A21CC
    addi r3, r2, -23404
    li r4, 0x3b8
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A21CC:
    stfs f31, 0x38(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A222C
    cmplwi r30, 0x0
    beq @L_801A222C
    cmplwi r30, 0x0
    bne @L_801A21FC
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A21FC:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A221C
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A221C
    li r3, 0x1
@L_801A221C:
    cmpwi r3, 0x0
    bne @L_801A222C
    mr r3, r30
    bl fn_8019D620
@L_801A222C:
    cmplwi r30, 0x0
    lfs f31, 0xc(r1)
    bne @L_801A2248
    addi r3, r2, -23404
    li r4, 0x3c6
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2248:
    stfs f31, 0x3c(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A22A8
    cmplwi r30, 0x0
    beq @L_801A22A8
    cmplwi r30, 0x0
    bne @L_801A2278
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2278:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2298
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2298
    li r3, 0x1
@L_801A2298:
    cmpwi r3, 0x0
    bne @L_801A22A8
    mr r3, r30
    bl fn_8019D620
@L_801A22A8:
    cmplwi r30, 0x0
    lfs f31, 0x10(r1)
    bne @L_801A22C4
    addi r3, r2, -23404
    li r4, 0x3d4
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A22C4:
    stfs f31, 0x40(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A22F4
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A22F4:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2314
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2314
    li r3, 0x1
@L_801A2314:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 10, 10
    beq @L_801A2354
    lwz r3, 0x80(r30)
    lis r4, 0x4000
    li r5, 0x0
    bl fn_801B00E0
    cmplwi r3, 0x0
    beq @L_801A2354
    lfs f0, 0x0(r29)
    stfs f0, 0xc(r3)
@L_801A2354:
    cmplwi r30, 0x0
    lfs f31, 0x0(r29)
    bne @L_801A2370
    addi r3, r2, -23404
    li r4, 0x2a4
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2370:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_801A238C
    addi r5, r31, 0x27c
    addi r3, r2, -23404
    li r4, 0x2a5
    bl fn_80196E10
@L_801A238C:
    stfs f31, 0x1c(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A23BC
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A23BC:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A23DC
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A23DC
    li r3, 0x1
@L_801A23DC:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    cmplwi r30, 0x0
    lfs f31, 0x0(r29)
    bne @L_801A240C
    addi r3, r2, -23404
    li r4, 0x2b8
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A240C:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_801A2428
    addi r5, r31, 0x27c
    addi r3, r2, -23404
    li r4, 0x2b9
    bl fn_80196E10
@L_801A2428:
    stfs f31, 0x20(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A2458
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2458:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2478
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2478
    li r3, 0x1
@L_801A2478:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    cmplwi r30, 0x0
    lfs f31, 0x0(r29)
    bne @L_801A24A8
    addi r3, r2, -23404
    li r4, 0x2cc
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A24A8:
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 14, 14
    beq @L_801A24C4
    addi r5, r31, 0x27c
    addi r3, r2, -23404
    li r4, 0x2cd
    bl fn_80196E10
@L_801A24C4:
    stfs f31, 0x24(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A24F4
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A24F4:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2514
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2514
    li r3, 0x1
@L_801A2514:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    cmplwi r30, 0x0
    lfs f31, 0x0(r29)
    bne @L_801A2544
    addi r3, r2, -23404
    li r4, 0x3b8
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2544:
    stfs f31, 0x38(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A2574
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2574:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2594
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2594
    li r3, 0x1
@L_801A2594:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    cmplwi r30, 0x0
    lfs f31, 0x0(r29)
    bne @L_801A25C4
    addi r3, r2, -23404
    li r4, 0x3c6
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A25C4:
    stfs f31, 0x3c(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A25F4
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A25F4:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2614
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2614
    li r3, 0x1
@L_801A2614:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    cmplwi r30, 0x0
    lfs f31, 0x0(r29)
    bne @L_801A2644
    addi r3, r2, -23404
    li r4, 0x3d4
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2644:
    stfs f31, 0x40(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A2674
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2674:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2694
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2694
    li r3, 0x1
@L_801A2694:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    cmplwi r30, 0x0
    lfs f31, 0x0(r29)
    bne @L_801A26C4
    addi r3, r2, -23404
    li r4, 0x325
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A26C4:
    stfs f31, 0x2c(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A26F4
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A26F4:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2714
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2714
    li r3, 0x1
@L_801A2714:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    cmplwi r30, 0x0
    lfs f31, 0x0(r29)
    bne @L_801A2744
    addi r3, r2, -23404
    li r4, 0x333
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2744:
    stfs f31, 0x30(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A2774
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2774:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2794
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2794
    li r3, 0x1
@L_801A2794:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    cmplwi r30, 0x0
    lfs f31, 0x0(r29)
    bne @L_801A27C4
    addi r3, r2, -23404
    li r4, 0x341
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A27C4:
    stfs f31, 0x34(r30)
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 6, 6
    bne @L_801A2B40
    cmplwi r30, 0x0
    beq @L_801A2B40
    cmplwi r30, 0x0
    bne @L_801A27F4
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A27F4:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2814
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2814
    li r3, 0x1
@L_801A2814:
    cmpwi r3, 0x0
    bne @L_801A2B40
    mr r3, r30
    bl fn_8019D620
    b @L_801A2B40
    lfs f1, 0x0(r29)
    lfd f0, -23376(r2)
    fcmpo cr0, f1, f0
    ble @L_801A2848
    mr r3, r30
    li r4, 0x10
    bl fn_8019F7F0
    b @L_801A2B40
@L_801A2848:
    mr r3, r30
    li r4, 0x10
    bl fn_8019FB90
    b @L_801A2B40
    lfs f1, 0x0(r29)
    lfd f0, -23376(r2)
    fcmpo cr0, f1, f0
    ble @L_801A28E0
    cmplwi r30, 0x0
    beq @L_801A2B40
    lwz r0, 0x14(r30)
    xori r0, r0, 0x10
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801A28D0
    cmplwi r30, 0x0
    beq @L_801A28D0
    cmplwi r30, 0x0
    bne @L_801A28A0
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A28A0:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A28C0
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A28C0
    li r3, 0x1
@L_801A28C0:
    cmpwi r3, 0x0
    bne @L_801A28D0
    mr r3, r30
    bl fn_8019D620
@L_801A28D0:
    lwz r0, 0x14(r30)
    rlwinm r0, r0, 0, 28, 26
    stw r0, 0x14(r30)
    b @L_801A2B40
@L_801A28E0:
    cmplwi r30, 0x0
    beq @L_801A2B40
    lwz r0, 0x14(r30)
    xori r0, r0, 0x10
    rlwinm. r0, r0, 0, 28, 28
    beq @L_801A2948
    cmplwi r30, 0x0
    beq @L_801A2948
    cmplwi r30, 0x0
    bne @L_801A2918
    addi r3, r2, -23404
    li r4, 0x25d
    addi r5, r2, -23396
    bl fn_80196E10
@L_801A2918:
    lwz r0, 0x14(r30)
    li r3, 0x0
    rlwinm. r0, r0, 0, 8, 8
    bne @L_801A2938
    lwz r0, 0x14(r30)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_801A2938
    li r3, 0x1
@L_801A2938:
    cmpwi r3, 0x0
    bne @L_801A2948
    mr r3, r30
    bl fn_8019D620
@L_801A2948:
    lwz r0, 0x14(r30)
    ori r0, r0, 0x10
    stw r0, 0x14(r30)
    b @L_801A2B40
    lwz r28, -21892(r13)
    b @L_801A2998
@L_801A2960:
    lwz r4, 0x0(r29)
    lis r0, 0x4330
    stw r0, 0x48(r1)
    mr r3, r30
    xoris r0, r4, 0x8000
    lfd f1, -23304(r2)
    stw r0, 0x4c(r1)
    mr r4, r27
    lwz r12, 0x4(r28)
    lfd f0, 0x48(r1)
    fsubs f1, f0, f1
    mtctr r12
    bctrl
    lwz r28, 0x0(r28)
@L_801A2998:
    cmplwi r28, 0x0
    bne @L_801A2960
    b @L_801A2B40
    lwz r28, -21892(r13)
    b @L_801A29C8
@L_801A29AC:
    lwz r12, 0x4(r28)
    mr r3, r30
    mr r4, r27
    lfs f1, 0x0(r29)
    mtctr r12
    bctrl
    lwz r28, 0x0(r28)
@L_801A29C8:
    cmplwi r28, 0x0
    bne @L_801A29AC
    b @L_801A2B40
    lwz r0, -21888(r13)
    lwz r3, 0x0(r29)
    lwz r4, 0x0(r29)
    cmplwi r0, 0x0
    extrwi r5, r3, 24, 2
    clrlwi r4, r4, 26
    beq @L_801A2B40
    lwz r12, -21888(r13)
    mr r6, r30
    li r3, 0x0
    mtctr r12
    bctrl
    b @L_801A2B40
    lwz r0, -21884(r13)
    cmplwi r0, 0x0
    beq @L_801A2B40
    lwz r12, -21884(r13)
    lwz r3, 0x0(r29)
    mtctr r12
    bctrl
    b @L_801A2B40
    lwz r0, -21880(r13)
    cmplwi r0, 0x0
    beq @L_801A2B40
    lwz r12, -21880(r13)
    mr r3, r30
    lwz r4, 0x0(r29)
    mtctr r12
    bctrl
    b @L_801A2B40
    lfs f0, 0x0(r29)
    stfs f0, 0x44(r30)
    lfs f0, 0x4(r29)
    stfs f0, 0x54(r30)
    lfs f0, 0x8(r29)
    stfs f0, 0x64(r30)
    b @L_801A2B40
    lfs f0, 0x0(r29)
    stfs f0, 0x48(r30)
    lfs f0, 0x4(r29)
    stfs f0, 0x58(r30)
    lfs f0, 0x8(r29)
    stfs f0, 0x68(r30)
    b @L_801A2B40
    lfs f0, 0x0(r29)
    stfs f0, 0x4c(r30)
    lfs f0, 0x4(r29)
    stfs f0, 0x5c(r30)
    lfs f0, 0x8(r29)
    stfs f0, 0x6c(r30)
    b @L_801A2B40
    lfs f0, 0x0(r29)
    stfs f0, 0x50(r30)
    lfs f0, 0x4(r29)
    stfs f0, 0x60(r30)
    lfs f0, 0x8(r29)
    stfs f0, 0x70(r30)
    b @L_801A2B40
    lwz r0, 0xc(r30)
    cmplwi r0, 0x0
    beq @L_801A2AE0
    lwz r3, 0xc(r30)
    addi r4, r30, 0x44
    addi r5, r1, 0x14
    addi r3, r3, 0x44
    bl fn_801A9DF0
    b @L_801A2AEC
@L_801A2AE0:
    addi r3, r30, 0x44
    addi r4, r1, 0x14
    bl fn_800A2D64
@L_801A2AEC:
    cmplwi r27, 0x36
    beq @L_801A2AFC
    cmplwi r27, 0x38
    bne @L_801A2B08
@L_801A2AFC:
    addi r3, r1, 0x14
    addi r4, r30, 0x38
    bl fn_801A9570
@L_801A2B08:
    cmplwi r27, 0x36
    beq @L_801A2B18
    cmplwi r27, 0x37
    bne @L_801A2B24
@L_801A2B18:
    addi r3, r1, 0x14
    addi r4, r30, 0x1c
    bl fn_801A98CC
@L_801A2B24:
    cmplwi r27, 0x36
    beq @L_801A2B34
    cmplwi r27, 0x39
    bne @L_801A2B40
@L_801A2B34:
    addi r3, r1, 0x14
    addi r4, r30, 0x2c
    bl fn_801A8D1C
@L_801A2B40:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    lmw r27, 0x5c(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
