    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r5, 0x11c
    li r6, 0x0
    stw r0, 0x54(r1)
    stmw r16, 0x10(r1)
    mr r31, r4
    mr r30, r3
    li r4, 0x0
    mr r3, r31
    bl fn_8012640C
    extsh r25, r3
    mr r3, r31
    li r4, 0x0
    li r5, 0x11e
    li r6, 0x0
    bl fn_8012640C
    extsh r24, r3
    mr r3, r30
    bl fn_80207BF4
    mr r16, r3
    mr r3, r31
    bl fn_80207BF4
    mr r26, r3
    mr r3, r30
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r23, r0
    li r5, 0x7
    mr r4, r23
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r22, r3, 0, 16, 31
    mr r3, r30
    bl fn_80205134
    mr r21, r3
    mr r4, r23
    li r3, 0x0
    li r5, 0xd
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r20, r3, 0, 24, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r19, r3
    bl fn_802096E8
    mr r27, r3
    mr r3, r30
    li r4, 0x0
    li r5, 0x107
    li r6, 0x0
    bl fn_8012640C
    lhz r4, -20640(r2)
    rlwinm r18, r3, 0, 24, 31
    lbz r0, -20638(r2)
    mr r3, r30
    sth r4, 0x8(r1)
    li r17, 0x0
    stb r0, 0xa(r1)
    bl fn_802016A4
    mr r28, r3
    mr r3, r31
    bl fn_802016A4
    mr r29, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r31
    bl fn_801F0134
    rlwinm r4, r26, 0, 16, 31
    mr r26, r3
    subi r0, r4, 0x9
    cmplwi r0, 0x2f
    bgt @L_8022DCA0
    lis r3, jumptable_8039A478@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A478@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_8022DCA0
    cmplwi r22, 0x0
    beq @L_8022DCA0
    cmplwi r0, 0x164
    beq @L_8022DCA0
    extsh. r0, r25
    bne @L_8022D850
    extsh. r0, r24
    beq @L_8022DCA0
@L_8022D850:
    mr r3, r31
    mr r4, r21
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022DCA0
    mr r3, r31
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    li r16, 0x0
    b @L_8022D894
@L_8022D880:
    mr r3, r31
    mr r4, r16
    mr r5, r21
    bl fn_80207B5C
    addi r16, r16, 0x1
@L_8022D894:
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_8022D880
    mr r3, r21
    bl fn_8010C4D4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lis r4, lbl_803798F3@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803798F3@l
    li r5, 0x0
    bl fn_80211B94
    li r17, 0x1
    b @L_8022DCA0
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    cmplwi r18, 0x0
    bne @L_8022DCA0
    extsh. r0, r25
    bne @L_8022D90C
    extsh. r0, r24
    beq @L_8022DCA0
@L_8022D90C:
    cmplwi r20, 0x0
    beq @L_8022DCA0
    mr r3, r30
    li r4, 0x10
    bl fn_80203B5C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r19
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r4, lbl_80379907@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379907@l
    li r5, 0x0
    bl fn_80211B94
    li r17, 0x1
    b @L_8022DCA0
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    cmplwi r18, 0x0
    bne @L_8022DCA0
    extsh. r0, r25
    bne @L_8022D98C
    extsh. r0, r24
    beq @L_8022DCA0
@L_8022D98C:
    cmplwi r20, 0x0
    beq @L_8022DCA0
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0xa
    divw r0, r4, r3
    mullw r0, r0, r3
    subf. r0, r0, r4
    bne @L_8022DCA0
    bl fn_800E0C54
    rlwinm r8, r3, 0, 16, 31
    li r6, 0x3
    divwu r5, r8, r6
    lwz r0, lbl_8047B618(r13)
    lis r4, lbl_8037992F@ha
    lwz r3, lbl_8047B62C(r13)
    ori r0, r0, 0x2000
    stw r0, lbl_8047B618(r13)
    mullw r7, r5, r6
    addi r6, r1, 0x8
    la r9, lbl_80478D78(r13)
    addi r4, r4, lbl_8037992F@l
    li r5, 0x0
    subf r0, r7, r8
    rlwinm r0, r0, 0, 24, 31
    lbzx r6, r6, r0
    stb r6, 0x3(r9)
    addi r0, r6, 0x40
    stb r0, 0x3(r9)
    bl fn_80211B94
    li r17, 0x1
    b @L_8022DCA0
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    cmplwi r18, 0x0
    bne @L_8022DCA0
    extsh. r0, r25
    bne @L_8022DA44
    extsh. r0, r24
    beq @L_8022DCA0
