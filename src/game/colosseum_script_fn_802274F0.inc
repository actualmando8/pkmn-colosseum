    nofralloc
    stwu r1, -0x100(r1)
    mflr r0
    stw r0, 0x104(r1)
    stmw r15, 0xbc(r1)
    mr r28, r3
    mr r15, r4
    mr r29, r5
    mr r16, r6
    li r31, 0x0
    li r30, 0x0
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r27, r3
    bl fn_80207B8C
    mr r19, r3
    mr r3, r27
    li r4, 0x1
    bl fn_80207B8C
    mr r20, r3
    mr r3, r27
    bl fn_80207BF4
    mr r3, r27
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r25, r0
    li r5, 0x7
    mr r4, r25
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r23, r3, 0, 16, 31
    mr r3, r27
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    mr r22, r3
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r24, r3, 0, 16, 31
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r26, r3
    bl fn_80207B8C
    mr r18, r3
    mr r3, r26
    li r4, 0x1
    bl fn_80207B8C
    mr r17, r3
    mr r3, r26
    bl fn_80207BF4
    cmplwi r22, 0x0
    mr r21, r3
    beq @L_8022760C
    li r0, 0x15
    addi r5, r1, 0x4
    subi r4, r22, 0x4
    mtctr r0
@L_802275F0:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_802275F0
    lwz r0, 0x4(r4)
    stw r0, 0x4(r5)
@L_8022760C:
    rlwinm r0, r25, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_80227910
    cmplwi r0, 0x164
    beq @L_80227910
    rlwinm r0, r16, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227634
    cmplwi r23, 0x0
    beq @L_80227910
@L_80227634:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022768C
    rlwinm r0, r19, 0, 16, 31
    cmplw r0, r24
    beq @L_80227658
    rlwinm r0, r20, 0, 16, 31
    cmplw r0, r24
    bne @L_8022768C
@L_80227658:
    mr r3, r22
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BEB4
    mulli r6, r3, 0xf
    li r0, 0xa
    mr r3, r22
    li r4, 0x0
    divw r7, r6, r0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
@L_8022768C:
    rlwinm r0, r21, 0, 16, 31
    cmplwi r0, 0x1a
    bne @L_8022771C
    cmplwi r24, 0x4
    bne @L_8022771C
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022770C
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x43
    bl fn_801F4C14
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    mr r3, r26
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r26
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
@L_8022770C:
    li r0, 0x4
    la r3, lbl_80478D78(r13)
    stb r0, 0x6(r3)
    b @L_802277A8
@L_8022771C:
    mr r3, r26
    li r4, 0x19
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227754
    rlwinm r0, r18, 0, 16, 31
    cmplwi r0, 0x7
    beq @L_8022774C
    rlwinm r0, r17, 0, 16, 31
    cmplwi r0, 0x7
    bne @L_80227754
@L_8022774C:
    cmplwi r24, 0x1
    ble @L_802277A8
@L_80227754:
    mr r3, r24
    mr r4, r18
    bl fn_8010C74C
    mr r0, r3
    mr r3, r22
    mr r4, r0
    mr r5, r25
    mr r6, r28
    bl fn_802279F8
    rlwinm r3, r18, 0, 16, 31
    rlwinm r0, r17, 0, 16, 31
    cmplw r3, r0
    beq @L_802277A8
    mr r3, r24
    mr r4, r17
    bl fn_8010C74C
    mr r4, r3
    mr r3, r22
    mr r5, r25
    mr r6, r28
    bl fn_802279F8
@L_802277A8:
    rlwinm r0, r21, 0, 16, 31
    cmplwi r0, 0x19
    bne @L_802278E0
    mr r4, r25
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r16, r3, 0, 16, 31
    cmplwi r16, 0x97
    bne @L_802277F4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802277F4
    li r0, 0x2
    b @L_80227844
@L_802277F4:
    cmplwi r16, 0x91
    beq @L_80227824
    cmplwi r16, 0x27
    beq @L_80227824
    cmplwi r16, 0x4b
    beq @L_80227824
    cmplwi r16, 0x97
    beq @L_80227824
    cmplwi r16, 0x9b
    beq @L_80227824
    cmplwi r16, 0x1a
    bne @L_80227840
@L_80227824:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 4, 4
    beq @L_80227838
    li r0, 0x1
    b @L_80227844
@L_80227838:
    li r0, 0x2
    b @L_80227844
@L_80227840:
    li r0, 0x2
@L_80227844:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802278E0
    mr r3, r22
    li r4, 0x41
    bl fn_8020990C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80227878
    mr r3, r22
    bl fn_80209870
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802278E0
@L_80227878:
    cmplwi r23, 0x0
    beq @L_802278E0
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802278D4
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    mr r3, r26
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r26
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
@L_802278D4:
    li r0, 0x3
    la r3, lbl_80478D78(r13)
    stb r0, 0x6(r3)
@L_802278E0:
    mr r3, r22
    li r4, 0x43
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227910
    mr r3, r27
    li r4, 0x0
    li r5, 0x108
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
@L_80227910:
    rlwinm. r0, r28, 0, 24, 31
    bne @L_802279CC
    mr r3, r22
    li r4, 0x40
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227934
    li r31, 0x1
@L_80227934:
    mr r3, r22
    li r4, 0x43
    bl fn_8020990C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227950
    li r30, 0x1
@L_80227950:
    cmplwi r22, 0x0
    beq @L_80227984
    li r0, 0x15
    subi r5, r22, 0x4
    addi r4, r1, 0x4
    mtctr r0
@L_80227968:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r5)
    stwu r0, 0x8(r5)
    bdnz @L_80227968
    lwz r0, 0x4(r4)
    stw r0, 0x4(r5)
@L_80227984:
    rlwinm r0, r31, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802279A8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
@L_802279A8:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802279CC
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x43
    bl fn_801F4C14
@L_802279CC:
    rlwinm r0, r15, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802279E4
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_802279E4:
    lmw r15, 0xbc(r1)
    lwz r0, 0x104(r1)
    mtlr r0
    addi r1, r1, 0x100
    blr
