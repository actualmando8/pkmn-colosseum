    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205B8C
    mr r0, r3
    li r3, 0x12
    mr r29, r0
    li r4, 0x0
    bl fn_801F025C
    mr r30, r3
    bl fn_801EF634
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80229EA8
    li r0, 0x2
    stb r0, lbl_8047B614(r13)
    b @L_8022A4F0
@L_80229EA8:
    mr r3, r31
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80229EDC
    lwz r4, lbl_8047B618(r13)
    rlwinm. r0, r4, 0, 22, 22
    bne @L_80229EDC
    lis r3, lbl_80375FBF@ha
    oris r4, r4, 0x8
    addi r0, r3, lbl_80375FBF@l
    stw r4, lbl_8047B618(r13)
    stw r0, lbl_8047B610(r13)
    b @L_8022A4F0
@L_80229EDC:
    bl fn_8022F2F8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022A4F0
    mr r3, r31
    bl fn_80207BF4
    mr r3, r31
    bl fn_80205184
    mr r26, r3
    mr r3, r30
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    li r25, 0x0
    cmpwi r0, 0x2b
    beq @L_80229F18
    b @L_80229F7C
@L_80229F18:
    mr r3, r30
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80229F7C
    mr r4, r26
    li r3, 0x0
    li r5, 0x17
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229F7C
    mr r3, r31
    li r4, 0x22
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229F6C
    lwz r0, lbl_8047B618(r13)
    ori r0, r0, 0x800
    stw r0, lbl_8047B618(r13)
@L_80229F6C:
    lis r3, lbl_8037989E@ha
    li r25, 0x1
    addi r0, r3, lbl_8037989E@l
    stw r0, lbl_8047B610(r13)
@L_80229F7C:
    rlwinm. r0, r25, 0, 24, 31
    bne @L_8022A4F0
    mr r3, r31
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r28, r0
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r27, r0
    li r5, 0xe
    mr r4, r27
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r26, r3, 0, 24, 31
    mr r4, r27
    li r3, 0x0
    li r5, 0xf
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r25, r3, 0, 24, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_8011BEB4
    extsb. r6, r3
    blt @L_8022A010
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_8022A010
    cmplwi r0, 0x164
    bne @L_8022A018
@L_8022A010:
    li r0, 0x0
    b @L_8022A02C
@L_8022A018:
    mr r3, r29
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
@L_8022A02C:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_8022A0B4
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_8022A0B4
    cmplwi r0, 0x164
    beq @L_8022A0B4
    mr r3, r31
    li r4, 0x22
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022A0B4
    lis r3, 0x80
    lwz r4, lbl_8047B618(r13)
    addi r0, r3, 0x200
    and. r0, r4, r0
    bne @L_8022A0B4
    mr r3, r28
    li r4, 0x40
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022A098
    mr r3, r28
    li r4, 0x40
    li r5, 0x0
    bl fn_802097C8
@L_8022A098:
    lwz r4, lbl_8047B618(r13)
    lis r3, lbl_80379021@ha
    addi r0, r3, lbl_80379021@l
    oris r3, r4, 0x8
    stw r0, lbl_8047B610(r13)
    stw r3, lbl_8047B618(r13)
    b @L_8022A4F0
@L_8022A0B4:
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 6, 6
    rlwinm r3, r3, 0, 9, 7
    stw r3, lbl_8047B618(r13)
    bne @L_8022A130
    mr r3, r31
    li r4, 0x22
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022A130
    mr r3, r31
    bl fn_8022A6C8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8022A130
    cmpwi r0, 0x2
    bne @L_8022A104
    lwz r0, lbl_8047B618(r13)
    oris r0, r0, 0x200
    stw r0, lbl_8047B618(r13)
    b @L_8022A4F0
@L_8022A104:
    mr r3, r28
    li r4, 0x40
    bl fn_8020981C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8022A4F0
    mr r3, r28
    li r4, 0x40
    li r5, 0x0
    bl fn_802097C8
    b @L_8022A4F0
