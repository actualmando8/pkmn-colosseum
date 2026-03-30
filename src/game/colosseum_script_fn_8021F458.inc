    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r27, 0xc(r1)
    bl fn_801F54A4
    lwz r5, lbl_8047B610(r13)
    rlwinm r28, r3, 0, 16, 31
    li r4, 0x0
    lbz r3, 0x1(r5)
    lbz r27, 0x2(r5)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    mr r4, r31
    li r3, 0x0
    bl fn_801F4354
    li r4, 0x0
    mr r29, r3
    li r5, 0x47
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r27, 0x2
    mr r30, r3
    beq @L_8021F4E4
    mr r3, r29
    mr r4, r31
    li r5, 0x2
    bl fn_801FBC20
@L_8021F4E4:
    mr r3, r31
    bl fn_80204A10
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021F508
    mr r3, r31
    mr r4, r28
    li r5, 0x0
    bl fn_80265598
    b @L_8021F518
@L_8021F508:
    mr r3, r31
    mr r4, r28
    li r5, 0x1
    bl fn_80265598
@L_8021F518:
    cmplwi r27, 0x2
    beq @L_8021F538
    mr r3, r31
    li r4, 0x1
    bl fn_80208C18
    mr r3, r31
    li r4, 0x2
    bl fn_80208C18
@L_8021F538:
    mr r3, r31
    li r4, 0x3
    bl fn_80208C18
    mr r3, r31
    li r4, 0x4
    bl fn_80208C18
    mr r3, r31
    mr r4, r28
    li r5, 0x0
    bl fn_8026532C
    cmplwi r27, 0x2
    beq @L_8021F578
    mr r3, r29
    mr r4, r31
    li r5, 0x3
    bl fn_801FBC20
@L_8021F578:
    mr r3, r31
    li r4, 0x5
    bl fn_80208C18
    cmplwi r27, 0x1
    bne @L_8021F5F4
    mr r3, r31
    li r4, 0x14
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021F5F4
    mr r3, r31
    bl fn_801FEF74
    cmpwi r3, 0x0
    ble @L_8021F5F4
    bl fn_8026246C
    mr r4, r31
    mr r5, r31
    li r3, 0xa4
    li r6, 0x0
    li r7, 0x0
    bl fn_8020F108
@L_8021F5D0:
    li r3, 0x6
    bl fn_801DA5C4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021F5EC
    bl fn_800F0308
    b @L_8021F5D0
@L_8021F5EC:
    mr r3, r31
    bl fn_802086B0
@L_8021F5F4:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021F63C
    mr r3, r31
    bl fn_80204A10
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021F63C
    mr r3, r31
    li r4, 0x0
    bl fn_80205AD4
    mr r3, r31
    li r4, 0x0
    bl fn_80205A7C
@L_8021F63C:
    mr r3, r30
    bl fn_80206C94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x3
    stw r0, lbl_8047B610(r13)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
