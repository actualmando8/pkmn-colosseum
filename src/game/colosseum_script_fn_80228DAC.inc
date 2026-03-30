    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r28, r3
    mr r27, r4
    mr r31, r5
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    bl fn_801F54A4
    rlwinm r26, r3, 0, 16, 31
    mr r4, r31
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    mr r3, r28
    mr r4, r26
    bl fn_801F0134
    mr r29, r3
    mr r3, r27
    li r4, 0x1d
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228F64
    mr r3, r27
    li r4, 0x1d
    bl fn_80201D84
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r29, 0, 16, 31
    cmplw r3, r0
    bne @L_80228F64
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r27, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r28, r3
    mr r3, r29
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80228EC8
    mr r3, r28
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r28
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80228F5C
@L_80228EC8:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r26, r3
    li r3, 0x2
    mr r4, r26
    bl fn_801F025C
    mr r29, r3
    mr r3, r26
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228F38
    cmplw r30, r29
    beq @L_80228F38
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_80228F38
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_80228F38:
    mr r3, r27
    mr r4, r28
    mr r5, r31
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80228F5C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_80228F5C:
    li r3, 0x1
    b @L_80229668
@L_80228F64:
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 15, 15
    bne @L_802290C0
    mr r3, r27
    li r4, 0x1f
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802290C0
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
    mr r26, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r27, r3
    mr r3, r28
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80229024
    mr r3, r27
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r27
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_802290B8
@L_80229024:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r28, r3
    li r3, 0x2
    mr r4, r28
    bl fn_801F025C
    mr r29, r3
    mr r3, r28
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229094
    cmplw r30, r29
    beq @L_80229094
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_80229094
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_80229094:
    mr r3, r26
    mr r4, r27
    mr r5, r31
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802290B8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_802290B8:
    li r3, 0x1
    b @L_80229668
@L_802290C0:
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 14, 14
    rlwinm r3, r3, 0, 16, 14
    stw r3, lbl_8047B618(r13)
    bne @L_80229224
    mr r3, r27
    li r4, 0x20
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229224
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
    mr r28, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r26, r3
    mr r3, r27
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80229188
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
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8022921C
@L_80229188:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r27, r3
    li r3, 0x2
    mr r4, r27
    bl fn_801F025C
    mr r29, r3
    mr r3, r27
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802291F8
    cmplw r30, r29
    beq @L_802291F8
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_802291F8
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_802291F8:
    mr r3, r28
    mr r4, r26
    mr r5, r31
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022921C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_8022921C:
    li r3, 0x1
    b @L_80229668
@L_80229224:
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 13, 13
    rlwinm r3, r3, 0, 15, 13
    stw r3, lbl_8047B618(r13)
    bne @L_80229388
    mr r3, r27
    li r4, 0x21
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229388
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
    mr r28, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r26, r3
    mr r3, r27
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802292EC
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
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80229380
@L_802292EC:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r27, r3
    li r3, 0x2
    mr r4, r27
    bl fn_801F025C
    mr r29, r3
    mr r3, r27
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022935C
    cmplw r30, r29
    beq @L_8022935C
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_8022935C
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_8022935C:
    mr r3, r28
    mr r4, r26
    mr r5, r31
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80229380
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_80229380:
    li r3, 0x1
    b @L_80229668
@L_80229388:
    lwz r0, lbl_8047B618(r13)
    mr r4, r31
    li r3, 0x0
    li r5, 0x9
    rlwinm r0, r0, 0, 14, 12
    li r6, 0x0
    stw r0, lbl_8047B618(r13)
    bl fn_8011BEB4
    rlwinm r26, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802293D4
    cmplwi r26, 0x98
    bne @L_802293D4
    li r0, 0x1
    b @L_802293D8
@L_802293D4:
    li r0, 0x0
@L_802293D8:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229504
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r28, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r26, r3
    mr r3, r27
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80229468
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
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_802294FC
@L_80229468:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r27, r3
    li r3, 0x2
    mr r4, r27
    bl fn_801F025C
    mr r29, r3
    mr r3, r27
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802294D8
    cmplw r30, r29
    beq @L_802294D8
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_802294D8
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_802294D8:
    mr r3, r28
    mr r4, r26
    mr r5, r31
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802294FC
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_802294FC:
    li r3, 0x1
    b @L_80229668
@L_80229504:
    mr r4, r31
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x11
    beq @L_8022952C
    cmplwi r0, 0x4e
    bne @L_80229534
@L_8022952C:
    li r0, 0x1
    b @L_80229538
@L_80229534:
    li r0, 0x0
@L_80229538:
    rlwinm r0, r0, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229664
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r28, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r26, r3
    mr r3, r27
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802295C8
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
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8022965C
@L_802295C8:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r29, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r27, r3
    li r3, 0x2
    mr r4, r27
    bl fn_801F025C
    mr r30, r3
    mr r3, r27
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229638
    cmplw r29, r30
    beq @L_80229638
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_80229638
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_80229638:
    mr r3, r28
    mr r4, r26
    mr r5, r31
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022965C
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x7
    stw r0, lbl_8047B610(r13)
@L_8022965C:
    li r3, 0x1
    b @L_80229668
@L_80229664:
    li r3, 0x0
@L_80229668:
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
