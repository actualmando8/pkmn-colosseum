    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x54(r1)
    stmw r15, 0xc(r1)
    mr r22, r3
    mr r23, r4
    li r27, 0x0
    li r26, 0x0
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    rlwinm r24, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    mr r19, r3
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r30, r3
    bl fn_80205184
    mr r0, r3
    li r3, 0x12
    mr r28, r0
    li r4, 0x0
    bl fn_801F025C
    la r31, lbl_80478D78(r13)
    mr r18, r3
    lbz r0, 0x3(r31)
    rlwinm. r0, r0, 0, 25, 25
    beq @L_80224A88
    mr r29, r30
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x47
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r18
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lbz r0, 0x3(r31)
    mr r3, r18
    li r26, 0x40
    andi. r0, r0, 0xbf
    stb r0, 0x3(r31)
    b @L_80224AC0
@L_80224A88:
    mr r29, r18
    mr r7, r18
    li r3, 0x0
    li r4, 0x0
    li r5, 0x47
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
@L_80224AC0:
    mr r4, r24
    bl fn_801F0134
    mr r0, r3
    mr r3, r29
    mr r17, r0
    bl fn_80207BF4
    mr r20, r3
    mr r3, r29
    bl fn_802040E8
    mr r21, r3
    mr r4, r29
    li r3, 0x2
    bl fn_801F025C
    mr r0, r3
    mr r3, r29
    mr r16, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r29
    mr r25, r0
    li r4, 0x0
    li r5, 0xf8
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r20, 0, 16, 31
    mr r15, r3
    cmplwi r0, 0x13
    bne @L_80224B6C
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 18, 18
    bne @L_80224B6C
    rlwinm. r0, r22, 0, 24, 31
    bne @L_80224B6C
    lbz r0, 0x3(r31)
    cmplwi r0, 0xa
    bge @L_80224B6C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80224B6C:
    mr r3, r16
    li r4, 0x4b
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224BB4
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 18, 18
    bne @L_80224BB4
    rlwinm. r0, r22, 0, 24, 31
    bne @L_80224BB4
    lbz r0, 0x3(r31)
    cmplwi r0, 0x8
    bge @L_80224BB4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80224BB4:
    mr r3, r29
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80224BE8
    lbz r0, 0x3(r31)
    cmplwi r0, 0xb
    beq @L_80224BE8
    cmplwi r0, 0x1f
    beq @L_80224BE8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80224BE8:
    mr r3, r29
    li r4, 0x14
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224C1C
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x40
    beq @L_80224C1C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80224C1C:
    lbz r0, 0x3(r31)
    cmplwi r0, 0x7
    bge @L_8022551C
    lis r3, lbl_80279EF4@ha
    clrlslwi r0, r0, 24, 1
    addi r3, r3, lbl_80279EF4@l
    lhzx r0, r3, r0
    cmpwi r0, 0x6
    beq @L_80224F68
    bge @L_80224C5C
    cmpwi r0, 0x4
    beq @L_80225290
    bge @L_802251F0
    cmpwi r0, 0x3
    bge @L_80224E1C
    b @L_80225404
@L_80224C5C:
    cmpwi r0, 0x8
    beq @L_80224C6C
    bge @L_80225404
    b @L_80225088
@L_80224C6C:
    rlwinm r0, r20, 0, 16, 31
    li r15, 0x1
    cmplwi r0, 0x2b
    beq @L_80224C94
    li r3, 0x0
    li r4, 0xb
    bl fn_801F33E8
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80224C94
    li r15, 0x0
@L_80224C94:
    mr r3, r29
    bl fn_80203CCC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225404
    lbz r0, 0x3(r31)
    lis r3, lbl_80279EF4@ha
    addi r4, r3, lbl_80279EF4@l
    rlwinm r0, r0, 1, 0, 30
    mr r3, r29
    lhzx r4, r4, r0
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80225404
    rlwinm r0, r15, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225404
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x48
    beq @L_80225404
    cmplwi r0, 0xf
    beq @L_80225404
    li r3, 0x0
    li r4, 0x0
    li r5, 0x34
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r16, r3, 0, 24, 31
    bl fn_80077B60
    mr r15, r3
    bl fn_80077B3C
    cmplwi r16, 0x1
    bne @L_80224DFC
    rlwinm r0, r15, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80224DFC
    mr r4, r29
    li r18, 0x0
    li r3, 0x2
    bl fn_801F025C
    mr r20, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r19, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    li r16, 0x0
    b @L_80224DDC
