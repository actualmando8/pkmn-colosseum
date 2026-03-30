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
    rlwinm r0, r3, 0, 16, 31
    sth r0, 0x8(r1)
    li r20, 0x0
    lbz r0, 0x2(r5)
    li r3, 0x11
    lbz r30, 0x1(r5)
    li r4, 0x0
    stw r0, 0xc(r1)
    bl fn_801F025C
    mr. r29, r3
    beq @L_8021FBF8
    bl fn_80203FE4
    mr r0, r3
    mr r3, r29
    mr r27, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r22, r3
    bl fn_802096E8
    mr r0, r3
    mr r3, r29
    mr r19, r0
    bl fn_80205184
    mr r0, r3
    mr r3, r29
    mr r25, r0
    bl fn_80205224
    lis r4, 0x1
    mr r23, r3
    subi r0, r4, 0x1
    mr r3, r22
    rlwinm r7, r23, 0, 16, 31
    li r4, 0x0
    rlwinm r0, r0, 0, 16, 31
    li r5, 0x30
    subf r0, r0, r7
    li r6, 0x0
    subic r0, r0, 0x1
    subfe r0, r0, r0
    andc r15, r23, r0
    bl fn_8011BEB4
    rlwinm r26, r3, 0, 16, 31
    mr r4, r25
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r21, r3, 0, 24, 31
    mr r4, r25
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r24, r3, 0, 16, 31
    mr r4, r25
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r16, r3, 0, 16, 31
    b @L_8021FC20
@L_8021FBF8:
    li r27, 0x0
    li r22, 0x0
    li r19, 0x0
    li r25, 0x0
    li r23, 0x0
    li r15, 0x0
    li r26, 0x9
    li r21, 0x1
    li r24, 0x0
    li r16, 0x0
@L_8021FC20:
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr. r28, r3
    beq @L_8021FC64
    li r4, 0x0
    li r5, 0x11c
    li r6, 0x0
    bl fn_8012640C
    extsh r18, r3
    mr r3, r28
    li r4, 0x0
    li r5, 0x11e
    li r6, 0x0
    bl fn_8012640C
    extsh r17, r3
    b @L_8021FC6C
@L_8021FC64:
    li r18, 0x0
    li r17, 0x0
@L_8021FC6C:
    rlwinm r31, r30, 0, 24, 31
@L_8021FC70:
    lbz r0, lbl_8047B627(r13)
    cmplwi r0, 0x11
    bgt @L_802206F8
    lis r3, jumptable_8039A0A8@ha
    rlwinm r0, r0, 2, 0, 29
    addi r3, r3, jumptable_8039A0A8@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    mr r3, r28
    li r4, 0x13
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FD54
    mr r3, r28
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FD54
    cmplw r29, r28
    beq @L_8021FD54
    mr r3, r29
    mr r4, r28
    bl fn_80202B88
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021FD54
    rlwinm r0, r19, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FD54
    extsh. r0, r18
    bne @L_8021FCF8
    extsh. r0, r17
    beq @L_8021FD54
@L_8021FCF8:
    rlwinm. r0, r16, 0, 16, 31
    beq @L_8021FD54
    mr r3, r28
    li r4, 0x0
    li r5, 0xe6
    li r6, 0x0
    bl fn_8012640C
    extsb r3, r3
    cmpwi r3, 0xc
    bge @L_8021FD54
    addi r0, r3, 0x1
    mr r3, r28
    extsb r7, r0
    li r4, 0x0
    li r5, 0xe6
    li r6, 0x0
    bl fn_801254B4
    lis r4, lbl_80378D02@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378D02@l
    li r5, 0x0
    bl fn_80211B94
    li r20, 0x1
