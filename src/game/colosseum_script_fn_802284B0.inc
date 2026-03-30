    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x54(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r16, 0x10(r1)
    bl fn_801F54A4
    lwz r5, lbl_8047B610(r13)
    rlwinm r16, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    lhz r31, 0x5(r5)
    bl fn_801F025C
    mr r4, r16
    mr r27, r3
    bl fn_801F0134
    mr r17, r3
    mr r3, r27
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r27
    mr r25, r0
    bl fn_80207BF4
    mr r28, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r26, r3
    bl fn_80207BF4
    mr r29, r3
    mr r3, r26
    bl fn_80203FE4
    mr r22, r3
    mr r3, r26
    bl fn_80203EDC
    mr r21, r3
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    rlwinm r0, r31, 0, 16, 31
    mr r30, r3
    cmplwi r0, 0xffff
    beq @L_80228578
    cmplwi r0, 0xfffe
    bne @L_802287B4
@L_80228578:
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0xffff
    bne @L_802285C8
    mr r3, r26
    li r4, 0x1d
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802285C8
    mr r3, r26
    li r4, 0x1d
    bl fn_80201D84
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r17, 0, 16, 31
    cmplw r0, r3
    bne @L_802285C8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
    b @L_80228D98
@L_802285C8:
    mr r3, r26
    bl fn_802010C8
    rlwinm. r0, r3, 0, 16, 31
    beq @L_802285E8
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80228D98
@L_802285E8:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    bl fn_80205184
    mr r17, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r4, r17
    mr r18, r3
    li r17, 0x0
    li r3, 0x0
    li r5, 0xe
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r19, r3, 0, 24, 31
    mr r3, r18
    li r4, 0x2b
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228650
    cmplwi r19, 0x1
    bne @L_80228650
    li r0, 0x1
    b @L_80228654
@L_80228650:
    li r0, 0x0
@L_80228654:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022879C
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
    mr r17, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r19, r3
    mr r3, r17
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802286FC
    mr r3, r19
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r19
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80228790
@L_802286FC:
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
    mr r21, r3
    li r3, 0x2
    mr r4, r21
    bl fn_801F025C
    mr r17, r3
    mr r3, r21
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022876C
    cmplw r18, r17
    beq @L_8022876C
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_8022876C
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_8022876C:
    mr r3, r20
    mr r4, r19
    li r5, 0x0
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80228790
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_80228790:
    li r17, 0x1
    la r3, lbl_80478D78(r13)
    stb r17, 0x6(r3)
@L_8022879C:
    rlwinm. r0, r17, 0, 24, 31
    bne @L_80228D98
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
    b @L_80228D98
@L_802287B4:
    cmplwi r0, 0x0
    bne @L_802287DC
    mr r3, r27
    bl fn_80205184
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205134
    mr r23, r3
    b @L_802287F4
@L_802287DC:
    mr r4, r31
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r23, r3, 0, 16, 31
@L_802287F4:
    mr r4, r31
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r24, r3, 0, 24, 31
    mr r3, r31
    mr r4, r27
    mr r5, r26
    bl fn_80229934
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228850
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x45
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80228D98
@L_80228850:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    bl fn_80205184
    mr r17, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r4, r17
    mr r18, r3
    li r17, 0x0
    li r3, 0x0
    li r5, 0xe
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r19, r3, 0, 24, 31
    mr r3, r18
    li r4, 0x2b
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802288B8
    cmplwi r19, 0x1
    bne @L_802288B8
    li r0, 0x1
    b @L_802288BC
@L_802288B8:
    li r0, 0x0
@L_802288BC:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228A04
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
    mr r19, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r17, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r20, r3
    mr r3, r17
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80228964
    mr r3, r20
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r20
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_802289F8
@L_80228964:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r17, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r16, r3
    li r3, 0x2
    mr r4, r16
    bl fn_801F025C
    mr r18, r3
    mr r3, r16
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802289D4
    cmplw r17, r18
    beq @L_802289D4
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_802289D4
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_802289D4:
    mr r3, r19
    mr r4, r20
    mr r5, r31
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802289F8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_802289F8:
    li r17, 0x1
    la r3, lbl_80478D78(r13)
    stb r17, 0x6(r3)
@L_80228A04:
    rlwinm. r0, r17, 0, 24, 31
    bne @L_80228D98
    mr r3, r27
    mr r4, r26
    mr r5, r31
    bl fn_80228DAC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80228D98
    mr r3, r27
    li r4, 0x0
    li r5, 0xeb
    li r6, 0x0
    bl fn_8012640C
    rlwinm r16, r3, 0, 24, 31
    mr r3, r26
    li r4, 0x0
    li r5, 0xec
    li r6, 0x0
    bl fn_8012640C
    rlwinm r17, r3, 0, 24, 31
    mr r3, r26
    li r4, 0x19
    bl fn_802026E4
    rlwinm r4, r3, 0, 24, 31
    subfic r0, r17, 0xc
    add r3, r16, r0
    subi r0, r3, 0x6
    cmplwi r4, 0x1
    extsb r0, r0
    bne @L_80228A80
    extsb r0, r16
@L_80228A80:
    extsb r3, r0
    bl fn_8020E4CC
    extsb r16, r3
    mr r4, r31
    li r3, 0x0
    li r5, 0x6
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r30, 0, 24, 31
    rlwinm r3, r3, 0, 24, 31
    cmplwi r0, 0x1
    mr r17, r3
    bne @L_80228AD8
    mr r4, r31
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x98
    bne @L_80228AD8
    li r17, 0x32
@L_80228AD8:
    mr r3, r16
    bl fn_8020A040
    bl fn_8020A028
    rlwinm r0, r3, 0, 24, 31
    mr r3, r16
    mullw r0, r17, r0
    rlwinm r16, r0, 0, 16, 31
    bl fn_8020A040
    bl fn_8020A010
    rlwinm r3, r3, 0, 24, 31
    rlwinm r0, r28, 0, 16, 31
    divw r3, r16, r3
    cmplwi r0, 0xe
    rlwinm r16, r3, 0, 16, 31
    bne @L_80228B24
    mulli r3, r16, 0x82
    li r0, 0x64
    divw r0, r3, r0
    rlwinm r16, r0, 0, 16, 31
@L_80228B24:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x3
    bne @L_80228B50
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x8
    bne @L_80228B50
    rlwinm r3, r16, 0, 16, 31
    li r0, 0x64
    mulli r3, r3, 0x50
    divw r0, r3, r0
    rlwinm r16, r0, 0, 16, 31
@L_80228B50:
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x37
    bne @L_80228B84
    mr r3, r23
    bl fn_8010C4A0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228B84
    rlwinm r3, r16, 0, 16, 31
    li r0, 0x64
    mulli r3, r3, 0x50
    divw r0, r3, r0
    rlwinm r16, r0, 0, 16, 31
@L_80228B84:
    rlwinm r0, r22, 0, 16, 31
    cmplwi r0, 0x16
    bne @L_80228BA8
    rlwinm r3, r16, 0, 16, 31
    subfic r0, r21, 0x64
    mullw r3, r3, r0
    li r0, 0x64
    divw r0, r3, r0
    rlwinm r16, r0, 0, 16, 31
@L_80228BA8:
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    li r4, 0x64
    divw r3, r5, r4
    rlwinm r0, r16, 0, 16, 31
    mullw r3, r3, r4
    subf r3, r3, r5
    addi r3, r3, 0x1
    cmpw r3, r0
    ble @L_80228C80
    mr r3, r25
    li r4, 0x40
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80228BF8
    mr r3, r25
    li r4, 0x40
    li r5, 0x0
    bl fn_802097C8
@L_80228BF8:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x19
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_80228C38
    cmplwi r24, 0x4
    beq @L_80228C28
    cmplwi r24, 0x6
    bne @L_80228C38
@L_80228C28:
    li r0, 0x2
    la r3, lbl_80478D78(r13)
    stb r0, 0x6(r3)
    b @L_80228C44
@L_80228C38:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x6(r3)
@L_80228C44:
    la r16, lbl_80478D78(r13)
    li r3, 0x0
    lbz r17, 0x6(r16)
    li r4, 0x0
    li r5, 0x0
    li r6, 0x1
    bl fn_802274F0
    lbz r0, 0x6(r16)
    cmplwi r0, 0x3
    bne @L_80228C80
    cmplwi r17, 0x2
    beq @L_80228C7C
    cmplwi r17, 0x0
    bne @L_80228C80
@L_80228C7C:
    stb r17, 0x6(r16)
@L_80228C80:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r21, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r18, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r17, r3
    mr r3, r18
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80228D04
    mr r3, r17
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r17
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80228D98
@L_80228D04:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r20, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r18, r3
    li r3, 0x2
    mr r4, r18
    bl fn_801F025C
    mr r19, r3
    mr r3, r18
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228D74
    cmplw r20, r19
    beq @L_80228D74
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_80228D74
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_80228D74:
    mr r3, r21
    mr r4, r17
    mr r5, r31
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80228D98
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_80228D98:
    lmw r16, 0x10(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