@L_80224D70:
    mr r3, r20
    rlwinm r4, r16, 0, 16, 31
    bl fn_801F7258
    mr. r17, r3
    beq @L_80224DD8
    li r15, 0x0
    b @L_80224DCC
@L_80224D8C:
    mr r3, r17
    mr r4, r15
    bl fn_801F986C
    mr. r21, r3
    beq @L_80224DC8
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80224DC8
    mr r3, r21
    li r4, 0x8
    bl fn_80202ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224DC8
    addi r18, r18, 0x1
@L_80224DC8:
    addi r15, r15, 0x1
@L_80224DCC:
    rlwinm r0, r15, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_80224D8C
@L_80224DD8:
    addi r16, r16, 0x1
@L_80224DDC:
    rlwinm r0, r16, 0, 16, 31
    cmplw r0, r19
    blt @L_80224D70
    rlwinm r0, r18, 0, 16, 31
    cmplwi r0, 0x1
    blt @L_80224DFC
    li r0, 0x1
    b @L_80224E00
@L_80224DFC:
    li r0, 0x0
@L_80224E00:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80225404
    mr r3, r29
    bl fn_80200B10
    li r27, 0x1
    b @L_80225404
@L_80224E1C:
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x11
    bne @L_80224E94
    rlwinm r0, r22, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80224E40
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x80
    bne @L_80224E94
@L_80224E40:
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_80224E64
    rlwinm r0, r3, 0, 19, 17
    li r4, 0x1
    la r3, lbl_80478D78(r13)
    stw r0, lbl_8047B618(r13)
    stb r4, 0x5(r3)
    b @L_80224E70
@L_80224E64:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_80224E70:
    lis r4, lbl_80379836@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379836@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80224E94:
    mr r3, r29
    li r4, 0x3
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80224EC4
    mr r3, r29
    li r4, 0x8
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80224F18
@L_80224EC4:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 18, 18
    beq @L_80224F18
    rlwinm r0, r22, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80224EE8
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x80
    bne @L_80224F18
@L_80224EE8:
    li r0, 0x2
    la r5, lbl_80478D78(r13)
    lis r4, lbl_80379836@ha
    stb r0, 0x5(r5)
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379836@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80224F18:
    mr r3, r29
    li r4, 0x3
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80225404
    mr r3, r29
    li r4, 0x8
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80225404
    mr r3, r29
    bl fn_80203CCC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225404
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x11
    beq @L_80225404
    li r27, 0x1
    b @L_80225404
@L_80224F68:
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x29
    bne @L_80224FE0
    rlwinm r0, r22, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80224F8C
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x80
    bne @L_80224FE0
@L_80224F8C:
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_80224FB0
    rlwinm r0, r3, 0, 19, 17
    li r4, 0x1
    la r3, lbl_80478D78(r13)
    stw r0, lbl_8047B618(r13)
    stb r4, 0x5(r3)
    b @L_80224FBC
@L_80224FB0:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_80224FBC:
    lis r4, lbl_80379808@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379808@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80224FE0:
    mr r3, r29
    li r4, 0xa
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022504C
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 18, 18
    beq @L_8022504C
    rlwinm r0, r22, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8022501C
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x80
    bne @L_8022504C
@L_8022501C:
    li r0, 0x2
    la r5, lbl_80478D78(r13)
    lis r4, lbl_80379808@ha
    stb r0, 0x5(r5)
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379808@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_8022504C:
    mr r3, r29
    li r4, 0xa
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80225404
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x29
    beq @L_80225404
    mr r3, r29
    bl fn_80203CCC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225404
    li r27, 0x1
    b @L_80225404
