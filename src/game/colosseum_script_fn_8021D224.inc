    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    li r31, 0x1
    bl fn_801F025C
    mr r30, r3
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0xf1
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0xfc
    li r6, 0x0
    bl fn_8012640C
    cmplwi r29, 0xb6
    rlwinm r28, r3, 0, 24, 31
    beq @L_8021D2CC
    cmplwi r29, 0xc5
    beq @L_8021D2CC
    cmplwi r29, 0xcb
    beq @L_8021D2CC
    mr r3, r30
    li r28, 0x0
    li r4, 0x0
    li r5, 0xfc
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
@L_8021D2CC:
    li r3, 0x0
    bl fn_801F221C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021D2E4
    li r31, 0x0
@L_8021D2E4:
    bl fn_800E0C54
    clrlslwi r0, r28, 24, 1
    addi r4, r2, -20648
    lhzx r0, r4, r0
    rlwinm r3, r3, 0, 16, 31
    cmplw r0, r3
    ble @L_8021D3B0
    rlwinm. r0, r31, 0, 24, 31
    beq @L_8021D3B0
    cmplwi r27, 0x6f
    bne @L_8021D344
    mr r3, r30
    li r4, 0x2b
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8021D338
    mr r3, r30
    li r4, 0x2b
    li r5, 0x0
    bl fn_8020248C
@L_8021D338:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021D344:
    cmplwi r27, 0x74
    bne @L_8021D380
    mr r3, r30
    li r4, 0x2c
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8021D374
    mr r3, r30
    li r4, 0x2c
    li r5, 0x0
    bl fn_8020248C
@L_8021D374:
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021D380:
    addi r28, r28, 0x1
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x3
    ble @L_8021D394
    li r28, 0x3
@L_8021D394:
    mr r3, r30
    rlwinm r7, r28, 0, 24, 31
    li r4, 0x0
    li r5, 0xfc
    li r6, 0x0
    bl fn_801254B4
    b @L_8021D3EC
@L_8021D3B0:
    mr r3, r30
    li r4, 0x0
    li r5, 0xfc
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    li r0, 0x2
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
@L_8021D3EC:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
