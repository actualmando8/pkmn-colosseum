    nofralloc
    stwu r1, -0xc0(r1)
    mflr r0
    stw r0, 0xc4(r1)
    stfd f31, 0xb0(r1)
    psq_st f31, 0xb8(r1), 0, 0
    stfd f30, 0xa0(r1)
    psq_st f30, 0xa8(r1), 0, 0
    stfd f29, 0x90(r1)
    psq_st f29, 0x98(r1), 0, 0
    stfd f28, 0x80(r1)
    psq_st f28, 0x88(r1), 0, 0
    stfd f27, 0x70(r1)
    psq_st f27, 0x78(r1), 0, 0
    stmw r22, 0x48(r1)
    lfs f27, lbl_8047D7D8(r2)
    mr r30, r3
    mr r31, r4
    li r25, 0x0
    b @L_8018223C
    @L_80182204:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182238
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_80182238
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_80182238
    lwz r25, 0x4(r3)
    b @L_801822A8
    @L_80182238:
    addi r25, r25, 0x1
    @L_8018223C:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80182204
    li r23, 0x0
    b @L_80182298
    @L_80182250:
    mr r3, r23
    bl fn_8018FD88
    mr r25, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182294
    lwz r0, 0x2c(r25)
    cmplw r0, r31
    bne @L_80182294
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r25, 0x4(r25)
    b @L_801822A8
    @L_80182294:
    addi r23, r23, 0x1
    @L_80182298:
    bl fn_8018FDB4
    cmpw r23, r3
    blt @L_80182250
    li r25, 0x0
    @L_801822A8:
    li r26, 0x0
    b @L_801822DC
    @L_801822B0:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801822D8
    lwz r0, 0x4(r3)
    cmplw r0, r25
    bne @L_801822D8
    mr r29, r3
    b @L_801822EC
    @L_801822D8:
    addi r26, r26, 0x1
    @L_801822DC:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_801822B0
    li r29, 0x0
    @L_801822EC:
    cmplwi r29, 0x0
    beq @L_80182FDC
    addic. r0, r1, 0x30
    beq @L_801823F8
    li r25, 0x0
    b @L_8018233C
    @L_80182304:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182338
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_80182338
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_80182338
    lwz r25, 0x4(r3)
    b @L_801823A8
    @L_80182338:
    addi r25, r25, 0x1
    @L_8018233C:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80182304
    li r23, 0x0
    b @L_80182398
    @L_80182350:
    mr r3, r23
    bl fn_8018FD88
    mr r25, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182394
    lwz r0, 0x2c(r25)
    cmplw r0, r31
    bne @L_80182394
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r25, 0x4(r25)
    b @L_801823A8
    @L_80182394:
    addi r23, r23, 0x1
    @L_80182398:
    bl fn_8018FDB4
    cmpw r23, r3
    blt @L_80182350
    li r25, 0x0
    @L_801823A8:
    li r26, 0x0
    b @L_801823D8
    @L_801823B0:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801823D4
    lwz r0, 0x4(r3)
    cmplw r0, r25
    bne @L_801823D4
    b @L_801823E8
    @L_801823D4:
    addi r26, r26, 0x1
    @L_801823D8:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_801823B0
    li r3, 0x0
    @L_801823E8:
    cmplwi r3, 0x0
    beq @L_801823F8
    addi r4, r1, 0x30
    bl fn_8018FC98
    @L_801823F8:
    lfs f28, 0x34(r1)
    li r25, 0x0
    b @L_8018243C
    @L_80182404:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182438
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_80182438
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_80182438
    lwz r25, 0x4(r3)
    b @L_801824A8
    @L_80182438:
    addi r25, r25, 0x1
    @L_8018243C:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80182404
    li r23, 0x0
    b @L_80182498
    @L_80182450:
    mr r3, r23
    bl fn_8018FD88
    mr r25, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182494
    lwz r0, 0x2c(r25)
    cmplw r0, r31
    bne @L_80182494
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r25, 0x4(r25)
    b @L_801824A8
    @L_80182494:
    addi r23, r23, 0x1
    @L_80182498:
    bl fn_8018FDB4
    cmpw r23, r3
    blt @L_80182450
    li r25, 0x0
    @L_801824A8:
    li r26, 0x0
    b @L_801824D8
    @L_801824B0:
    mr r3, r26
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801824D4
    lwz r0, 0x4(r3)
    cmplw r0, r25
    bne @L_801824D4
    b @L_801824E8
    @L_801824D4:
    addi r26, r26, 0x1
    @L_801824D8:
    bl fn_8018FDB4
    cmpw r26, r3
    blt @L_801824B0
    li r3, 0x0
    @L_801824E8:
    cmplwi r3, 0x0
    beq @L_80182608
    li r4, 0x1
    bl fn_8018FB60
    li r28, 0x1
    li r23, 0x0
    b @L_8018253C
    @L_80182504:
    mr r3, r23
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182538
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_80182538
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_80182538
    lwz r23, 0x4(r3)
    b @L_801825A8
    @L_80182538:
    addi r23, r23, 0x1
    @L_8018253C:
    bl fn_8018FDB4
    cmpw r23, r3
    blt @L_80182504
    li r23, 0x0
    b @L_80182598
    @L_80182550:
    mr r3, r23
    bl fn_8018FD88
    mr r25, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182594
    lwz r0, 0x2c(r25)
    cmplw r0, r31
    bne @L_80182594
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r23, 0x4(r25)
    b @L_801825A8
    @L_80182594:
    addi r23, r23, 0x1
    @L_80182598:
    bl fn_8018FDB4
    cmpw r23, r3
    blt @L_80182550
    li r23, 0x0
    @L_801825A8:
    li r24, 0x0
    b @L_801825D8
    @L_801825B0:
    mr r3, r24
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801825D4
    lwz r0, 0x4(r3)
    cmplw r0, r23
    bne @L_801825D4
    b @L_801825E8
    @L_801825D4:
    addi r24, r24, 0x1
    @L_801825D8:
    bl fn_8018FDB4
    cmpw r24, r3
    blt @L_801825B0
    li r3, 0x0
    @L_801825E8:
    cmplwi r3, 0x0
    beq @L_80182608
    lbz r0, 0x21(r3)
    cmplwi r0, 0x0
    bne @L_80182600
    li r28, 0x0
    @L_80182600:
    mr r4, r28
    bl fn_8018FB2C
    @L_80182608:
    mr r3, r29
    li r4, 0x8
    bl fn_8018FBAC
    li r3, 0x49e
    bl fn_80166A28
    li r0, 0x7
    stw r0, 0x90(r29)
    lwz r3, 0x30(r29)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_80182878
    lwz r0, 0x90(r29)
    addi r5, r1, 0x2c
    addi r6, r1, 0xa
    clrlwi r4, r0, 24
    bl fn_8018F4C8
    lwz r27, 0x2c(r1)
    cmpwi r27, -0x1
    beq @L_80182878
    cmpwi r27, 0x0
    lbz r26, 0xa(r1)
    lwz r23, 0x2c(r29)
    li r28, 0x0
    lwz r24, 0x28(r29)
    blt @L_80182878
    mr r25, r28
    b @L_801826AC
    @L_80182674:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801826A8
    lwz r0, 0x28(r3)
    cmplw r0, r24
    bne @L_801826A8
    lwz r0, 0x2c(r3)
    cmplw r0, r23
    bne @L_801826A8
    lwz r23, 0x4(r3)
    b @L_80182718
    @L_801826A8:
    addi r25, r25, 0x1
    @L_801826AC:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80182674
    li r22, 0x0
    b @L_80182708
    @L_801826C0:
    mr r3, r22
    bl fn_8018FD88
    mr r25, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182704
    lwz r0, 0x2c(r25)
    cmplw r0, r23
    bne @L_80182704
    lis r3, lbl_80273FD8@ha
    mr r4, r24
    addi r3, r3, lbl_80273FD8@l
    mr r5, r23
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r23, 0x4(r25)
    b @L_80182718
    @L_80182704:
    addi r22, r22, 0x1
    @L_80182708:
    bl fn_8018FDB4
    cmpw r22, r3
    blt @L_801826C0
    li r23, 0x0
    @L_80182718:
    li r24, 0x0
    b @L_8018274C
    @L_80182720:
    mr r3, r24
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182748
    lwz r0, 0x4(r3)
    cmplw r0, r23
    bne @L_80182748
    mr r23, r3
    b @L_8018275C
    @L_80182748:
    addi r24, r24, 0x1
    @L_8018274C:
    bl fn_8018FDB4
    cmpw r24, r3
    blt @L_80182720
    li r23, 0x0
    @L_8018275C:
    cmplwi r23, 0x0
    beq @L_80182878
    mr r3, r23
    bl fn_8018FBD4
    mr. r25, r3
    beq @L_80182878
    bl fn_800EC954
    clrlwi. r0, r3, 24
    beq @L_80182788
    li r28, 0x1
    b @L_801827CC
    @L_80182788:
    mr r3, r25
    bl fn_800EC960
    clrlwi. r0, r3, 24
    bne @L_801827A0
    li r28, 0x1
    b @L_801827CC
    @L_801827A0:
    mr r3, r25
    addi r4, r1, 0x1c
    addi r5, r1, 0x20
    bl fn_800EC578
    lwz r0, 0x1c(r1)
    cmpw r0, r27
    bne @L_801827C8
    lwz r0, 0x20(r1)
    cmpwi r0, -0x1
    beq @L_801827CC
    @L_801827C8:
    li r28, 0x1
    @L_801827CC:
    clrlwi. r0, r28, 24
    beq @L_80182854
    stw r27, 0x48(r23)
    mr r3, r25
    lfs f0, lbl_8047D7A0(r2)
    mr r4, r27
    stfs f0, 0xd8(r23)
    bl fn_800ECCA8
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r25
    bl fn_800ECA78
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r25
    bl fn_800EC9DC
    mr r3, r25
    mr r4, r27
    bl fn_800EC35C
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r25
    bl fn_800EC2A4
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r25
    bl fn_800EC308
    cmplwi r26, 0x0
    beq @L_80182840
    mr r3, r25
    li r4, 0x1
    bl fn_800ECB74
    b @L_8018284C
    @L_80182840:
    mr r3, r25
    li r4, 0x0
    bl fn_800ECB74
    @L_8018284C:
    mr r3, r25
    bl fn_800EC990
    @L_80182854:
    cmplwi r26, 0x0
    beq @L_8018286C
    mr r3, r25
    li r4, 0x1
    bl fn_800ECB74
    b @L_80182878
    @L_8018286C:
    mr r3, r25
    li r4, 0x0
    bl fn_800ECB74
    @L_80182878:
    lfd f29, lbl_8047D7D0(r2)
    lis r26, 0x4330
    lfd f30, lbl_8047D7E0(r2)
    lfs f31, lbl_8047D7A0(r2)
    @L_80182888:
    bl fn_800D3088
    stw r3, 0x44(r1)
    stw r26, 0x40(r1)
    lfd f0, 0x40(r1)
    fsub f0, f0, f29
    fnmsub f27, f30, f0, f27
    frsp f27, f27
    fcmpo cr0, f27, f31
    bge @L_801828B0
    fmr f27, f31
    @L_801828B0:
    fadds f0, f28, f27
    li r23, 0x0
    stfs f0, 0x34(r1)
    b @L_801828F8
    @L_801828C0:
    mr r3, r23
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_801828F4
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_801828F4
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_801828F4
    lwz r23, 0x4(r3)
    b @L_80182964
    @L_801828F4:
    addi r23, r23, 0x1
    @L_801828F8:
    bl fn_8018FDB4
    cmpw r23, r3
    blt @L_801828C0
    li r22, 0x0
    b @L_80182954
    @L_8018290C:
    mr r3, r22
    bl fn_8018FD88
    mr r25, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182950
    lwz r0, 0x2c(r25)
    cmplw r0, r31
    bne @L_80182950
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r23, 0x4(r25)
    b @L_80182964
    @L_80182950:
    addi r22, r22, 0x1
    @L_80182954:
    bl fn_8018FDB4
    cmpw r22, r3
    blt @L_8018290C
    li r23, 0x0
    @L_80182964:
    li r24, 0x0
    b @L_80182998
    @L_8018296C:
    mr r3, r24
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182994
    lwz r0, 0x4(r3)
    cmplw r0, r23
    bne @L_80182994
    mr r23, r3
    b @L_801829A8
    @L_80182994:
    addi r24, r24, 0x1
    @L_80182998:
    bl fn_8018FDB4
    cmpw r24, r3
    blt @L_8018296C
    li r23, 0x0
    @L_801829A8:
    cmplwi r23, 0x0
    beq @L_801829C8
    mr r3, r23
    addi r4, r1, 0x30
    bl fn_8018FC74
    mr r3, r23
    addi r4, r1, 0x30
    bl fn_8018FBDC
    @L_801829C8:
    fcmpo cr0, f27, f31
    cror eq, lt, eq
    beq @L_801829DC
    bl fn_800F0308
    b @L_80182888
    @L_801829DC:
    li r0, 0x8
    stw r0, 0x90(r29)
    lwz r3, 0x30(r29)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_80182C38
    lwz r0, 0x90(r29)
    addi r5, r1, 0x28
    addi r6, r1, 0x9
    clrlwi r4, r0, 24
    bl fn_8018F4C8
    lwz r27, 0x28(r1)
    cmpwi r27, -0x1
    beq @L_80182C38
    cmpwi r27, 0x0
    lbz r28, 0x9(r1)
    lwz r24, 0x2c(r29)
    li r26, 0x0
    lwz r23, 0x28(r29)
    blt @L_80182C38
    mr r25, r26
    b @L_80182A6C
    @L_80182A34:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182A68
    lwz r0, 0x28(r3)
    cmplw r0, r23
    bne @L_80182A68
    lwz r0, 0x2c(r3)
    cmplw r0, r24
    bne @L_80182A68
    lwz r23, 0x4(r3)
    b @L_80182AD8
    @L_80182A68:
    addi r25, r25, 0x1
    @L_80182A6C:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80182A34
    li r22, 0x0
    b @L_80182AC8
    @L_80182A80:
    mr r3, r22
    bl fn_8018FD88
    mr r25, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182AC4
    lwz r0, 0x2c(r25)
    cmplw r0, r24
    bne @L_80182AC4
    lis r3, lbl_80273FD8@ha
    mr r4, r23
    addi r3, r3, lbl_80273FD8@l
    mr r5, r24
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r23, 0x4(r25)
    b @L_80182AD8
    @L_80182AC4:
    addi r22, r22, 0x1
    @L_80182AC8:
    bl fn_8018FDB4
    cmpw r22, r3
    blt @L_80182A80
    li r23, 0x0
    @L_80182AD8:
    li r24, 0x0
    b @L_80182B0C
    @L_80182AE0:
    mr r3, r24
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182B08
    lwz r0, 0x4(r3)
    cmplw r0, r23
    bne @L_80182B08
    mr r24, r3
    b @L_80182B1C
    @L_80182B08:
    addi r24, r24, 0x1
    @L_80182B0C:
    bl fn_8018FDB4
    cmpw r24, r3
    blt @L_80182AE0
    li r24, 0x0
    @L_80182B1C:
    cmplwi r24, 0x0
    beq @L_80182C38
    mr r3, r24
    bl fn_8018FBD4
    mr. r23, r3
    beq @L_80182C38
    bl fn_800EC954
    clrlwi. r0, r3, 24
    beq @L_80182B48
    li r26, 0x1
    b @L_80182B8C
    @L_80182B48:
    mr r3, r23
    bl fn_800EC960
    clrlwi. r0, r3, 24
    bne @L_80182B60
    li r26, 0x1
    b @L_80182B8C
    @L_80182B60:
    mr r3, r23
    addi r4, r1, 0x14
    addi r5, r1, 0x18
    bl fn_800EC578
    lwz r0, 0x14(r1)
    cmpw r0, r27
    bne @L_80182B88
    lwz r0, 0x18(r1)
    cmpwi r0, -0x1
    beq @L_80182B8C
    @L_80182B88:
    li r26, 0x1
    @L_80182B8C:
    clrlwi. r0, r26, 24
    beq @L_80182C14
    stw r27, 0x48(r24)
    mr r3, r23
    lfs f0, lbl_8047D7A0(r2)
    mr r4, r27
    stfs f0, 0xd8(r24)
    bl fn_800ECCA8
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r23
    bl fn_800ECA78
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r23
    bl fn_800EC9DC
    mr r3, r23
    mr r4, r27
    bl fn_800EC35C
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r23
    bl fn_800EC2A4
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r23
    bl fn_800EC308
    cmplwi r28, 0x0
    beq @L_80182C00
    mr r3, r23
    li r4, 0x1
    bl fn_800ECB74
    b @L_80182C0C
    @L_80182C00:
    mr r3, r23
    li r4, 0x0
    bl fn_800ECB74
    @L_80182C0C:
    mr r3, r23
    bl fn_800EC990
    @L_80182C14:
    cmplwi r28, 0x0
    beq @L_80182C2C
    mr r3, r23
    li r4, 0x1
    bl fn_800ECB74
    b @L_80182C38
    @L_80182C2C:
    mr r3, r23
    li r4, 0x0
    bl fn_800ECB74
    @L_80182C38:
    li r23, 0x0
    b @L_80182C78
    @L_80182C40:
    mr r3, r23
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182C74
    lwz r0, 0x28(r3)
    cmplw r0, r30
    bne @L_80182C74
    lwz r0, 0x2c(r3)
    cmplw r0, r31
    bne @L_80182C74
    lwz r23, 0x4(r3)
    b @L_80182CE4
    @L_80182C74:
    addi r23, r23, 0x1
    @L_80182C78:
    bl fn_8018FDB4
    cmpw r23, r3
    blt @L_80182C40
    li r22, 0x0
    b @L_80182CD4
    @L_80182C8C:
    mr r3, r22
    bl fn_8018FD88
    mr r26, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182CD0
    lwz r0, 0x2c(r26)
    cmplw r0, r31
    bne @L_80182CD0
    lis r3, lbl_80273FD8@ha
    mr r4, r30
    addi r3, r3, lbl_80273FD8@l
    mr r5, r31
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r23, 0x4(r26)
    b @L_80182CE4
    @L_80182CD0:
    addi r22, r22, 0x1
    @L_80182CD4:
    bl fn_8018FDB4
    cmpw r22, r3
    blt @L_80182C8C
    li r23, 0x0
    @L_80182CE4:
    li r24, 0x0
    b @L_80182D14
    @L_80182CEC:
    mr r3, r24
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182D10
    lwz r0, 0x4(r3)
    cmplw r0, r23
    bne @L_80182D10
    b @L_80182D24
    @L_80182D10:
    addi r24, r24, 0x1
    @L_80182D14:
    bl fn_8018FDB4
    cmpw r24, r3
    blt @L_80182CEC
    li r3, 0x0
    @L_80182D24:
    cmplwi r3, 0x0
    beq @L_80182D80
    bl fn_8018FBD4
    mr. r25, r3
    beq @L_80182D80
    @L_80182D38:
    mr r3, r25
    bl fn_800EC954
    clrlwi. r0, r3, 24
    bne @L_80182D80
    lwz r0, 0x8c(r25)
    cmpwi r0, 0x1
    bne @L_80182D78
    lis r3, lbl_80274008@ha
    lis r4, lbl_8036C4F8@ha
    addi r3, r3, lbl_80274008@l
    mr r5, r30
    addi r4, r4, lbl_8036C4F8@l
    mr r6, r31
    crxor 6, 6, 6
    bl fn_800DD970
    b @L_80182D80
    @L_80182D78:
    bl fn_800F0308
    b @L_80182D38
    @L_80182D80:
    li r0, 0x1
    stw r0, 0x90(r29)
    lwz r3, 0x30(r29)
    bl fn_8018F6F4
    cmplwi r3, 0x0
    beq @L_80182FDC
    lwz r0, 0x90(r29)
    addi r5, r1, 0x24
    addi r6, r1, 0x8
    clrlwi r4, r0, 24
    bl fn_8018F4C8
    lwz r27, 0x24(r1)
    cmpwi r27, -0x1
    beq @L_80182FDC
    cmpwi r27, 0x0
    lbz r28, 0x8(r1)
    lwz r24, 0x2c(r29)
    li r26, 0x0
    lwz r23, 0x28(r29)
    blt @L_80182FDC
    mr r25, r26
    b @L_80182E10
    @L_80182DD8:
    mr r3, r25
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182E0C
    lwz r0, 0x28(r3)
    cmplw r0, r23
    bne @L_80182E0C
    lwz r0, 0x2c(r3)
    cmplw r0, r24
    bne @L_80182E0C
    lwz r23, 0x4(r3)
    b @L_80182E7C
    @L_80182E0C:
    addi r25, r25, 0x1
    @L_80182E10:
    bl fn_8018FDB4
    cmpw r25, r3
    blt @L_80182DD8
    li r22, 0x0
    b @L_80182E6C
    @L_80182E24:
    mr r3, r22
    bl fn_8018FD88
    mr r29, r3
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182E68
    lwz r0, 0x2c(r29)
    cmplw r0, r24
    bne @L_80182E68
    lis r3, lbl_80273FD8@ha
    mr r4, r23
    addi r3, r3, lbl_80273FD8@l
    mr r5, r24
    crxor 6, 6, 6
    bl fn_800DD970
    lwz r23, 0x4(r29)
    b @L_80182E7C
    @L_80182E68:
    addi r22, r22, 0x1
    @L_80182E6C:
    bl fn_8018FDB4
    cmpw r22, r3
    blt @L_80182E24
    li r23, 0x0
    @L_80182E7C:
    li r24, 0x0
    b @L_80182EB0
    @L_80182E84:
    mr r3, r24
    bl fn_8018FD88
    lbz r0, 0x0(r3)
    cmplwi r0, 0x0
    beq @L_80182EAC
    lwz r0, 0x4(r3)
    cmplw r0, r23
    bne @L_80182EAC
    mr r24, r3
    b @L_80182EC0
    @L_80182EAC:
    addi r24, r24, 0x1
    @L_80182EB0:
    bl fn_8018FDB4
    cmpw r24, r3
    blt @L_80182E84
    li r24, 0x0
    @L_80182EC0:
    cmplwi r24, 0x0
    beq @L_80182FDC
    mr r3, r24
    bl fn_8018FBD4
    mr. r23, r3
    beq @L_80182FDC
    bl fn_800EC954
    clrlwi. r0, r3, 24
    beq @L_80182EEC
    li r26, 0x1
    b @L_80182F30
    @L_80182EEC:
    mr r3, r23
    bl fn_800EC960
    clrlwi. r0, r3, 24
    bne @L_80182F04
    li r26, 0x1
    b @L_80182F30
    @L_80182F04:
    mr r3, r23
    addi r4, r1, 0xc
    addi r5, r1, 0x10
    bl fn_800EC578
    lwz r0, 0xc(r1)
    cmpw r0, r27
    bne @L_80182F2C
    lwz r0, 0x10(r1)
    cmpwi r0, -0x1
    beq @L_80182F30
    @L_80182F2C:
    li r26, 0x1
    @L_80182F30:
    clrlwi. r0, r26, 24
    beq @L_80182FB8
    stw r27, 0x48(r24)
    mr r3, r23
    lfs f0, lbl_8047D7A0(r2)
    mr r4, r27
    stfs f0, 0xd8(r24)
    bl fn_800ECCA8
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r23
    bl fn_800ECA78
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r23
    bl fn_800EC9DC
    mr r3, r23
    mr r4, r27
    bl fn_800EC35C
    lfs f1, lbl_8047D7A0(r2)
    mr r3, r23
    bl fn_800EC2A4
    lfs f1, lbl_8047D7A4(r2)
    mr r3, r23
    bl fn_800EC308
    cmplwi r28, 0x0
    beq @L_80182FA4
    mr r3, r23
    li r4, 0x1
    bl fn_800ECB74
    b @L_80182FB0
    @L_80182FA4:
    mr r3, r23
    li r4, 0x0
    bl fn_800ECB74
    @L_80182FB0:
    mr r3, r23
    bl fn_800EC990
    @L_80182FB8:
    cmplwi r28, 0x0
    beq @L_80182FD0
    mr r3, r23
    li r4, 0x1
    bl fn_800ECB74
    b @L_80182FDC
    @L_80182FD0:
    mr r3, r23
    li r4, 0x0
    bl fn_800ECB74
    @L_80182FDC:
    psq_l f31, 0xb8(r1), 0, 0
    lfd f31, 0xb0(r1)
    psq_l f30, 0xa8(r1), 0, 0
    lfd f30, 0xa0(r1)
    psq_l f29, 0x98(r1), 0, 0
    lfd f29, 0x90(r1)
    psq_l f28, 0x88(r1), 0, 0
    lfd f28, 0x80(r1)
    psq_l f27, 0x78(r1), 0, 0
    lfd f27, 0x70(r1)
    lmw r22, 0x48(r1)
    lwz r0, 0xc4(r1)
    mtlr r0
    addi r1, r1, 0xc0
    blr
