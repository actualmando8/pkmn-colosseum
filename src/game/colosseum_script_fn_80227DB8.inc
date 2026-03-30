    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    bl fn_801F025C
    mr r26, r3
    bl fn_80207BF4
    mr r3, r26
    bl fn_80203FE4
    mr r27, r3
    mr r3, r26
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r26
    mr r24, r0
    bl fn_80205184
    mr r0, r3
    mr r3, r26
    mr r23, r0
    bl fn_80203D3C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    rlwinm r0, r27, 0, 16, 31
    li r28, 0x0
    cmplwi r0, 0x3f
    mr r25, r3
    bne @L_80227E50
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x71
    bne @L_80227E50
    li r28, 0x1
@L_80227E50:
    rlwinm r0, r27, 0, 16, 31
    li r29, 0x0
    cmplwi r0, 0x42
    bne @L_80227E70
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x53
    bne @L_80227E70
    li r29, 0x1
@L_80227E70:
    mr r3, r26
    li r4, 0xf
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    mr r4, r23
    subfic r0, r0, 0x1
    li r3, 0x0
    cntlzw r0, r0
    li r5, 0x9
    li r6, 0x0
    rlwinm r31, r0, 28, 4, 30
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    mr r4, r23
    subfic r0, r0, 0x2b
    li r3, 0x0
    cntlzw r0, r0
    li r5, 0x9
    rlwinm r30, r0, 27, 5, 31
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    mr r4, r23
    subfic r0, r0, 0x4b
    li r3, 0x0
    cntlzw r0, r0
    li r5, 0x9
    rlwinm r0, r0, 27, 5, 31
    li r6, 0x0
    add r31, r31, r0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    mr r4, r23
    subfic r0, r0, 0xc8
    li r3, 0x0
    cntlzw r0, r0
    li r5, 0x9
    rlwinm r0, r0, 27, 5, 31
    li r6, 0x0
    add r31, r31, r0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    lwz r3, lbl_80478D60(r13)
    subfic r4, r0, 0xd1
    rlwinm r0, r27, 0, 16, 31
    cntlzw r6, r4
    rlwinm r4, r28, 1, 0, 30
    subfic r5, r0, 0x29
    rlwinm r0, r29, 1, 0, 30
    rlwinm r6, r6, 27, 5, 31
    subi r7, r3, 0x1
    cntlzw r3, r5
    add r5, r31, r6
    rlwinm r3, r3, 27, 5, 31
    add r3, r5, r3
    add r3, r3, r4
    add r0, r3, r0
    add r0, r30, r0
    rlwinm r27, r0, 0, 16, 31
    cmplw r27, r7
    ble @L_80227F68
    rlwinm r27, r7, 0, 16, 31
@L_80227F68:
    mr r3, r25
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x4
    beq @L_80228054
    mr r3, r25
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x4b
    beq @L_80228054
    li r3, 0x0
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228054
    mr r3, r27
    bl fn_8020A080
    bl fn_8020A068
    rlwinm r25, r3, 0, 24, 31
    bl fn_800E0C54
    rlwinm r3, r3, 0, 16, 31
    divw r0, r3, r25
    mullw r0, r0, r25
    subf. r0, r0, r3
    beq @L_8022801C
    mr r3, r26
    li r4, 0x3e
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228038
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0x164
    bne @L_80228038
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    cmpwi r0, 0x5a
    bge @L_80228038
@L_8022801C:
    mr r3, r24
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x2
    bl fn_8011BBD8
    b @L_8022806C
@L_80228038:
    mr r3, r24
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x1
    bl fn_8011BBD8
    b @L_8022806C
@L_80228054:
    mr r3, r24
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x1
    bl fn_8011BBD8
@L_8022806C:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