@L_8021FD54:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    mr r3, r28
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FE34
    mr r3, r28
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FE34
    cmplw r29, r28
    beq @L_8021FE34
    mr r3, r28
    li r4, 0x0
    li r5, 0x11e
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_8021FE34
    mr r3, r22
    bl fn_802096E8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FE34
    mr r4, r25
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xa
    bne @L_8021FE34
    mr r3, r28
    li r4, 0x7
    bl fn_80202810
    mr r3, r28
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FE1C
    mr r3, r28
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8021FE1C:
    lis r4, lbl_803792C9@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803792C9@l
    li r5, 0x0
    bl fn_80211B94
    li r20, 0x1
@L_8021FE34:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    mr r3, r29
    mr r4, r28
    bl fn_8022D2CC
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    mr r3, r29
    mr r4, r28
    bl fn_8022D6BC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021FE78
    li r20, 0x1
@L_8021FE78:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, -11372
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    mr r3, r29
    mr r4, r28
    bl fn_8022D20C
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 6, 6
    beq @L_8021FF64
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0x1d
    bne @L_8021FF64
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_8021FF64
    cmplwi r0, 0x164
    beq @L_8021FF64
    mr r3, r29
    li r4, 0x36
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8021FF64
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0xe2
    bne @L_8021FF44
    mr r3, r22
    li r4, 0x45
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FF44
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
@L_8021FF44:
    mr r3, r29
    li r4, 0x36
    li r5, 0x0
    bl fn_8020248C
    mr r3, r29
    mr r5, r23
    li r4, 0x36
    bl fn_80201B2C
@L_8021FF64:
    mr r3, r29
    li r4, 0x36
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021FFB4
    mr r3, r29
    li r4, 0x36
    bl fn_80201C58
    mr r0, r3
    mr r3, r29
    mr r14, r0
    bl fn_80205B8C
    mr r4, r14
    bl fn_80123B5C
    extsb. r0, r3
    bge @L_8021FFB4
    mr r3, r29
    li r4, 0x36
    bl fn_80202810
@L_8021FFB4:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    lis r4, -32734
    li r3, 0x0
    addi r4, r4, 2004
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    lis r4, -32733
    li r3, 0x0
    addi r4, r4, -19000
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    mr r3, r29
    mr r4, r28
    bl fn_8022BB84
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    mr r3, r29
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80220088
    mr r3, r29
    bl fn_802010C8
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80220088
    cmplwi r0, 0x1f
    beq @L_8022006C
    mr r3, r29
    li r4, 0x0
    bl fn_80209484
    b @L_80220088
@L_8022006C:
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x3
    bl fn_801DA3CC
@L_80220088:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    mr r3, r29
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80220128
    rlwinm. r0, r19, 0, 24, 31
    beq @L_802200D8
    mr r3, r29
    bl fn_802010C8
    rlwinm. r0, r3, 0, 16, 31
    beq @L_802200D8
    mr r3, r29
    bl fn_80200A5C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80220128
@L_802200D8:
    mr r3, r29
    bl fn_80200E00
    mr r3, r29
    li r4, 0x0
    li r5, 0x115
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    mr r3, r29
    li r4, 0x1
    bl fn_80209484
    mr r3, r29
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_80220128
    li r4, 0x3
    bl fn_801DA36C
@L_80220128:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    mr r3, r28
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802201A8
    mr r3, r28
    li r4, 0x0
    li r5, 0x115
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_802201A8
    mr r3, r28
    bl fn_802010C8
    rlwinm. r0, r3, 0, 16, 31
    bne @L_802201A8
    mr r3, r28
    li r4, 0x1
    bl fn_80209484
    mr r3, r28
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_802201A8
    li r4, 0x3
    bl fn_801DA36C
