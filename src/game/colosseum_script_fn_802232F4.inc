    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x64(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r14, 0x18(r1)
    bl fn_801F54A4
    lwz r5, lbl_8047B610(r13)
    rlwinm r31, r3, 0, 16, 31
    li r30, 0x0
    li r4, 0x0
    lbz r3, 0x1(r5)
    bl fn_801F025C
    mr r19, r3
    bl fn_80203D3C
    mr r0, r3
    li r3, 0x0
    mr r18, r0
    li r5, 0x10
    mr r4, r18
    li r6, 0x0
    bl fn_8012640C
    rlwinm r15, r3, 0, 16, 31
    mr r3, r19
    bl fn_80203E0C
    mr r14, r3
    mr r3, r19
    bl fn_80204A10
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80223A04
    li r3, 0x0
    li r4, 0x0
    li r5, 0x24
    li r6, 0x0
    bl fn_801F54A4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80223A04
    mr r4, r19
    li r3, 0x3
    bl fn_801F025C
    mr r20, r3
    bl fn_801F7404
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80223A04
    li r16, 0x0
    li r21, 0x0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r17, r3, 0, 16, 31
    li r22, 0x0
    b @L_80223488
@L_802233D8:
    mr r3, r20
    rlwinm r4, r22, 0, 16, 31
    bl fn_801F7258
    mr. r24, r3
    beq @L_80223484
    bl fn_801F9034
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80223484
    mr r3, r24
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    li r23, 0x0
    mr r25, r3
@L_80223414:
    mr r3, r25
    rlwinm r5, r23, 0, 16, 31
    li r4, 0x3
    bl fn_8012A5B0
    mr r4, r3
    mr r3, r24
    bl fn_801F9930
    mr. r26, r3
    beq @L_80223478
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80223478
    mr r3, r19
    mr r4, r26
    bl fn_80205904
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223460
    addi r16, r16, 0x1
@L_80223460:
    mr r3, r26
    bl fn_802041EC
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x19
    bne @L_80223478
    addi r21, r21, 0x1
@L_80223478:
    addi r23, r23, 0x1
    cmpwi r23, 0x6
    blt @L_80223414
@L_80223484:
    addi r22, r22, 0x1
@L_80223488:
    cmpw r22, r17
    blt @L_802233D8
    cmpwi r16, 0x0
    bne @L_802234A0
    cmpwi r21, 0x0
    beq @L_80223A04
@L_802234A0:
    rlwinm r3, r14, 0, 24, 31
    li r0, 0x7
    mullw r3, r15, r3
    cmpwi r21, 0x0
    divw r0, r3, r0
    rlwinm r0, r0, 0, 16, 31
    beq @L_802234E4
    rlwinm r3, r0, 31, 17, 31
    divw r0, r3, r16
    rlwinm. r4, r0, 0, 16, 31
    bne @L_802234D0
    li r4, 0x1
@L_802234D0:
    divw r0, r3, r21
    rlwinm. r3, r0, 0, 16, 31
    bne @L_802234F8
    li r3, 0x1
    b @L_802234F8
@L_802234E4:
    divw r0, r0, r16
    rlwinm. r4, r0, 0, 16, 31
    bne @L_802234F4
    li r4, 0x1
@L_802234F4:
    li r3, 0x0
@L_802234F8:
    rlwinm r0, r4, 0, 16, 31
    li r26, 0x0
    stw r0, 0xc(r1)
    rlwinm r0, r3, 0, 16, 31
    stw r0, 0x8(r1)
    b @L_802239FC
@L_80223510:
    mr r3, r20
    rlwinm r6, r26, 0, 16, 31
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr r25, r3
    bl fn_801F9034
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802239F8
    mr r3, r25
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    li r24, 0x0
    mr r14, r3
@L_80223550:
    mr r3, r14
    rlwinm r5, r24, 0, 16, 31
    li r16, 0x0
    li r4, 0x3
    bl fn_8012A5B0
    mr r4, r3
    mr r3, r25
    bl fn_801F9930
    mr. r23, r3
    beq @L_802239EC
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802239EC
    mr r3, r23
    bl fn_80203E7C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x64
    bge @L_802239EC
    mr r3, r19
    mr r4, r23
    bl fn_80205904
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802235B4
    lwz r16, 0xc(r1)
@L_802235B4:
    mr r3, r23
    bl fn_802041EC
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x19
    bne @L_802235D0
    lwz r0, 0x8(r1)
    add r16, r16, r0
@L_802235D0:
    cmplwi r16, 0x0
    beq @L_802239EC
    mr r3, r23
    bl fn_802041EC
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x28
    bne @L_802235F8
    mulli r3, r16, 0x96
    li r0, 0x64
    divwu r16, r3, r0
@L_802235F8:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223624
    mulli r3, r16, 0x96
    li r0, 0x64
    divwu r16, r3, r0
@L_80223624:
    mr r3, r25
    mr r4, r23
    bl fn_801F90C4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022364C
    mulli r3, r16, 0x96
    li r0, 0x64
    li r15, 0x7534
    divwu r16, r3, r0
    b @L_80223650
@L_8022364C:
    li r15, 0x7533
@L_80223650:
    mr r3, r23
    bl fn_80205BE8
    mr r21, r3
    bl fn_8011FC74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802236E8
    mr r3, r21
    bl fn_8011F77C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x3
    blt @L_802239EC
    mr r3, r21
    li r4, 0x0
    li r5, 0xc6
    li r6, 0x0
    bl fn_8012640C
    add r7, r3, r16
    mr r3, r21
    li r4, 0x0
    li r5, 0xc6
    li r6, 0x0
    bl fn_801254B4
    mr r3, r25
    mr r4, r23
    bl fn_801F8E34
    mr. r15, r3
    beq @L_802239EC
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802239EC
    mr r3, r15
    li r4, 0xc6
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
    b @L_802239EC
@L_802236E8:
    rlwinm. r0, r30, 0, 24, 31
    bne @L_80223700
    bl fn_801C3430
    li r3, 0x1
    bl fn_801EF8F4
    li r30, 0x1
@L_80223700:
    mr r3, r25
    mr r4, r23
    bl fn_801F8E34
    cmplwi r3, 0x0
    beq @L_80223720
    mr r4, r31
    li r5, 0x1
    bl fn_80265598
@L_80223720:
    mr r3, r23
    bl fn_80203848
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r15
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    mr r4, r16
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x7532
    bl fn_802624CC
    mr r15, r3
    bl fn_8026246C
    mr r3, r23
    mr r5, r18
    li r4, 0x0
    bl fn_8020341C
    lis r3, lbl_80478278@ha
    addi r29, r3, lbl_80478278@l
@L_8022377C:
    cmplwi r16, 0x0
    beq @L_802239BC
    mr r3, r23
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802239BC
    mr r3, r23
    bl fn_80203E7C
    mr r28, r3
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x64
    bge @L_802239BC
    mr r3, r23
    mr r4, r29
    bl fn_801FE468
    stw r23, lbl_8047B64C(r13)
    mr r3, r23
    bl fn_80203620
    addi r0, r28, 0x1
    mr r21, r3
    mr r3, r23
    rlwinm r4, r0, 0, 24, 31
    bl fn_8020355C
    add r27, r21, r16
    mr r21, r3
    cmplw r27, r21
    blt @L_80223940
    mr r3, r23
    mr r4, r21
    subf r16, r21, r27
    bl fn_8020367C
    mr r3, r23
    bl fn_80203848
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r3, r23
    bl fn_80203E7C
    rlwinm r4, r3, 0, 24, 31
    li r3, 0x2f
    bl fn_80132A38
    li r3, 0x0
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223860
    mr r3, r25
    bl fn_801FB8F8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80223860
    mr r3, r23
    li r4, 0x0
    bl fn_802032E4
@L_80223860:
    mr r3, r23
    li r4, 0x0
    li r5, 0xd0
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    mr r3, r25
    mr r4, r23
    bl fn_801F8E34
    mr. r15, r3
    beq @L_802238C8
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802238A4
    mr r3, r15
    bl fn_801FE5D4
@L_802238A4:
    mr r3, r15
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802238C8
    mr r3, r15
    li r4, 0xd0
    li r5, 0x0
    bl fn_801FE710
@L_802238C8:
    mr r3, r25
    mr r4, r23
    bl fn_801F8E34
    mr. r15, r3
    beq @L_80223924
    mr r4, r31
    li r5, 0x0
    bl fn_802656AC
    mr r0, r3
    mr r3, r23
    mr r22, r0
    rlwinm r4, r28, 0, 24, 31
    bl fn_8020355C
    subf r4, r3, r21
    mr r3, r22
    bl fn_80011BC4
    mr r3, r22
    li r4, 0x1
    bl fn_80011B4C
    mr r3, r15
    mr r4, r31
    li r5, 0x1
    bl fn_802653FC
@L_80223924:
    lis r4, lbl_80378724@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378724@l
    li r5, 0x0
    bl fn_80211B94
    li r15, 0x0
    b @L_802239B0
@L_80223940:
    mr r3, r23
    mr r4, r27
    li r16, 0x0
    bl fn_802035BC
    mr r3, r25
    mr r4, r23
    bl fn_801F8E34
    mr. r22, r3
    beq @L_802239B0
    mr r4, r31
    li r5, 0x0
    bl fn_802656AC
    mr r21, r3
    mr r3, r23
    rlwinm r4, r28, 0, 24, 31
    bl fn_8020355C
    subf r4, r3, r27
    mr r3, r21
    bl fn_80011BC4
    li r3, 0x40
    bl fn_801F000C
    mr r3, r21
    li r4, 0x1
    bl fn_80011B4C
    mr r3, r22
    mr r4, r31
    li r5, 0x1
    bl fn_802653FC
@L_802239B0:
    li r0, 0x0
    stw r0, lbl_8047B64C(r13)
    b @L_8022377C
@L_802239BC:
    rlwinm r0, r15, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802239CC
    bl fn_8026246C
@L_802239CC:
    mr r3, r25
    mr r4, r23
    bl fn_801F8E34
    cmplwi r3, 0x0
    beq @L_802239EC
    mr r4, r31
    li r5, 0x0
    bl fn_8026532C
@L_802239EC:
    addi r24, r24, 0x1
    cmpwi r24, 0x6
    blt @L_80223550
@L_802239F8:
    addi r26, r26, 0x1
@L_802239FC:
    cmpw r26, r17
    blt @L_80223510
@L_80223A04:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x2
    stw r0, lbl_8047B610(r13)
    lmw r14, 0x18(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