@L_80225088:
    mr r3, r29
    li r4, 0xf
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80225404
    mr r3, r29
    bl fn_80203CCC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225404
    rlwinm r0, r19, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80225404
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x28
    beq @L_80225404
    li r3, 0x0
    li r4, 0x0
    li r5, 0x34
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r15, r3, 0, 24, 31
    bl fn_80077B60
    bl fn_80077B3C
    cmplwi r15, 0x1
    bne @L_802251D0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802251D0
    mr r4, r29
    li r18, 0x0
    li r3, 0x2
    bl fn_801F025C
    mr r20, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r19, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_801F54A4
    li r16, 0x0
    b @L_802251B0
@L_80225144:
    mr r3, r20
    rlwinm r4, r16, 0, 16, 31
    bl fn_801F7258
    mr. r17, r3
    beq @L_802251AC
    li r15, 0x0
    b @L_802251A0
@L_80225160:
    mr r3, r17
    mr r4, r15
    bl fn_801F986C
    mr. r21, r3
    beq @L_8022519C
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022519C
    mr r3, r21
    li r4, 0x7
    bl fn_80202ADC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022519C
    addi r18, r18, 0x1
@L_8022519C:
    addi r15, r15, 0x1
@L_802251A0:
    rlwinm r0, r15, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_80225160
@L_802251AC:
    addi r16, r16, 0x1
@L_802251B0:
    rlwinm r0, r16, 0, 16, 31
    cmplw r0, r19
    blt @L_80225144
    rlwinm r0, r18, 0, 16, 31
    cmplwi r0, 0x1
    blt @L_802251D0
    li r0, 0x1
    b @L_802251D4
@L_802251D0:
    li r0, 0x0
@L_802251D4:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80225404
    mr r3, r29
    bl fn_80200B10
    li r27, 0x1
    b @L_80225404
@L_802251F0:
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x7
    bne @L_80225268
    rlwinm r0, r22, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80225214
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x80
    bne @L_80225268
@L_80225214:
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_80225238
    rlwinm r0, r3, 0, 19, 17
    li r4, 0x1
    la r3, lbl_80478D78(r13)
    stw r0, lbl_8047B618(r13)
    stb r4, 0x5(r3)
    b @L_80225244
@L_80225238:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_80225244:
    lis r4, lbl_8037981F@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037981F@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225268:
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x7
    beq @L_80225404
    mr r3, r29
    bl fn_80203CCC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225404
    li r27, 0x1
    b @L_80225404
@L_80225290:
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x11
    bne @L_80225308
    rlwinm r0, r22, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802252B4
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x80
    bne @L_80225308
@L_802252B4:
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_802252D8
    rlwinm r0, r3, 0, 19, 17
    li r4, 0x1
    la r3, lbl_80478D78(r13)
    stw r0, lbl_8047B618(r13)
    stb r4, 0x5(r3)
    b @L_802252E4
@L_802252D8:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_802252E4:
    lis r4, lbl_80379836@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379836@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225308:
    mr r3, r29
    li r4, 0x3
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80225338
    mr r3, r29
    li r4, 0x8
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022538C
@L_80225338:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 18, 18
    beq @L_8022538C
    rlwinm r0, r22, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8022535C
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x80
    bne @L_8022538C
@L_8022535C:
    li r0, 0x2
    la r5, lbl_80478D78(r13)
    lis r4, lbl_80379836@ha
    stb r0, 0x5(r5)
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379836@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_8022538C:
    mr r3, r29
    bl fn_80203CCC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225404
    mr r3, r29
    li r4, 0x3
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802253DC
    mr r3, r29
    li r4, 0x8
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802253DC
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x11
    beq @L_80225404
    li r27, 0x1
    b @L_80225404
@L_802253DC:
    mr r3, r25
    li r4, 0x43
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80225404
    mr r3, r25
    li r4, 0x43
    li r5, 0x0
    bl fn_802097C8
