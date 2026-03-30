    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stfd f31, 0x70(r1)
    psq_st f31, 0x78(r1), 0, 0
    stfd f30, 0x60(r1)
    psq_st f30, 0x68(r1), 0, 0
    stfd f29, 0x50(r1)
    psq_st f29, 0x58(r1), 0, 0
    stfd f28, 0x40(r1)
    psq_st f28, 0x48(r1), 0, 0
    stfd f27, 0x30(r1)
    psq_st f27, 0x38(r1), 0, 0
    stmw r22, 0x8(r1)
    mr r23, r4
    lhz r22, 0x32(r3)
    lfs f1, -21568(r2)
    mr r25, r5
    lfs f0, 0x4(r4)
    mulli r0, r22, 0xd4
    lwz r3, 0x2c(r3)
    mr r24, r6
    fmuls f29, f1, f0
    lfs f0, 0x8(r4)
    add r29, r3, r0
    lwz r30, 0x0(r29)
    fcmpo cr0, f0, f29
    lfs f31, -21668(r2)
    lfs f30, -21676(r2)
    lfs f28, 0x28(r4)
    ble @L_801D4544
    fmr f29, f0
@L_801D4544:
    lfs f0, -21672(r2)
    fmr f2, f28
    fmuls f1, f0, f29
    bl fn_800CE2D8
    frsp f4, f1
    lfs f0, -21548(r2)
    lfs f3, -21552(r2)
    fmr f2, f28
    fmuls f1, f0, f29
    fmuls f0, f0, f4
    fmuls f27, f3, f0
    bl fn_800CE2D8
    frsp f3, f1
    lfs f1, -21548(r2)
    lfs f0, -21544(r2)
    lfs f2, -21552(r2)
    fmuls f1, f1, f3
    fcmpo cr0, f27, f0
    fmuls f28, f2, f1
    bge @L_801D4598
    fmr f27, f0
@L_801D4598:
    lfs f0, -21544(r2)
    fcmpo cr0, f28, f0
    bge @L_801D45A8
    fmr f28, f0
@L_801D45A8:
    lfs f0, -21540(r2)
    fcmpo cr0, f27, f0
    ble @L_801D45B8
    fmr f27, f0
@L_801D45B8:
    lfs f0, -21540(r2)
    fcmpo cr0, f28, f0
    ble @L_801D45C8
    fmr f28, f0
@L_801D45C8:
    lwz r0, 0x0(r23)
    cmpwi r0, 0x4
    bge @L_801D45E0
    cmpwi r0, 0x0
    beq @L_801D45E8
    b @L_801D46BC
@L_801D45E0:
    cmpwi r0, 0x6
    bge @L_801D46BC
@L_801D45E8:
    rlwinm. r0, r25, 0, 26, 26
    beq @L_801D45F8
    li r5, 0x1
    b @L_801D460C
@L_801D45F8:
    rlwinm. r0, r25, 0, 24, 24
    beq @L_801D4608
    li r5, 0x4
    b @L_801D460C
@L_801D4608:
    li r5, 0x2
@L_801D460C:
    rlwinm r0, r5, 0, 31, 31
    li r4, 0x0
    cmpwi r0, 0x1
    mr r3, r4
    bne @L_801D462C
    lfs f31, -21668(r2)
    li r4, 0x1
    lfs f30, -21696(r2)
@L_801D462C:
    rlwinm r0, r5, 0, 30, 30
    cmpwi r0, 0x2
    bne @L_801D464C
    rlwinm. r0, r4, 0, 24, 31
    bne @L_801D4644
    lfs f31, -21536(r2)
@L_801D4644:
    lfs f30, -21532(r2)
    li r3, 0x1
@L_801D464C:
    rlwinm r0, r5, 0, 29, 29
    cmpwi r0, 0x4
    bne @L_801D4668
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D4664
    lfs f31, -21568(r2)
@L_801D4664:
    lfs f30, -21676(r2)