@L_8022DA44:
    cmplwi r20, 0x0
    beq @L_8022DCA0
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x3
    divw r0, r4, r3
    mullw r0, r0, r3
    subf. r0, r0, r4
    bne @L_8022DCA0
    lwz r0, lbl_8047B618(r13)
    lis r3, lbl_8037992F@ha
    addi r4, r3, lbl_8037992F@l
    li r5, 0x42
    ori r0, r0, 0x2000
    la r3, lbl_80478D78(r13)
    stb r5, 0x3(r3)
    li r5, 0x0
    lwz r3, lbl_8047B62C(r13)
    stw r0, lbl_8047B618(r13)
    bl fn_80211B94
    li r17, 0x1
    b @L_8022DCA0
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    cmplwi r18, 0x0
    bne @L_8022DCA0
    extsh. r0, r25
    bne @L_8022DAD4
    extsh. r0, r24
    beq @L_8022DCA0
@L_8022DAD4:
    cmplwi r20, 0x0
    beq @L_8022DCA0
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x3
    divw r0, r4, r3
    mullw r0, r0, r3
    subf. r0, r0, r4
    bne @L_8022DCA0
    lwz r0, lbl_8047B618(r13)
    lis r3, lbl_8037992F@ha
    addi r4, r3, lbl_8037992F@l
    li r5, 0x45
    ori r0, r0, 0x2000
    la r3, lbl_80478D78(r13)
    stb r5, 0x3(r3)
    li r5, 0x0
    lwz r3, lbl_8047B62C(r13)
    stw r0, lbl_8047B618(r13)
    bl fn_80211B94
    li r17, 0x1
    b @L_8022DCA0
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    cmplwi r18, 0x0
    bne @L_8022DCA0
    cmplwi r20, 0x0
    beq @L_8022DCA0
    extsh. r0, r25
    bne @L_8022DB6C
    extsh. r0, r24
    beq @L_8022DCA0
@L_8022DB6C:
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x3
    divw r0, r4, r3
    mullw r0, r0, r3
    subf. r0, r0, r4
    bne @L_8022DCA0
    lwz r0, lbl_8047B618(r13)
    lis r3, lbl_8037992F@ha
    addi r4, r3, lbl_8037992F@l
    li r5, 0x43
    ori r0, r0, 0x2000
    la r3, lbl_80478D78(r13)
    stb r5, 0x3(r3)
    li r5, 0x0
    lwz r3, lbl_8047B62C(r13)
    stw r0, lbl_8047B618(r13)
    bl fn_80211B94
    li r17, 0x1
    b @L_8022DCA0
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    cmplwi r18, 0x0
    bne @L_8022DCA0
    cmplwi r20, 0x0
    beq @L_8022DCA0
    extsh. r0, r25
    bne @L_8022DBFC
    extsh. r0, r24
    beq @L_8022DCA0
@L_8022DBFC:
    mr r3, r31
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022DCA0
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x3
    divw r0, r4, r3
    mullw r0, r0, r3
    subf. r0, r0, r4
    bne @L_8022DCA0
    rlwinm r0, r16, 0, 16, 31
    cmplwi r0, 0xc
    beq @L_8022DCA0
    rlwinm r3, r28, 0, 24, 31
    rlwinm r0, r29, 0, 24, 31
    cmplw r3, r0
    beq @L_8022DCA0
    mr r3, r30
    li r4, 0xa
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022DCA0
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_8022DCA0
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_8022DCA0
    mr r3, r30
    mr r5, r26
    li r4, 0xa
    bl fn_8020248C
    lis r4, lbl_80379932@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379932@l
    li r5, 0x0
    bl fn_80211B94
    li r17, 0x1
@L_8022DCA0:
    mr r3, r17
    lmw r16, 0x10(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