@L_80225404:
    rlwinm. r0, r27, 0, 24, 31
    beq @L_8022550C
    lbz r0, 0x3(r31)
    lis r3, lbl_80279EF4@ha
    addi r4, r3, lbl_80279EF4@l
    rlwinm r0, r0, 1, 0, 30
    mr r3, r29
    lhzx r4, r4, r0
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80225454
    lbz r0, 0x3(r31)
    lis r3, lbl_80279EF4@ha
    addi r4, r3, lbl_80279EF4@l
    li r5, 0x0
    rlwinm r0, r0, 1, 0, 30
    mr r3, r29
    lhzx r4, r4, r0
    bl fn_8020248C
@L_80225454:
    mr r3, r29
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022547C
    mr r3, r29
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8022547C:
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 18, 18
    beq @L_802254A0
    rlwinm r4, r3, 0, 19, 17
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stw r4, lbl_8047B618(r13)
    stb r0, 0x5(r3)
    b @L_802254AC
@L_802254A0:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_802254AC:
    lbz r4, 0x3(r31)
    cmplwi r4, 0x2
    beq @L_802254D0
    subi r0, r4, 0x5
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    ble @L_802254D0
    cmplwi r4, 0x3
    bne @L_802254E0
@L_802254D0:
    lwz r0, lbl_8047B618(r13)
    stb r4, lbl_8047B628(r13)
    ori r0, r0, 0x4000
    stw r0, lbl_8047B618(r13)
@L_802254E0:
    lis r3, lbl_80399F58@ha
    clrlslwi r0, r4, 24, 2
    addi r4, r3, lbl_80399F58@l
    lwz r3, lbl_8047B62C(r13)
    lwzx r4, r4, r0
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_8022550C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_8022551C:
    lis r3, lbl_80279EF4@ha
    clrlslwi r0, r0, 24, 1
    addi r3, r3, lbl_80279EF4@l
    lhzx r4, r3, r0
    cmplwi r4, 0x0
    beq @L_80225558
    mr r3, r29
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225558
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225558:
    lbz r4, 0x3(r31)
    subi r0, r4, 0x7
    cmplwi r0, 0x34
    bgt @L_80226114
    lis r3, jumptable_8039A220@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A220@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x14
    beq @L_802255F8
    mr r3, r29
    li r4, 0x9
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802255F8
    mr r3, r29
    li r4, 0x9
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802255C8
    mr r3, r29
    li r4, 0x9
    li r5, 0x0
    bl fn_8020248C
@L_802255C8:
    lbz r0, 0x3(r31)
    lis r3, lbl_80399F58@ha
    addi r4, r3, lbl_80399F58@l
    lwz r3, lbl_8047B62C(r13)
    rlwinm r0, r0, 2, 0, 29
    li r5, 0x0
    lwzx r4, r4, r0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_802255F8:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x27
    bne @L_8022563C
    rlwinm r0, r22, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8022562C
    rlwinm r0, r23, 0, 24, 31
    cmplwi r0, 0x80
    bne @L_8022563C
@L_8022562C:
    lis r3, lbl_80379868@ha
    addi r0, r3, lbl_80379868@l
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_8022563C:
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x27
    beq @L_802256AC
    mr r3, r29
    li r4, 0x0
    li r5, 0x112
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_802256AC
    lbz r0, 0x3(r31)
    lis r3, lbl_80279EF4@ha
    addi r4, r3, lbl_80279EF4@l
    rlwinm r0, r0, 1, 0, 30
    mr r3, r29
    lhzx r4, r4, r0
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802256AC
    lbz r0, 0x3(r31)
    lis r3, lbl_80279EF4@ha
    addi r4, r3, lbl_80279EF4@l
    li r5, 0x0
    rlwinm r0, r0, 1, 0, 30
    mr r3, r29
    lhzx r4, r4, r0
    bl fn_8020248C