@L_801D4668:
    bl fn_800E0BE4
    fsubs f2, f30, f31
    lis r3, -32698
    fsubs f0, f28, f27
    addi r3, r3, 29652
    fmadds f1, f2, f1, f31
    fmadds f1, f1, f0, f27
    stfs f1, 0x14(r3)
    stfs f1, 0x10(r3)
    stfs f1, 0x4(r3)
    stfs f1, 0x0(r3)
    stfs f1, -31556(r13)
    lwz r0, 0xc(r29)
    slw r0, r0, r24
    stw r0, 0x1c(r3)
    stw r0, 0x18(r3)
    stw r0, 0xc(r3)
    stw r0, 0x8(r3)
    stw r0, -21560(r13)
    bl fn_801766A8
    b @L_801D499C
@L_801D46BC:
    cmpwi r22, 0x8
    li r0, 0x0
    beq @L_801D46D4
    cmpwi r22, 0x9
    beq @L_801D46D4
    li r0, 0x1
@L_801D46D4:
    mr r4, r25
    rlwinm r3, r0, 0, 24, 31
    bl fn_801D2B4C
    addi r25, r3, 0x4
    li r4, 0x0
    lwz r5, 0x1c(r3)
    mr r3, r4
    rlwinm r0, r5, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_801D4708
    lfs f31, -21668(r2)
    li r4, 0x1
    lfs f30, -21696(r2)
@L_801D4708:
    rlwinm r0, r5, 0, 30, 30
    cmpwi r0, 0x2
    bne @L_801D4728
    rlwinm. r0, r4, 0, 24, 31
    bne @L_801D4720
    lfs f31, -21536(r2)
@L_801D4720:
    lfs f30, -21532(r2)
    li r3, 0x1
@L_801D4728:
    rlwinm r0, r5, 0, 29, 29
    cmpwi r0, 0x4
    bne @L_801D4744
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D4740
    lfs f31, -21568(r2)
@L_801D4740:
    lfs f30, -21676(r2)
@L_801D4744:
    bl fn_800E0BE4
    fsubs f2, f30, f31
    lis r3, -32698
    fsubs f0, f28, f27
    addi r3, r3, 29652
    fmadds f1, f2, f1, f31
    fmadds f27, f1, f0, f27
    stfs f27, 0x14(r3)
    fmr f1, f27
    stfs f27, 0x10(r3)
    stfs f27, 0x4(r3)
    stfs f27, 0x0(r3)
    stfs f27, -31556(r13)
    lwz r0, 0xc(r29)
    slw r0, r0, r24
    stw r0, 0x1c(r3)
    stw r0, 0x18(r3)
    stw r0, 0xc(r3)
    stw r0, 0x8(r3)
    stw r0, -21560(r13)
    bl fn_801766A8
    cmpwi r30, 0x2
    ble @L_801D499C
    fmr f29, f27
    lis r3, -32698
    lwz r27, 0xc(r29)
    mr r31, r29
    addi r30, r3, 29652
    mr r29, r23
    li r26, 0x0
@L_801D47BC:
    mr r28, r27
    lwz r27, 0x10(r31)
    cmpw r28, r27
    bne @L_801D47E4
    slw r0, r28, r24
    stw r0, 0x8(r30)
    stw r0, 0xc(r30)
    stfs f29, 0x0(r30)
    stfs f29, 0x4(r30)
    b @L_801D4980
@L_801D47E4:
    lwz r0, 0x0(r25)
    cmpwi r0, 0x3
    bge @L_801D47F4
    b @L_801D4968
@L_801D47F4:
    cmpwi r0, 0x5
    bge @L_801D4968
    lwz r3, 0x4(r25)
    addi r4, r25, 0x8
    subf r5, r28, r27
    bl fn_801D49D8
    lfs f1, -21568(r2)
    mr r22, r3
    lfs f0, 0x4(r23)
    lfs f2, 0x8(r23)
    fmuls f28, f1, f0
    lfs f27, 0x2c(r29)
    fcmpo cr0, f2, f28
    ble @L_801D4830
    fmr f28, f2
