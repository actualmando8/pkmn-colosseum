    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    stw r0, 0x54(r1)
    rlwinm. r0, r5, 0, 25, 25
    stmw r17, 0x14(r1)
    mr r17, r5
    mr r19, r3
    mr r27, r4
    mr r26, r6
    li r25, 0x0
    li r21, 0x0
    beq @L_8021B958
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r22, 0x0
    mr r30, r3
    b @L_8021B96C
@L_8021B958:
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    li r22, 0x1
    mr r30, r3
@L_8021B96C:
    mr r4, r30
    li r3, 0x2
    bl fn_801F025C
    mr r0, r3
    mr r3, r30
    mr r20, r0
    bl fn_80207BF4
    mr r24, r3
    mr r3, r30
    bl fn_80205184
    rlwinm r28, r17, 0, 24, 31
    mr r23, r3
    andi. r0, r28, 0xbf
    rlwinm. r0, r0, 0, 24, 24
    beq @L_8021B9AC
    li r25, 0x1
@L_8021B9AC:
    rlwinm. r0, r28, 0, 26, 26
    beq @L_8021B9B8
    li r21, 0x1
@L_8021B9B8:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x7
    bgt @L_8021BA14
    lis r3, jumptable_8039A048@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A048@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r31, 0xe6
    b @L_8021BA18
    li r31, 0xe7
    b @L_8021BA18
    li r31, 0xea
    b @L_8021BA18
    li r31, 0xe8
    b @L_8021BA18
    li r31, 0xe9
    b @L_8021BA18
    li r31, 0xeb
    b @L_8021BA18
    li r31, 0xec
    b @L_8021BA18
@L_8021BA14:
    li r31, 0x0
@L_8021BA18:
    mr r3, r30
    mr r5, r31
    li r4, 0x0
    li r6, 0x0
    bl fn_8012640C
    lis r4, lbl_80279E7C@ha
    clrlslwi r0, r27, 24, 2
    addi r4, r4, lbl_80279E7C@l
    extsb r29, r3
    lwzx r3, r4, r0
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    rlwinm r0, r19, 0, 24, 31
    cmpwi r0, 0x90
    beq @L_8021BA94
    bge @L_8021BA78
    cmpwi r0, 0x20
    beq @L_8021BA8C
    bge @L_8021BAA4
    cmpwi r0, 0x10
    beq @L_8021BA84
    b @L_8021BAA4
@L_8021BA78:
    cmpwi r0, 0xa0
    beq @L_8021BA9C
    b @L_8021BAA4
@L_8021BA84:
    li r27, 0x1
    b @L_8021BAA8
@L_8021BA8C:
    li r27, 0x2
    b @L_8021BAA8
@L_8021BA94:
    li r27, -0x1
    b @L_8021BAA8
@L_8021BA9C:
    li r27, -0x2
    b @L_8021BAA8
@L_8021BAA4:
    li r27, 0x0
@L_8021BAA8:
    extsb. r0, r27
    bge @L_8021BF2C
    mr r3, r20
    li r4, 0x4c
    bl fn_801F6E98
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021BB5C
    rlwinm. r0, r25, 0, 24, 31
    bne @L_8021BB5C
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0xae
    beq @L_8021BB5C
    rlwinm r0, r28, 0, 27, 31
    cmplwi r0, 0x1
    bne @L_8021BB54
    mr r3, r30
    li r4, 0x0
    li r5, 0x113
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_8021BB0C
    stw r26, lbl_8047B610(r13)
    b @L_8021BB54
@L_8021BB0C:
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x113
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lis r4, lbl_80378CEB@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378CEB@l
    li r5, 0x0
    bl fn_80211B94
    stw r26, lbl_8047B610(r13)
@L_8021BB54:
    li r3, 0x1
    b @L_8021C020
@L_8021BB5C:
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0xae
    beq @L_8021BD30
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021BD30
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    bl fn_80205184
    mr r18, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r4, r18
    mr r19, r3
    li r18, 0x0
    li r3, 0x0
    li r5, 0xe
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r20, r3, 0, 24, 31
    mr r3, r19
    li r4, 0x2b
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021BBDC
    cmplwi r20, 0x1
    bne @L_8021BBDC
    li r0, 0x1
    b @L_8021BBE0
@L_8021BBDC:
    li r0, 0x0
@L_8021BBE0:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021BD14
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r20, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r18, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r21, r3
    mr r3, r18
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021BC88
    mr r3, r21
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r21
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021BD08
@L_8021BC88:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r18, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r17, r3
    li r3, 0x2
    mr r4, r17
    bl fn_801F025C
    mr r19, r3
    mr r3, r17
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021BCF8
    cmplw r18, r19
    beq @L_8021BCF8
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_8021BCF8
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_8021BCF8:
    mr r3, r20
    mr r4, r21
    li r5, 0x0
    bl fn_8022DCB8
@L_8021BD08:
    li r18, 0x1
    la r3, lbl_80478D78(r13)
    stb r18, 0x6(r3)
