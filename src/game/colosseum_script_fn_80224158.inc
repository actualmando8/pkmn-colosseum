    nofralloc
    stwu r1, -0x100(r1)
    mflr r0
    li r3, 0x0
    stw r0, 0x104(r1)
    stmw r22, 0xd8(r1)
    bl fn_80261B68
    li r3, 0x0
    bl fn_80261E7C
    bl fn_8026246C
    lwz r3, lbl_8047B610(r13)
    lbz r0, 0x2(r3)
    cmplwi r0, 0x0
    beq @L_802241F0
    lbz r3, 0x1(r3)
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r22, r0
    mr r4, r22
    bl fn_801F025C
    mr r0, r3
    mr r3, r22
    mr r22, r0
    bl fn_802038A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224720
    mr r3, r22
    li r4, 0x0
    li r5, 0x8
    li r6, 0x0
    li r7, 0x0
    bl fn_801F75F8
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x3(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8022472C
@L_802241F0:
    lbz r0, 0x1(r3)
    cmplwi r0, 0x11
    bne @L_8022423C
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    lis r4, lbl_803786F4@ha
    mr r29, r3
    addi r0, r4, lbl_803786F4@l
    mr r30, r0
    b @L_802242E4
@L_8022423C:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x12
    bne @L_80224288
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    lis r4, lbl_80378703@ha
    mr r29, r3
    addi r0, r4, lbl_80378703@l
    mr r30, r0
    b @L_802242E4
@L_80224288:
    cmplwi r0, 0x15
    bne @L_802242D0
    li r3, 0x15
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    lis r4, lbl_80378712@ha
    mr r29, r3
    addi r0, r4, lbl_80378712@l
    mr r30, r0
    b @L_802242E4
@L_802242D0:
    lis r3, lbl_80378703@ha
    li r31, 0x0
    addi r0, r3, lbl_80378703@l
    li r29, 0x0
    mr r30, r0
@L_802242E4:
    mr r3, r31
    bl fn_802038A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224720
    mr r3, r31
    li r4, 0x0
    li r5, 0x120
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    mr r3, r31
    bl fn_80204A10
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224420
    lwz r0, lbl_8047B618(r13)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x27
    oris r0, r0, 0x40
    li r6, 0x0
    stw r0, lbl_8047B618(r13)
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224420
    cmplwi r31, 0x0
    beq @L_80224420
    mr r3, r31
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr r25, r3
    mr r3, r31
    bl fn_80203E0C
    mr r28, r3
    mr r6, r31
    addi r7, r1, 0x8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2
    bl fn_801F1D5C
    li r24, 0x0
    addi r22, r1, 0x8
    mr r23, r24
    rlwinm r26, r3, 0, 16, 31
    b @L_802243DC
@L_802243A8:
    clrlslwi r27, r23, 16, 2
    lwzx r3, r22, r27
    cmplwi r3, 0x0
    beq @L_802243D8
    bl fn_80203E0C
    rlwinm r3, r3, 0, 24, 31
    rlwinm r0, r24, 0, 24, 31
    cmplw r0, r3
    bge @L_802243D8
    lwzx r3, r22, r27
    bl fn_80203E0C
    mr r24, r3
@L_802243D8:
    addi r23, r23, 0x1
@L_802243DC:
    rlwinm r0, r23, 0, 16, 31
    cmplw r0, r26
    blt @L_802243A8
    rlwinm r3, r24, 0, 24, 31
    rlwinm r0, r28, 0, 24, 31
    cmplw r3, r0
    ble @L_80224420
    subf r0, r0, r3
    cmpwi r0, 0x1e
    blt @L_80224414
    mr r3, r25
    li r4, 0x8
    bl fn_802032E4
    b @L_80224420
@L_80224414:
    mr r3, r25
    li r4, 0x6
    bl fn_802032E4
@L_80224420:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 7, 7
    bne @L_80224614
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x28
    mr r22, r3
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224614
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r28, r3
    bl fn_80205B8C
    mr r0, r3
    mr r3, r28
    mr r27, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r28
    mr r23, r0
    bl fn_80205224
    mr r0, r3
    mr r3, r28
    mr r24, r0
    bl fn_80205184
    mr r26, r3
    mr r3, r28
    mr r4, r22
    bl fn_80202B88
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80224614
    mr r3, r28
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224614
    rlwinm r0, r26, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_80224614
    cmplwi r0, 0x164
    beq @L_80224614
    mr r3, r27
    mr r4, r24
    bl fn_80123B5C
    mr r22, r3
    extsb. r0, r22
    bge @L_80224510
    mr r3, r23
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_8011BEB4
    extsb r22, r3
@L_80224510:
    extsb r23, r22
    mr r3, r27
    mr r6, r23
    li r4, 0x0
    li r5, 0x80
    li r7, 0x0
    bl fn_801254B4
    mr r3, r28
    li r4, 0x10
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802245A0
    mr r3, r28
    li r4, 0x31
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802245A0
    mr r3, r28
    li r4, 0x31
    bl fn_80201890
    rlwinm r22, r22, 0, 24, 31
    li r0, 0x1
    slw r0, r0, r22
    and. r0, r3, r0
    bne @L_802245A0
    mr r3, r28
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802245A0
    mr r3, r28
    mr r5, r22
    li r4, 0x80
    li r6, 0x0
    bl fn_801FE7EC
@L_802245A0:
    mr r3, r28
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802245C8
    mr r3, r28
    mr r5, r23
    li r4, 0x80
    li r6, 0x0
    bl fn_801FE7EC
@L_802245C8:
    mr r3, r27
    mr r6, r23
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lis r4, lbl_80379167@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379167@l
    li r5, 0x0
    bl fn_80211B94
@L_80224614:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_802246C8
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r22, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802246BC
    mr r3, r22
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r22, r3
    addi r3, r1, 0x28
    mr r4, r22
    bl fn_8020A2B8
    mr r3, r29
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r22
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r22
    bl fn_80209FAC
    lis r4, lbl_80378D54@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378D54@l
    li r5, 0x0
    bl fn_80211B94
    mr r3, r22
    addi r4, r1, 0x28
    bl fn_8020A2B8
@L_802246BC:
    lwz r0, lbl_8047B618(r13)
    rlwinm r0, r0, 0, 26, 24
    stw r0, lbl_8047B618(r13)
@L_802246C8:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F54A4
    mr r22, r3
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, lbl_8047B62C(r13)
    mr r4, r30
    li r5, 0x0
    bl fn_80211B94
    mr r7, r22
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
@L_80224720:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_8022472C:
    lmw r22, 0xd8(r1)
    lwz r0, 0x104(r1)
    mtlr r0
    addi r1, r1, 0x100
    blr