@L_801D4830:
    lfs f0, -21672(r2)
    fmr f2, f27
    fmuls f1, f0, f28
    bl fn_800CE2D8
    frsp f4, f1
    lfs f0, -21548(r2)
    lfs f3, -21552(r2)
    fmr f2, f27
    fmuls f1, f0, f28
    fmuls f0, f0, f4
    fmuls f27, f3, f0
    bl fn_800CE2D8
    frsp f3, f1
    lfs f1, -21548(r2)
    lfs f0, -21544(r2)
    lfs f2, -21552(r2)
    fmuls f1, f1, f3
    fcmpo cr0, f27, f0
    fmuls f28, f2, f1
    bge @L_801D4884
    fmr f27, f0
@L_801D4884:
    lfs f0, -21544(r2)
    fcmpo cr0, f28, f0
    bge @L_801D4894
    fmr f28, f0
@L_801D4894:
    lfs f0, -21540(r2)
    fcmpo cr0, f27, f0
    ble @L_801D48A4
    fmr f27, f0
@L_801D48A4:
    lfs f0, -21540(r2)
    fcmpo cr0, f28, f0
    ble @L_801D48B4
    fmr f28, f0
@L_801D48B4:
    lwz r5, 0x1c(r25)
    li r4, 0x0
    mr r3, r4
    rlwinm r0, r5, 0, 31, 31
    cmpwi r0, 0x1
    bne @L_801D48D8
    lfs f31, -21668(r2)
    li r4, 0x1
    lfs f30, -21696(r2)
@L_801D48D8:
    rlwinm r0, r5, 0, 30, 30
    cmpwi r0, 0x2
    bne @L_801D48F8
    rlwinm. r0, r4, 0, 24, 31
    bne @L_801D48F0
    lfs f31, -21536(r2)
@L_801D48F0:
    lfs f30, -21532(r2)
    li r3, 0x1
@L_801D48F8:
    rlwinm r0, r5, 0, 29, 29
    cmpwi r0, 0x4
    bne @L_801D4914
    rlwinm. r0, r3, 0, 24, 31
    bne @L_801D4910
    lfs f31, -21568(r2)
@L_801D4910:
    lfs f30, -21676(r2)
@L_801D4914:
    bl fn_800E0BE4
    fsubs f2, f30, f31
    lwz r0, 0x20(r25)
    fsubs f0, f28, f27
    cmpwi r0, 0x2
    fmadds f1, f2, f1, f31
    fmadds f0, f1, f0, f27
    bne @L_801D4940
    mr r4, r27
    subf r0, r22, r27
    b @L_801D4948
@L_801D4940:
    mr r0, r28
    add r4, r28, r22
@L_801D4948:
    slw r3, r0, r24
    slw r0, r4, r24
    stw r3, 0x8(r30)
    stw r0, 0xc(r30)
    stfs f29, 0x0(r30)
    fmr f29, f0
    stfs f0, 0x4(r30)
    b @L_801D4980
@L_801D4968:
    slw r3, r28, r24
    slw r0, r27, r24
    stw r3, 0x8(r30)
    stw r0, 0xc(r30)
    stfs f29, 0x0(r30)
    stfs f29, 0x4(r30)
@L_801D4980:
    addi r26, r26, 0x1
    addi r30, r30, 0x10
    cmpwi r26, 0x2
    addi r29, r29, 0x4
    addi r25, r25, 0x24
    addi r31, r31, 0x4
    blt @L_801D47BC
@L_801D499C:
    psq_l f31, 0x78(r1), 0, 0
    lfd f31, 0x70(r1)
    psq_l f30, 0x68(r1), 0, 0
    lfd f30, 0x60(r1)
    psq_l f29, 0x58(r1), 0, 0
    lfd f29, 0x50(r1)
    psq_l f28, 0x48(r1), 0, 0
    lfd f28, 0x40(r1)
    psq_l f27, 0x38(r1), 0, 0
    lfd f27, 0x30(r1)
    lmw r22, 0x8(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