@L_8021BD14:
    rlwinm. r0, r18, 0, 24, 31
    beq @L_8021BD30
    lis r4, lbl_80377B05@ha
    li r3, 0x1
    addi r0, r4, lbl_80377B05@l
    stw r0, lbl_8047B610(r13)
    b @L_8021C020
@L_8021BD30:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x1d
    beq @L_8021BD44
    cmplwi r0, 0x49
    bne @L_8021BDD8
@L_8021BD44:
    rlwinm. r0, r25, 0, 24, 31
    bne @L_8021BDD8
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0xae
    beq @L_8021BDD8
    rlwinm r0, r28, 0, 27, 31
    cmplwi r0, 0x1
    bne @L_8021BDD0
    mr r3, r30
    li r4, 0x0
    li r5, 0x113
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_8021BD88
    stw r26, lbl_8047B610(r13)
    b @L_8021BDD0
@L_8021BD88:
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0x113
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    lis r4, lbl_803797F1@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803797F1@l
    li r5, 0x0
    bl fn_80211B94
    stw r26, lbl_8047B610(r13)
@L_8021BDD0:
    li r3, 0x1
    b @L_8021C020
@L_8021BDD8:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x33
    bne @L_8021BE3C
    rlwinm. r0, r25, 0, 24, 31
    bne @L_8021BE3C
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0xeb
    bne @L_8021BE3C
    rlwinm r0, r28, 0, 27, 31
    cmplwi r0, 0x1
    bne @L_8021BE34
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_803798BB@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803798BB@l
    li r5, 0x0
    bl fn_80211B94
    stw r26, lbl_8047B610(r13)
@L_8021BE34:
    li r3, 0x1
    b @L_8021C020
@L_8021BE3C:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x34
    bne @L_8021BEA0
    rlwinm. r0, r25, 0, 24, 31
    bne @L_8021BEA0
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0xe6
    bne @L_8021BEA0
    rlwinm r0, r28, 0, 27, 31
    cmplwi r0, 0x1
    bne @L_8021BE98
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    lis r4, lbl_803798BB@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803798BB@l
    li r5, 0x0
    bl fn_80211B94
    stw r26, lbl_8047B610(r13)
@L_8021BE98:
    li r3, 0x1
    b @L_8021C020
@L_8021BEA0:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0x13
    bne @L_8021BEBC
    rlwinm. r0, r28, 0, 27, 31
    bne @L_8021BEBC
    li r3, 0x1
    b @L_8021C020
@L_8021BEBC:
    extsb r0, r27
    cmpwi r0, -0x2
    bgt @L_8021BEE0
    li r3, 0x7628
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    b @L_8021BEF4
@L_8021BEE0:
    li r3, 0x76bd
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
@L_8021BEF4:
    li r3, 0x7629
    bl fn_800FA280
    mr r4, r3
    li r3, 0x41
    bl fn_80132A38
    extsb. r0, r29
    bgt @L_8021BF20
    li r0, 0x2
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021BF98
@L_8021BF20:
    la r3, lbl_80478D78(r13)
    stb r22, 0x5(r3)
    b @L_8021BF98
@L_8021BF2C:
    extsb r0, r27
    cmpwi r0, 0x2
    blt @L_8021BF50
    li r3, 0x7626
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
    b @L_8021BF64
@L_8021BF50:
    li r3, 0x76bd
    bl fn_800FA280
    mr r4, r3
    li r3, 0xe
    bl fn_80132A38
@L_8021BF64:
    li r3, 0x7627
    bl fn_800FA280
    mr r4, r3
    li r3, 0x41
    bl fn_80132A38
    cmpwi r29, 0xc
    blt @L_8021BF90
    li r0, 0x2
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021BF98
@L_8021BF90:
    la r3, lbl_80478D78(r13)
    stb r22, 0x5(r3)
@L_8021BF98:
    add r29, r29, r27
    extsb. r0, r29
    bge @L_8021BFA8
    li r29, 0x0
@L_8021BFA8:
    extsb r0, r29
    cmpwi r0, 0xc
    ble @L_8021BFB8
    li r29, 0xc
@L_8021BFB8:
    mr r3, r30
    mr r5, r31
    extsb r7, r29
    li r4, 0x0
    li r6, 0x0
    bl fn_801254B4
    la r17, lbl_80478D78(r13)
    lbz r0, 0x5(r17)
    cmplwi r0, 0x2
    bne @L_8021C000
    rlwinm. r0, r28, 0, 31, 31
    beq @L_8021C000
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
@L_8021C000:
    lbz r0, 0x5(r17)
    cmplwi r0, 0x2
    bne @L_8021C01C
    rlwinm. r0, r28, 0, 31, 31
    bne @L_8021C01C
    li r3, 0x1
    b @L_8021C020
@L_8021C01C:
    li r3, 0x0
@L_8021C020:
    lmw r17, 0x14(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