@L_802201A8:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    lis r4, -32734
    li r3, 0x0
    addi r4, r4, 1912
    li r5, 0x0
    li r6, 0x0
    bl fn_801F37B0
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 19, 19
    beq @L_80220358
    li r3, 0x0
    bl fn_801F22D8
    lwz r0, lbl_8047B618(r13)
    li r3, 0x11
    li r4, 0x0
    rlwinm r0, r0, 0, 20, 18
    stw r0, lbl_8047B618(r13)
    bl fn_801F025C
    mr. r29, r3
    beq @L_802202E4
    bl fn_80203FE4
    mr r0, r3
    mr r3, r29
    mr r27, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r22, r3
    bl fn_802096E8
    mr r0, r3
    mr r3, r29
    mr r19, r0
    bl fn_80205184
    mr r0, r3
    mr r3, r29
    mr r25, r0
    bl fn_80205224
    lis r4, 0x1
    mr r23, r3
    subi r0, r4, 0x1
    mr r3, r22
    rlwinm r7, r23, 0, 16, 31
    li r4, 0x0
    rlwinm r0, r0, 0, 16, 31
    li r5, 0x30
    subf r0, r0, r7
    li r6, 0x0
    subic r0, r0, 0x1
    subfe r0, r0, r0
    andc r15, r23, r0
    bl fn_8011BEB4
    rlwinm r26, r3, 0, 16, 31
    mr r4, r25
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r21, r3, 0, 24, 31
    mr r4, r25
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r24, r3, 0, 16, 31
    mr r4, r25
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r16, r3, 0, 16, 31
    b @L_8022030C
@L_802202E4:
    li r27, 0x0
    li r22, 0x0
    li r19, 0x0
    li r25, 0x0
    li r23, 0x0
    li r15, 0x0
    li r26, 0x9
    li r21, 0x1
    li r24, 0x0
    li r16, 0x0
@L_8022030C:
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr. r28, r3
    beq @L_80220350
    li r4, 0x0
    li r5, 0x11c
    li r6, 0x0
    bl fn_8012640C
    extsh r18, r3
    mr r3, r28
    li r4, 0x0
    li r5, 0x11e
    li r6, 0x0
    bl fn_8012640C
    extsh r17, r3
    b @L_80220358
@L_80220350:
    li r18, 0x0
    li r17, 0x0
@L_80220358:
    mr r4, r15
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7f
    bne @L_802203A0
    mr r4, r15
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7f
    bne @L_802203B8
    rlwinm. r0, r19, 0, 24, 31
    bne @L_802203B8
@L_802203A0:
    mr r3, r29
    rlwinm r7, r23, 0, 16, 31
    li r4, 0x0
    li r5, 0xef
    li r6, 0x0
    bl fn_801254B4
@L_802203B8:
    mr r3, r29
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022052C
    mr r4, r15
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7f
    beq @L_8022052C
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 6, 6
    beq @L_8022042C
    mr r3, r29
    rlwinm r7, r23, 0, 16, 31
    li r4, 0x0
    li r5, 0xf0
    li r6, 0x0
    bl fn_801254B4
    mr r3, r29
    rlwinm r7, r25, 0, 16, 31
    li r4, 0x0
    li r5, 0xf1
    li r6, 0x0
    bl fn_801254B4
    b @L_80220464
@L_8022042C:
    lis r4, 0x1
    mr r3, r29
    subi r7, r4, 0x1
    li r4, 0x0
    li r5, 0xf0
    li r6, 0x0
    bl fn_801254B4
    lis r4, 0x1
    mr r3, r29
    subi r7, r4, 0x1
    li r4, 0x0
    li r5, 0xf1
    li r6, 0x0
    bl fn_801254B4
@L_80220464:
    mr r3, r28
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802204A0
    lhz r4, 0x8(r1)
    mr r3, r29
    bl fn_801F0134
    mr r0, r3
    mr r3, r28
    rlwinm r7, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0xf2
    li r6, 0x0
    bl fn_801254B4
@L_802204A0:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 6, 6
    beq @L_80220510
    rlwinm r0, r19, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80220510
    rlwinm r7, r23, 0, 16, 31
    cmplwi r7, 0xffff
    bne @L_802204DC
    mr r3, r28
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    bl fn_801254B4
    b @L_8022052C
