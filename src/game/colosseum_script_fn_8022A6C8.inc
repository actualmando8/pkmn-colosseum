    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x54(r1)
    li r6, 0x0
    stmw r21, 0x24(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r31
    bl fn_80205B8C
    mr r22, r3
    mr r4, r31
    li r3, 0x2
    bl fn_801F025C
    mr r0, r3
    mr r3, r31
    mr r25, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r24, r0
    bl fn_80203E0C
    mr r4, r31
    li r3, 0x0
    bl fn_801F4354
    mr. r29, r3
    bne @L_8022A754
    li r3, 0x0
    b @L_8022B288
@L_8022A754:
    mr r4, r29
    li r3, 0x9
    bl fn_801F025C
    mr r0, r3
    mr r3, r29
    mr r28, r0
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r31
    bl fn_80205184
    mr r21, r3
    mr r3, r31
    bl fn_802040E8
    mr r0, r3
    mr r3, r31
    mr r23, r0
    bl fn_80203FE4
    mr r3, r31
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0xbf
    li r6, 0x0
    bl fn_8012640C
    rlwinm r27, r3, 0, 24, 31
    mr r3, r24
    li r4, 0x0
    li r5, 0x32
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8022B284
    li r3, 0x0
    li r4, 0x0
    li r5, 0x31
    li r6, 0x0
    bl fn_801F54A4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022B284
    mr r3, r31
    bl fn_80205B8C
    bl fn_8011FC74
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022B284
    mr r3, r31
    li r4, 0x3e
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022B284
    rlwinm r0, r21, 0, 16, 31
    cmplwi r0, 0x164
    beq @L_8022B284
    mr r3, r29
    bl fn_801F8100
    mr r4, r3
    li r3, 0x13
    bl fn_80132A38
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    li r4, 0x64
    divw r0, r5, r4
    mr r3, r27
    mullw r0, r0, r4
    subf r26, r0, r5
    bl fn_8011CE18
    bl fn_8011CCE4
    rlwinm r21, r3, 0, 24, 31
    cmplw r26, r21
    bge @L_8022AAF0
    addi r3, r1, 0xc
    li r5, 0x0
    li r4, -0x1
    b @L_8022A88C
@L_8022A880:
    clrlslwi r0, r5, 24, 1
    addi r5, r5, 0x1
    sthx r4, r3, r0
@L_8022A88C:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8022A880
    li r23, 0x0
    li r25, 0x0
    b @L_8022A918
@L_8022A8A4:
    rlwinm r21, r25, 0, 16, 31
    mr r3, r22
    mr r4, r21
    bl fn_80123CD4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022A914
    mr r3, r31
    mr r4, r21
    li r5, 0x0
    li r6, 0x0
    bl fn_801FFEC8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022A914
    mr r3, r22
    mr r6, r21
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm. r0, r3, 0, 16, 31
    beq @L_8022A914
    cmplwi r0, 0x165
    beq @L_8022A914
    cmplwi r0, 0x163
    beq @L_8022A914
    clrlslwi r0, r23, 24, 1
    addi r3, r1, 0xc
    sthx r25, r3, r0
    addi r23, r23, 0x1
@L_8022A914:
    addi r25, r25, 0x1
@L_8022A918:
    extsh r0, r25
    cmpwi r0, 0x4
    blt @L_8022A8A4
    rlwinm. r0, r23, 0, 24, 31
    beq @L_8022AE74
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    rlwinm r4, r23, 0, 24, 31
    divw r0, r5, r4
    addi r3, r1, 0xc
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 24, 1
    lhax r21, r3, r0
    extsh. r0, r21
    blt @L_8022AE74
    mr r3, r22
    rlwinm r6, r21, 0, 16, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm. r22, r3, 0, 16, 31
    beq @L_8022AE74
    cmplwi r22, 0x165
    beq @L_8022AE74
    cmplwi r22, 0x163
    beq @L_8022AE74
    lwz r0, lbl_8047B618(r13)
    lis r3, -32733
    addi r6, r3, -19812
    sth r22, lbl_8047B60C(r13)
    rlwinm r0, r0, 0, 22, 20
    mr r3, r31
    stw r0, lbl_8047B618(r13)
    oris r0, r0, 0x20
    mr r4, r22
    mr r5, r30
    stw r0, lbl_8047B618(r13)
    li r7, 0x1
    li r8, 0x1
    li r9, -0x1
    bl fn_8022B2CC
    mr r4, r30
    mr r23, r3
    bl fn_801F0134
    mr r0, r3
    mr r3, r24
    mr r6, r0
    mr r5, r22
    extsb r4, r21
    li r7, 0x0
    bl fn_802099AC
    mr r7, r23
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r4, r22
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x28
    bl fn_80132A38
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x1
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x1
    bl fn_80208404
    li r3, 0x770c
    bl fn_802624CC
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x2
    bl fn_80208404
    bl fn_8026246C
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x3
    bl fn_80208404
    lwz r4, lbl_8047B618(r13)
    lis r3, lbl_803799ED@ha
    addi r0, r3, lbl_803799ED@l
    li r3, 0x2
    ori r4, r4, 0x400
    stw r0, lbl_8047B610(r13)
    stw r4, lbl_8047B618(r13)
    b @L_8022B288
@L_8022AAF0:
    mr r3, r27
    bl fn_8011CE18
    bl fn_8011CCCC
    rlwinm r0, r3, 0, 24, 31
    add r21, r21, r0
    cmplw r26, r21
    bge @L_8022ABFC
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80208404
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    bl fn_80208750
    mr r3, r28
    li r4, 0x0
    bl fn_801FBA24
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x1
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    bl fn_80208404
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    li r7, -0x1
    bl fn_802085C4
    li r3, 0x770d
    bl fn_802624CC
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    li r6, 0x6
    bl fn_80208554
    bl fn_8026246C
    mr r3, r28
    li r4, 0x1
    bl fn_801FBA24
    mr r3, r28
    li r4, 0x2
    bl fn_801FBA24
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    bl fn_802086E8
    mr r3, r28
    li r4, 0x3
    bl fn_801FBA24
    lis r4, lbl_803799F4@ha
    li r3, 0x1
    addi r0, r4, lbl_803799F4@l
    stw r0, lbl_8047B610(r13)
    b @L_8022B288
@L_8022ABFC:
    mr r3, r27
    bl fn_8011CE18
    bl fn_8011CCB4
    rlwinm r0, r3, 0, 24, 31
    add r21, r21, r0
    cmplw r26, r21
    bge @L_8022AD08
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80208404
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    bl fn_80208750
    mr r3, r29
    li r4, 0x0
    bl fn_801FBA24
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x1
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    bl fn_80208404
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    li r6, 0x0
    li r7, -0x1
    bl fn_802085C4
    li r3, 0x770e
    bl fn_802624CC
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    li r6, 0x6
    bl fn_80208554
    bl fn_8026246C
    mr r3, r29
    li r4, 0x1
    bl fn_801FBA24
    mr r3, r29
    li r4, 0x2
    bl fn_801FBA24
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x1
    li r5, 0x1
    bl fn_802086E8
    mr r3, r29
    li r4, 0x3
    bl fn_801FBA24
    lis r4, lbl_803799F4@ha
    li r3, 0x1
    addi r0, r4, lbl_803799F4@l
    stw r0, lbl_8047B610(r13)
    b @L_8022B288
@L_8022AD08:
    mr r3, r27
    bl fn_8011CE18
    bl fn_8011CC9C
    rlwinm r0, r3, 0, 24, 31
    add r21, r21, r0
    cmplw r26, r21
    bge @L_8022AE58
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_80208404
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    mr r4, r31
    mr r5, r25
    li r6, 0x1
    li r7, 0x28
    li r8, -0x1
    bl fn_80232110
    mr r0, r3
    mr r3, r24
    mr r7, r0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x0
    li r5, 0x107
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    oris r0, r0, 0x8
    li r6, 0x1
    stw r0, lbl_8047B618(r13)
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x1
    bl fn_80208404
    li r3, 0x770f
    bl fn_802624CC
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x2
    bl fn_80208404
    bl fn_8026246C
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x2
    li r6, 0x3
    bl fn_80208404
    lis r4, lbl_803799EF@ha
    li r3, 0x2
    addi r0, r4, lbl_803799EF@l
    stw r0, lbl_8047B610(r13)
    b @L_8022B288
@L_8022AE58:
    mr r3, r27
    bl fn_8011CE18
    bl fn_8011CC84
    rlwinm r0, r3, 0, 24, 31
    add r21, r21, r0
    cmplw r26, r21
    bge @L_8022AF34
@L_8022AE74:
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80208404
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    li r6, 0x0
    bl fn_80208750
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x1
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    bl fn_80208404
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    li r6, 0x0
    li r7, -0x1
    bl fn_802085C4
    li r3, 0x7710
    bl fn_802624CC
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    li r6, 0x6
    bl fn_80208554
    bl fn_8026246C
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    bl fn_802086E8
    lis r4, lbl_803799F4@ha
    li r3, 0x1
    addi r0, r4, lbl_803799F4@l
    stw r0, lbl_8047B610(r13)
    b @L_8022B288
@L_8022AF34:
    mr r3, r27
    bl fn_8011CE18
    bl fn_8011CC6C
    rlwinm r0, r3, 0, 24, 31
    add r21, r21, r0
    cmplw r26, r21
    bge @L_8022B0FC
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80208404
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    li r6, 0x0
    bl fn_80208750
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x1
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    bl fn_80208404
    mr r3, r23
    bl fn_80142984
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022AFEC
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    li r6, 0x0
    li r7, -0x1
    bl fn_802085C4
    li r3, 0x7712
    bl fn_802624CC
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    li r6, 0x6
    bl fn_80208554
    bl fn_8026246C
    b @L_8022B0C4
@L_8022AFEC:
    rlwinm r3, r23, 0, 16, 31
    cmplwi r3, 0x2c
    beq @L_8022B028
    subi r0, r23, 0x85
    rlwinm r0, r0, 0, 16, 31
    cmplwi r0, 0xe
    ble @L_8022B028
    subi r0, r23, 0xa8
    rlwinm r0, r0, 0, 16, 31
    cmplwi r0, 0x6
    ble @L_8022B028
    cmplwi r3, 0xb4
    beq @L_8022B028
    cmplwi r3, 0xb9
    bne @L_8022B030
@L_8022B028:
    li r0, 0x1
    b @L_8022B034
@L_8022B030:
    li r0, 0x0
@L_8022B034:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022B08C
    mr r3, r31
    li r4, 0x2
    bl fn_8022BE2C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022B08C
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    bl fn_802086E8
    lis r4, lbl_803799F4@ha
    li r3, 0x1
    addi r0, r4, lbl_803799F4@l
    stw r0, lbl_8047B610(r13)
    b @L_8022B288
@L_8022B08C:
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    li r6, 0x0
    li r7, -0x1
    bl fn_802085C4
    li r3, 0x7711
    bl fn_802624CC
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    li r6, 0x6
    bl fn_80208554
    bl fn_8026246C
@L_8022B0C4:
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x85
    li r5, 0x3
    bl fn_802086E8
    lis r4, lbl_803799F4@ha
    li r3, 0x1
    addi r0, r4, lbl_803799F4@l
    stw r0, lbl_8047B610(r13)
    b @L_8022B288
@L_8022B0FC:
    mr r3, r27
    bl fn_8011CE18
    bl fn_8011CC54
    rlwinm r0, r3, 0, 24, 31
    add r0, r21, r0
    cmplw r26, r0
    bge @L_8022B284
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    bl fn_80208ED0
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x1
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x2
    bl fn_80208404
    li r0, 0x0
    mr r3, r29
    sth r0, 0x8(r1)
    addi r4, r1, 0x8
    bl fn_801F8A18
    cmplwi r3, 0x0
    beq @L_8022B208
    mr r3, r31
    li r4, 0x1
    bl fn_80208ED0
    li r3, 0x7713
    bl fn_802624CC
    mr r3, r31
    li r4, 0x2
    bl fn_80208ED0
    bl fn_8026246C
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x3
    bl fn_80208ED0
    mr r3, r31
    li r4, 0x4
    bl fn_80208ED0
    lis r4, lbl_803799FE@ha
    li r3, 0x1
    addi r0, r4, lbl_803799FE@l
    stw r0, lbl_8047B610(r13)
    b @L_8022B288
@L_8022B208:
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x1
    bl fn_80208404
    li r3, 0x7714
    bl fn_802624CC
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x2
    bl fn_80208404
    bl fn_8026246C
    mr r3, r31
    li r4, 0x0
    li r5, 0x1
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x0
    li r5, 0x0
    li r6, 0x3
    bl fn_80208404
    mr r3, r31
    li r4, 0x3
    bl fn_80208ED0
    lis r4, lbl_803799F4@ha
    li r3, 0x1
    addi r0, r4, lbl_803799F4@l
    stw r0, lbl_8047B610(r13)
    b @L_8022B288
@L_8022B284:
    li r3, 0x0
@L_8022B288:
    lmw r21, 0x24(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
