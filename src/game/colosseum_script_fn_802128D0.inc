    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r31, r3
    mr r24, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r4, r24
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r25, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r28, r0
    mr r4, r28
    bl fn_801F025C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r27, r0
    mr r4, r27
    bl fn_801F025C
    mr r0, r3
    mr r3, r28
    mr r26, r0
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80212D58
    mr r3, r28
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    bl fn_801F1170
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80212D58
    li r3, 0x0
    li r4, 0x0
    li r5, 0x51
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    li r3, 0x0
    li r4, 0x0
    li r5, 0x52
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    li r3, 0x0
    li r4, 0x0
    li r5, 0x53
    li r6, 0x0
    li r7, 0x0
    bl fn_801F4C14
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x6(r3)
    mr r3, r28
    li r4, 0x2a
    stw r0, lbl_8047B644(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212B00
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_80212B00
    mr r3, r28
    li r4, 0x2a
    bl fn_80201C58
    mr r26, r3
    mr r3, r28
    li r4, 0x2a
    bl fn_80201890
    mr r24, r3
    mr r3, r28
    bl fn_80205B8C
    rlwinm r6, r24, 0, 16, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r27, r3, 0, 16, 31
    rlwinm r0, r26, 0, 16, 31
    cmplw r0, r27
    beq @L_80212A64
    mr r3, r28
    li r4, 0x2a
    bl fn_80202810
@L_80212A64:
    mr r3, r28
    mr r4, r27
    mr r5, r29
    li r6, 0x0
    li r7, 0x1
    li r8, 0x1
    li r9, -0x1
    bl fn_8022B2CC
    mr r4, r29
    mr r26, r3
    bl fn_801F0134
    li r0, 0x1
    lis r4, lbl_80375CA8@ha
    stw r0, 0x8(r1)
    mr r9, r3
    addi r7, r4, lbl_80375CA8@l
    mr r3, r28
    mr r8, r27
    extsb r10, r24
    li r4, 0x0
    li r5, 0x13
    li r6, 0x0
    bl fn_80204F6C
    mr r24, r27
    mr r7, r26
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r27, r0
    mr r4, r27
    bl fn_801F025C
    mr r26, r3
@L_80212B00:
    rlwinm r0, r24, 0, 16, 31
    cmplwi r0, 0xa5
    beq @L_80212B14
    cmplwi r0, 0x164
    bne @L_80212B20
@L_80212B14:
    lwz r0, lbl_8047B618(r13)
    ori r0, r0, 0x800
    stw r0, lbl_8047B618(r13)
@L_80212B20:
    mr r4, r24
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x91
    beq @L_80212B68
    cmplwi r0, 0x27
    beq @L_80212B68
    cmplwi r0, 0x4b
    beq @L_80212B68
    cmplwi r0, 0x97
    beq @L_80212B68
    cmplwi r0, 0x9b
    beq @L_80212B68
    cmplwi r0, 0x1a
    bne @L_80212B70
@L_80212B68:
    li r0, 0x1
    b @L_80212B74
@L_80212B70:
    li r0, 0x0
@L_80212B74:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212C0C
    mr r3, r28
    li r4, 0x22
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80212C0C
    cmplwi r25, 0x97
    bne @L_80212C60
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80212C60
    mr r3, r28
    mr r4, r24
    mr r5, r29
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    li r9, -0x1
    bl fn_8022B2CC
    mr. r25, r3
    beq @L_80212C60
    mr r27, r25
    mr r7, r25
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r4, r25
    li r3, 0x2
    bl fn_801F025C
    mr r26, r3
    b @L_80212C60
@L_80212C0C:
    mr r3, r28
    mr r4, r24
    mr r5, r29
    li r6, 0x0
    li r7, 0x0
    li r8, 0x1
    li r9, -0x1
    bl fn_8022B2CC
    mr. r25, r3
    beq @L_80212C60
    mr r27, r25
    mr r7, r25
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r4, r25
    li r3, 0x2
    bl fn_801F025C
    mr r26, r3
@L_80212C60:
    mr r3, r27
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80212CE0
    cmplw r30, r26
    bne @L_80212C94
    mr r4, r28
    li r3, 0xf
    bl fn_801F025C
    mr r27, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80212CBC
@L_80212C94:
    mr r4, r27
    li r3, 0xe
    bl fn_801F025C
    mr. r27, r3
    beq @L_80212D58
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80212CBC
    b @L_80212D58
    b @L_80212D58
@L_80212CBC:
    mr r7, r27
    li r3, 0x0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    mr r4, r27
    li r3, 0x2
    bl fn_801F025C
@L_80212CE0:
    mr r4, r24
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    li r0, 0x0
    rlwinm r25, r3, 0, 16, 31
    stb r0, lbl_8047B614(r13)
    mr r4, r24
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x28
    bl fn_80132A38
    lis r3, lbl_80379BFF@ha
    clrlslwi r0, r25, 16, 2
    addi r4, r3, lbl_80379BFF@l
    mr r3, r31
    lwzx r4, r4, r0
    li r5, 0x1
    bl fn_80211B94
    bl fn_801DA7AC
    li r3, 0x0
    bl fn_80261B68
    li r3, 0x0
    bl fn_80261E7C
    bl fn_8026246C
@L_80212D58:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