@L_802204DC:
    mr r3, r28
    rlwinm r7, r25, 0, 16, 31
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    bl fn_801254B4
    mr r3, r28
    rlwinm r7, r26, 0, 16, 31
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    bl fn_801254B4
    b @L_8022052C
@L_80220510:
    lis r4, 0x1
    mr r3, r28
    subi r7, r4, 0x1
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    bl fn_801254B4
@L_8022052C:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    mr r3, r29
    bl fn_80206780
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802205B4
    mr r4, r15
    li r3, 0x0
    li r5, 0x11
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802205B4
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 6, 6
    beq @L_802205B4
    cmplw r29, r28
    beq @L_802205B4
    mr r3, r28
    bl fn_80206780
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802205B4
    rlwinm r0, r19, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802205B4
    mr r3, r28
    mr r4, r29
    mr r5, r23
    bl fn_80201340
@L_802205B4:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 12, 12
    bne @L_802206CC
    li r3, 0x0
    li r4, 0x0
    li r5, 0x19
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_802206CC
    mr r3, r29
    li r4, 0x0
    li r5, 0x109
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_802206CC
    rlwinm r0, r21, 0, 24, 31
    cmplwi r0, 0x4
    bne @L_802206CC
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_802206CC
    mr r4, r28
    li r3, 0xe
    bl fn_801F025C
    mr r14, r3
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802206C0
    mr r7, r14
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r22
    bl fn_80209F18
    lwz r0, lbl_8047B618(r13)
    li r7, 0x0
    la r6, lbl_80478D78(r13)
    lis r3, lbl_80379BF4@ha
    rlwinm r0, r0, 0, 26, 24
    stb r7, 0x3(r6)
    addi r4, r3, lbl_80379BF4@l
    lwz r3, lbl_8047B62C(r13)
    stw r0, lbl_8047B618(r13)
    rlwinm r5, r0, 0, 18, 16
    ori r0, r5, 0x200
    stw r5, lbl_8047B618(r13)
    li r5, 0x0
    stb r7, 0x6(r6)
    stw r0, lbl_8047B618(r13)
    stb r7, lbl_8047B627(r13)
    bl fn_80211B94
    lis r3, lbl_80379BFF@ha
    clrlslwi r0, r24, 16, 2
    addi r3, r3, lbl_80379BFF@l
    lwzx r0, r3, r0
    stw r0, lbl_8047B610(r13)
    b @L_80220764
@L_802206C0:
    lwz r0, lbl_8047B618(r13)
    ori r0, r0, 0x200
    stw r0, lbl_8047B618(r13)
@L_802206CC:
    lbz r3, lbl_8047B627(r13)
    addi r0, r3, 0x1
    stb r0, lbl_8047B627(r13)
    b @L_802206F8
    rlwinm. r0, r30, 0, 24, 31
    bne @L_802206F8
    li r3, 0x0
    bl fn_80261B68
    li r3, 0x0
    bl fn_80261E7C
    bl fn_8026246C
@L_802206F8:
    cmplwi r31, 0x1
    bne @L_80220710
    rlwinm. r0, r20, 0, 24, 31
    bne @L_80220710
    li r0, 0x11
    stb r0, lbl_8047B627(r13)
@L_80220710:
    cmplwi r31, 0x2
    bne @L_80220734
    lwz r0, 0xc(r1)
    lbz r3, lbl_8047B627(r13)
    rlwinm r0, r0, 0, 24, 31
    cmplw r0, r3
    bne @L_80220734
    li r0, 0x11
    stb r0, lbl_8047B627(r13)
@L_80220734:
    lbz r3, lbl_8047B627(r13)
    cmplwi r3, 0x11
    beq @L_80220748
    rlwinm. r0, r20, 0, 24, 31
    beq @L_8021FC70
@L_80220748:
    cmplwi r3, 0x11
    bne @L_80220764
    rlwinm. r0, r20, 0, 24, 31
    bne @L_80220764
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x3
    stw r0, lbl_8047B610(r13)
@L_80220764:
    lmw r14, 0x18(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