@L_802256AC:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r29
    li r4, 0xb
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80225730
    mr r3, r29
    li r4, 0xb
    li r5, 0x0
    bl fn_8020248C
    mr r3, r30
    li r4, 0x22
    li r5, 0x0
    bl fn_8020248C
    mr r3, r15
    mr r4, r25
    bl fn_8020A2B8
    lbz r0, 0x3(r31)
    lis r3, lbl_80399F58@ha
    addi r4, r3, lbl_80399F58@l
    lwz r3, lbl_8047B62C(r13)
    rlwinm r0, r0, 2, 0, 29
    li r5, 0x0
    lwzx r4, r4, r0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225730:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r4, r30
    li r3, 0x0
    bl fn_801F4354
    mr. r16, r3
    beq @L_802257C4
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801FB1C0
    mr r15, r3
    mr r3, r30
    bl fn_80203E0C
    rlwinm r0, r3, 0, 24, 31
    lis r3, 0x1
    mulli r0, r0, 0x5
    subi r7, r3, 0x1
    add r0, r15, r0
    cmplwi r0, 0xffff
    bgt @L_80225790
    mr r7, r0
@L_80225790:
    mr r3, r16
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801FAA58
    lbz r0, 0x3(r31)
    lis r3, lbl_80399F58@ha
    addi r4, r3, lbl_80399F58@l
    lwz r3, lbl_8047B62C(r13)
    rlwinm r0, r0, 2, 0, 29
    li r5, 0x0
    lwzx r4, r4, r0
    bl fn_80211B94
@L_802257C4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r29
    bl fn_80203CCC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225818
    bl fn_800E0C54
    rlwinm r6, r3, 0, 16, 31
    li r5, 0x3
    divw r0, r6, r5
    li r3, 0x0
    li r4, 0x0
    mullw r0, r0, r5
    subf r5, r0, r6
    addi r0, r5, 0x3
    stb r0, 0x3(r31)
    bl fn_802249B8
    b @L_80226120
@L_80225818:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r29
    li r4, 0x22
    li r5, 0x0
    bl fn_8020248C
    mr r3, r15
    mr r4, r25
    bl fn_8020A2B8
    mr r3, r29
    li r4, 0x0
    li r5, 0x109
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r29
    li r4, 0xe
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80225918
    mr r3, r29
    mr r5, r17
    li r4, 0xe
    bl fn_8020248C
    mr r3, r29
    mr r5, r28
    li r4, 0xe
    bl fn_80201B2C
    li r0, 0x0
    la r6, lbl_80478D78(r13)
    lis r3, lbl_80279F6C@ha
    stb r0, 0x5(r6)
    rlwinm r5, r28, 0, 16, 31
    addi r4, r3, lbl_80279F6C@l
    b @L_802258DC
@L_802258C0:
    clrlslwi r0, r0, 24, 1
    lhzx r0, r4, r0
    cmplw r5, r0
    beq @L_802258E8
    lbz r3, 0x5(r6)
    addi r0, r3, 0x1
    stb r0, 0x5(r6)
@L_802258DC:
    lbz r0, 0x5(r6)
    cmplwi r0, 0x5
    blt @L_802258C0
@L_802258E8:
    lbz r0, 0x3(r31)
    lis r3, lbl_80399F58@ha
    addi r4, r3, lbl_80399F58@l
    lwz r3, lbl_8047B62C(r13)
    rlwinm r0, r0, 2, 0, 29
    li r5, 0x0
    lwzx r4, r4, r0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225918:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x164
    bne @L_80225968
    mr r3, r29
    li r4, 0x10
    bl fn_80203B5C
    rlwinm r15, r3, 0, 16, 31
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x3
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    add r15, r0, r15
    subi r15, r15, 0x1
    b @L_80225984
@L_80225968:
    mr r3, r25
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_8011BEB4
    srawi r0, r3, 2
    addze r15, r0
@L_80225984:
    cmpwi r15, 0x0
    bne @L_80225990
    li r15, 0x1
