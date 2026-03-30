    nofralloc
    stwu r1, -0xed0(r1)
    mflr r0
    stw r0, 0xed4(r1)
    stmw r14, 0xe88(r1)
    mr r16, r3
    mr r17, r4
    mr r18, r5
    mr r19, r6
    mr r15, r7
    mr r20, r8
    mr r14, r9
    mr r21, r10
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F54A4
    mr r0, r3
    li r3, 0x0
    stw r0, 0xe7c(r1)
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F54A4
    lwz r0, lbl_8047B610(r13)
    li r4, 0x8
    addi r6, r13, -31400
    addi r5, r1, 0x8
    mr r23, r3
    stw r0, 0xe80(r1)
    lwz r22, lbl_8047B618(r13)
    mtctr r4
@L_802111F0:
    lbz r0, 0x0(r6)
    addi r6, r6, 0x1
    stb r0, 0x0(r5)
    addi r5, r5, 0x1
    bdnz @L_802111F0
    mr r4, r18
    addi r3, r1, 0x79c
    bl fn_801FCEC4
    mr r4, r19
    addi r3, r1, 0xbc
    bl fn_801FCEC4
    mr r7, r18
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r19
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r18
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    addi r3, r1, 0x10
    mr r24, r0
    mr r4, r24
    bl fn_8020A2B8
    mr r3, r24
    bl fn_80209D90
    mr r3, r24
    mr r5, r17
    li r4, 0x0
    li r6, 0x0
    li r7, 0x0
    bl fn_802099AC
    li r4, 0x0
    li r0, 0x8
    addi r3, r13, -31400
    stw r4, lbl_8047B618(r13)
    mtctr r0
@L_802112A8:
    stb r4, 0x0(r3)
    addi r3, r3, 0x1
    bdnz @L_802112A8
    cmplwi r14, 0x0
    beq @L_802112D8
    mr r12, r14
    mr r3, r16
    mr r4, r17
    mr r5, r18
    mr r6, r19
    mtctr r12
    bctrl
@L_802112D8:
    clrlwi r0, r15, 24
    cmplwi r0, 0x1
    bne @L_8021158C
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r25, r3
    bl fn_80207BF4
    mr r3, r25
    bl fn_80203FE4
    mr r29, r3
    mr r3, r25
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    mr r3, r25
    bl fn_80205184
    mr r28, r3
    mr r3, r25
    bl fn_80203D3C
    mr r14, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    clrlwi r0, r29, 16
    li r30, 0x0
    cmplwi r0, 0x3f
    mr r26, r3
    bne @L_80211364
    clrlwi r0, r14, 16
    cmplwi r0, 0x71
    bne @L_80211364
    li r30, 0x1
@L_80211364:
    clrlwi r0, r29, 16
    li r31, 0x0
    cmplwi r0, 0x42
    bne @L_80211384
    clrlwi r0, r14, 16
    cmplwi r0, 0x53
    bne @L_80211384
    li r31, 0x1
@L_80211384:
    mr r3, r25
    li r4, 0xf
    bl fn_802026E4
    clrlwi r0, r3, 24
    mr r4, r28
    subfic r0, r0, 0x1
    li r3, 0x0
    cntlzw r0, r0
    li r5, 0x9
    li r6, 0x0
    rlwinm r15, r0, 28, 4, 30
    bl fn_8011BEB4
    clrlwi r0, r3, 16
    mr r4, r28
    subfic r0, r0, 0x2b
    li r3, 0x0
    cntlzw r0, r0
    li r5, 0x9
    srwi r14, r0, 5
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r0, r3, 16
    mr r4, r28
    subfic r0, r0, 0x4b
    li r3, 0x0
    cntlzw r0, r0
    li r5, 0x9
    srwi r0, r0, 5
    li r6, 0x0
    add r15, r15, r0
    bl fn_8011BEB4
    clrlwi r0, r3, 16
    mr r4, r28
    subfic r0, r0, 0xc8
    li r3, 0x0
    cntlzw r0, r0
    li r5, 0x9
    srwi r0, r0, 5
    li r6, 0x0
    add r15, r15, r0
    bl fn_8011BEB4
    clrlwi r0, r3, 16
    lwz r3, lbl_80478D60(r13)
    subfic r4, r0, 0xd1
    clrlwi r0, r29, 16
    cntlzw r6, r4
    slwi r4, r30, 1
    subfic r5, r0, 0x29
    slwi r0, r31, 1
    srwi r6, r6, 5
    subi r7, r3, 0x1
    cntlzw r3, r5
    add r5, r15, r6
    srwi r3, r3, 5
    add r3, r5, r3
    add r3, r3, r4
    add r0, r3, r0
    add r0, r14, r0
    clrlwi r14, r0, 16
    cmplw r14, r7
    ble @L_8021147C
    clrlwi r14, r7, 16