@L_8022A130:
    lwz r0, lbl_8047B618(r13)
    mr r3, r30
    li r4, 0x37
    oris r0, r0, 0x200
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022A2C8
    cmplwi r25, 0x1
    bne @L_8022A2C8
    cmplwi r31, 0x0
    beq @L_8022A254
    cmplwi r30, 0x0
    beq @L_8022A254
    mr r3, r31
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2e
    bne @L_8022A254
    mr r3, r30
    bl fn_80205B8C
    li r4, 0x115
    mr r26, r3
    bl fn_80123B5C
    mr r25, r3
    extsb. r0, r25
    blt @L_8022A254
    extsb r29, r25
    mr r3, r26
    mr r6, r29
    li r4, 0x0
    li r5, 0x80
    bl fn_8012640C
    rlwinm. r4, r3, 0, 24, 31
    beq @L_8022A1C8
    subi r0, r4, 0x1
    rlwinm r4, r0, 0, 24, 31
@L_8022A1C8:
    mr r3, r26
    mr r6, r29
    rlwinm r7, r4, 0, 24, 31
    li r4, 0x0
    li r5, 0x80
    bl fn_801254B4
    mr r3, r30
    rlwinm r25, r25, 0, 24, 31
    li r4, 0x10
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022A254
    mr r3, r30
    li r4, 0x31
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022A254
    mr r3, r30
    li r4, 0x31
    bl fn_80201890
    li r0, 0x1
    slw r0, r0, r25
    and. r0, r3, r0
    bne @L_8022A254
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022A254
    mr r3, r30
    mr r5, r25
    li r4, 0x80
    li r6, 0x0
    bl fn_801FE7EC
@L_8022A254:
    mr r3, r30
    li r4, 0x37
    bl fn_80202810
    mr r3, r30
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    lis r4, lbl_8037917B@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_8037917B@l
    mr r25, r0
    li r5, 0x0
    bl fn_80211B94
    cmplwi r28, 0x0
    beq @L_8022A4F0
    cmplwi r25, 0x0
    beq @L_8022A4F0
    mr r3, r25
    mr r4, r28
    bl fn_8020A2B8
    mr r3, r25
    rlwinm r7, r27, 0, 16, 31
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BBD8
    b @L_8022A4F0
@L_8022A2C8:
    li r0, 0x0
    lis r3, -32733
    stw r31, 0x8(r1)
    addi r4, r3, -23292
    addi r5, r1, 0x8
    li r3, 0x0
    stw r0, 0xc(r1)
    li r6, 0x1
    bl fn_801F37B0
    lwz r25, 0xc(r1)
    cmplwi r25, 0x0
    beq @L_8022A36C
    mr r3, r31
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
    mr r3, r25
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr. r28, r3
    beq @L_8022A36C
    cmplwi r29, 0x0
    beq @L_8022A36C
    li r4, 0x0
    li r5, 0x28
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r25, r3, 0, 16, 31
    mr r3, r28
    mr r4, r29
    bl fn_8020A2B8
    mr r3, r28
    mr r7, r25
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BBD8
@L_8022A36C:
    mr r3, r30
    li r4, 0x0
    li r5, 0x114
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x1
    bne @L_8022A3B4
    mr r3, r30
    li r4, 0x0
    li r5, 0x114
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    lis r4, lbl_803796F3@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_803796F3@l
    li r5, 0x0
    bl fn_80211B94
@L_8022A3B4:
    mr r3, r30
    li r4, 0x2b
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022A4E4
    cmplwi r26, 0x1
    bne @L_8022A4E4
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0xae
    bne @L_8022A404
    mr r3, r31
    li r4, 0x7
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022A404
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    b @L_8022A4F0
@L_8022A404:
    mr r4, r27
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x91
    beq @L_8022A44C
    cmplwi r0, 0x27
    beq @L_8022A44C
    cmplwi r0, 0x4b
    beq @L_8022A44C
    cmplwi r0, 0x97
    beq @L_8022A44C
    cmplwi r0, 0x9b
    beq @L_8022A44C
    cmplwi r0, 0x1a
    bne @L_8022A454
@L_8022A44C:
    li r0, 0x1
    b @L_8022A458
@L_8022A454:
    li r0, 0x0
@L_8022A458:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_8022A478
    mr r3, r31
    li r4, 0x22
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022A4E4
@L_8022A478:
    mr r3, r31
    bl fn_80200B10
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    mr r3, r30
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r30
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    li r5, 0x1
    la r4, lbl_80478D78(r13)
    addi r0, r3, 0x1
    stb r5, 0x6(r4)
    stw r0, lbl_8047B610(r13)
    b @L_8022A4F0
@L_8022A4E4:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_8022A4F0:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