@L_80225990:
    mr r3, r25
    mr r7, r15
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lbz r0, 0x3(r31)
    lis r3, lbl_80399F58@ha
    addi r4, r3, lbl_80399F58@l
    lwz r3, lbl_8047B62C(r13)
    rlwinm r0, r0, 2, 0, 29
    li r5, 0x0
    lwzx r4, r4, r0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    subi r0, r4, 0xe
    rlwinm r5, r26, 0, 24, 31
    li r3, 0x10
    li r6, 0x0
    rlwinm r4, r0, 0, 24, 31
    bl fn_8021B910
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80225A08
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225A08:
    lbz r0, 0x3(r31)
    lis r3, lbl_80379F58@ha
    addi r4, r3, lbl_80379F58@l
    rlwinm r5, r0, 0, 26, 31
    lis r3, lbl_8037631E@ha
    addis r6, r4, 0x1
    li r0, 0x0
    addi r4, r3, lbl_8037631E@l
    stb r5, 0x60a4(r6)
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    stb r0, 0x60a5(r6)
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    subi r0, r4, 0x15
    rlwinm r5, r26, 0, 24, 31
    li r3, 0x90
    li r6, 0x0
    rlwinm r4, r0, 0, 24, 31
    bl fn_8021B910
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80225A7C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225A7C:
    lbz r0, 0x3(r31)
    lis r3, lbl_80379F58@ha
    addi r4, r3, lbl_80379F58@l
    rlwinm r5, r0, 0, 26, 31
    lis r3, lbl_803763CE@ha
    addis r6, r4, 0x1
    li r0, 0x0
    addi r4, r3, lbl_803763CE@l
    stb r5, 0x60a4(r6)
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    stb r0, 0x60a5(r6)
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    subi r0, r4, 0x26
    rlwinm r5, r26, 0, 24, 31
    li r3, 0x20
    li r6, 0x0
    rlwinm r4, r0, 0, 24, 31
    bl fn_8021B910
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80225AF0
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225AF0:
    lbz r0, 0x3(r31)
    lis r3, lbl_80379F58@ha
    addi r4, r3, lbl_80379F58@l
    rlwinm r5, r0, 0, 26, 31
    lis r3, lbl_8037631E@ha
    addis r6, r4, 0x1
    li r0, 0x0
    addi r4, r3, lbl_8037631E@l
    stb r5, 0x60a4(r6)
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    stb r0, 0x60a5(r6)
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    subi r0, r4, 0x2d
    rlwinm r5, r26, 0, 24, 31
    li r3, 0xa0
    li r6, 0x0
    rlwinm r4, r0, 0, 24, 31
    bl fn_8021B910
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80225B64
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225B64:
    lbz r0, 0x3(r31)
    lis r3, lbl_80379F58@ha
    addi r4, r3, lbl_80379F58@l
    rlwinm r5, r0, 0, 26, 31
    lis r3, lbl_803763CE@ha
    addis r6, r4, 0x1
    li r0, 0x0
    addi r4, r3, lbl_803763CE@l
    stb r5, 0x60a4(r6)
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    stb r0, 0x60a5(r6)
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r29
    li r4, 0x12
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80225BD0
    mr r3, r29
    li r4, 0x12
    li r5, 0x0
    bl fn_8020248C
@L_80225BD0:
    mr r3, r15
    mr r4, r25
    bl fn_8020A2B8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r30
    li r4, 0x13
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80225C14
    mr r3, r30
    li r4, 0x13
    li r5, 0x0
    bl fn_8020248C
@L_80225C14:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225C5C
    mr r3, r30
    bl fn_80204A10
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80225C5C
    li r0, 0x0
    b @L_80225CA0
@L_80225C5C:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225C9C
    mr r3, r30
    li r4, 0x3d
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225C9C
    li r0, 0x0
    b @L_80225CA0
@L_80225C9C:
    li r0, 0x1