@L_8021147C:
    mr r3, r26
    bl fn_80207BF4
    clrlwi r0, r3, 16
    cmplwi r0, 0x4
    beq @L_80211568
    mr r3, r26
    bl fn_80207BF4
    clrlwi r0, r3, 16
    cmplwi r0, 0x4b
    beq @L_80211568
    li r3, 0x0
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80211568
    mr r3, r14
    bl fn_8020A080
    bl fn_8020A068
    clrlwi r14, r3, 24
    bl fn_800E0C54
    clrlwi r3, r3, 16
    divw r0, r3, r14
    mullw r0, r0, r14
    subf. r0, r0, r3
    beq @L_80211530
    mr r3, r25
    li r4, 0x3e
    bl fn_802026E4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8021154C
    clrlwi r0, r28, 16
    cmplwi r0, 0x164
    bne @L_8021154C
    bl fn_800E0C54
    clrlwi r4, r3, 16
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    cmpwi r0, 0x5a
    bge @L_8021154C
@L_80211530:
    mr r3, r27
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x2
    bl fn_8011BBD8
    b @L_80211580
@L_8021154C:
    mr r3, r27
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x1
    bl fn_8011BBD8
    b @L_80211580
@L_80211568:
    mr r3, r27
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x1
    bl fn_8011BBD8
@L_80211580:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_8021158C:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r15, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r29, r3
    li r3, 0x2
    mr r4, r29
    bl fn_801F025C
    mr r28, r3
    mr r3, r15
    bl fn_80205184
    mr r27, r3
    mr r3, r15
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    mr r14, r3
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r25, r3, 16
    mr r3, r14
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r26, r3, 16
    mr r3, r15
    mr r4, r29
    mr r5, r28
    mr r6, r27
    mr r7, r25
    mr r8, r26
    bl fn_80232110
    mr r25, r3
    mr r3, r14
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r0, r3, 24
    mr r3, r14
    mullw r25, r25, r0
    li r4, 0x0
    li r5, 0x2c
    li r6, 0x0
    bl fn_8011BEB4
    clrlwi r0, r3, 24
    mr r3, r15
    mullw r25, r25, r0
    li r4, 0x24
    bl fn_802026E4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80211688
    cmplwi r26, 0xd
    bne @L_80211688
    slwi r25, r25, 1
@L_80211688:
    mr r3, r15
    li r4, 0x32
    bl fn_802026E4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802116AC
    mulli r3, r25, 0xf
    li r0, 0xa
    divw r25, r3, r0
@L_802116AC:
    mr r3, r14
    mr r7, r25
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lwz r6, lbl_8047B610(r13)
    li r3, 0x1
    li r4, 0x1
    li r5, 0x1
    addi r0, r6, 0x1
    li r6, 0x0
    stw r0, lbl_8047B610(r13)
    bl fn_802274F0
    clrlwi r0, r20, 24
    cmplwi r0, 0x1
    bne @L_80211708
    li r3, 0x1
    li r4, 0x1
    bl fn_802271E0
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_80211708:
    cmplwi r21, 0x0
    beq @L_8021172C
    mr r12, r21
    mr r3, r16
    mr r4, r17
    mr r5, r18
    mr r6, r19
    mtctr r12
    bctrl
@L_8021172C:
    mr r3, r24
    bl fn_802096E8
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80211758
    mr r3, r24
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BEB4
    b @L_8021175C
@L_80211758:
    li r3, 0x0
@L_8021175C:
    lwz r7, 0xe7c(r1)
    mr r14, r3
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r23
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r24
    addi r4, r1, 0x10
    bl fn_8020A2B8
    lwz r0, 0xe80(r1)
    li r3, 0x8
    addi r4, r13, -31400
    addi r5, r1, 0x8
    stw r0, lbl_8047B610(r13)
    stw r22, lbl_8047B618(r13)
    mtctr r3
@L_802117B8:
    lbz r0, 0x0(r5)
    addi r5, r5, 0x1
    stb r0, 0x0(r4)
    addi r4, r4, 0x1
    bdnz @L_802117B8
    mr r3, r18
    addi r4, r1, 0x79c
    bl fn_801FCEC4
    mr r3, r19
    addi r4, r1, 0xbc
    bl fn_801FCEC4
    mr r3, r14
    lmw r14, 0xe88(r1)
    lwz r0, 0xed4(r1)
    mtlr r0
    addi r1, r1, 0xed0
    blr