@L_80225CA0:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80225CB8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225CB8:
    mr r3, r30
    bl fn_802040E8
    mr r16, r3
    mr r3, r18
    bl fn_80207BF4
    mr r15, r3
    mr r3, r18
    bl fn_802040E8
    rlwinm r0, r15, 0, 16, 31
    mr r15, r3
    cmplwi r0, 0x3c
    bne @L_80225D00
    rlwinm. r0, r15, 0, 16, 31
    beq @L_80225D00
    lis r3, lbl_803798D8@ha
    addi r0, r3, lbl_803798D8@l
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225D00:
    rlwinm. r0, r16, 0, 16, 31
    bne @L_80225DF8
    rlwinm r0, r15, 0, 16, 31
    cmplwi r0, 0xaf
    beq @L_80225DF8
    cmplwi r0, 0x0
    beq @L_80225DF8
    mr r3, r15
    bl fn_80142984
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225DF8
    mr r3, r18
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_8020147C
    rlwinm r15, r15, 0, 16, 31
    mr r3, r30
    mr r7, r15
    li r4, 0x0
    li r5, 0xfb
    li r6, 0x0
    bl fn_801254B4
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225D88
    mr r3, r30
    li r4, 0x82
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80225D88:
    mr r3, r18
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225DB0
    mr r3, r18
    li r4, 0x82
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80225DB0:
    mr r3, r18
    li r4, 0x36
    bl fn_80202810
    mr r7, r15
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_8037958A@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037958A@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80225DF8:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r29
    li r4, 0x16
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80225E3C
    mr r3, r30
    mr r4, r24
    bl fn_801F0134
    mr r5, r3
    mr r3, r29
    li r4, 0x16
    bl fn_8020248C
@L_80225E3C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r29
    li r4, 0x17
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80225E74
    mr r3, r29
    li r4, 0x17
    li r5, 0x0
    bl fn_8020248C
@L_80225E74:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    lis r4, lbl_80378E5A@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378E5A@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    lis r4, lbl_80378EFB@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378EFB@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r18
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225F2C
    mr r3, r18
    li r4, 0x5
    bl fn_80202810
    mr r3, r18
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80225F18
    mr r3, r18
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_80225F18:
    lis r4, lbl_8037943C@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037943C@l
    li r5, 0x0
    bl fn_80211B94
@L_80225F2C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    lis r4, lbl_803790E7@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803790E7@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r25
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    bl fn_8011BEB4
    li r0, 0x3
    divw. r7, r3, r0
    bne @L_80225F84
    li r7, 0x1
@L_80225F84:
    mr r3, r25
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lbz r0, 0x3(r31)
    lis r3, lbl_80399F58@ha
    addi r4, r3, lbl_80399F58@l
    lwz r3, lbl_8047B62C(r13)
    rlwinm r0, r0, 2, 0, 29
    li r5, 0x0
    lwzx r4, r4, r0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    mr r3, r29
    li r4, 0xd
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80226010
    mr r3, r29
    li r4, 0xd
    li r5, 0x0
    bl fn_8020248C
    mr r3, r29
    li r4, 0x22
    li r5, 0x0
    bl fn_8020248C
    mr r3, r15
    mr r4, r25
    bl fn_8020A2B8
    b @L_80226120
@L_80226010:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    rlwinm r0, r20, 0, 16, 31
    cmplwi r0, 0x3c
    bne @L_80226044
    rlwinm. r0, r21, 0, 16, 31
    beq @L_80226044
    lis r3, lbl_803798D8@ha
    addi r0, r3, lbl_803798D8@l
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80226044:
    rlwinm. r0, r21, 0, 16, 31
    beq @L_802260E0
    mr r3, r29
    li r4, 0x3d
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802260E0
    rlwinm r7, r21, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r29
    li r4, 0x3d
    li r5, 0x0
    bl fn_8020248C
    mr r3, r29
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802260B0
    mr r3, r29
    li r4, 0xcd
    li r5, 0x0
    bl fn_801FE710
@L_802260B0:
    mr r3, r29
    li r4, 0x36
    bl fn_80202810
    lis r4, lbl_8037913D@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037913D@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_802260E0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
    lis r4, lbl_803791D0@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803791D0@l
    li r5, 0x0
    bl fn_80211B94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_80226120
@L_80226114:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_80226120:
    lmw r15, 0xc(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
