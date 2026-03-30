    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    li r28, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80207BF4
    mr r31, r3
    li r3, 0x0
    li r4, 0x1
    bl fn_801F453C
    rlwinm r27, r3, 0, 24, 31
    mr r3, r30
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021A9F4
    li r28, 0x0
    b @L_8021AAE0
@L_8021A9F4:
    cmplwi r27, 0x3
    bne @L_8021AA84
    mr r3, r30
    li r4, 0x5
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021AA80
    mr r3, r30
    li r4, 0x8
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021AA80
    mr r3, r30
    li r4, 0x4
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021AA80
    rlwinm r0, r31, 0, 16, 31
    cmplwi r0, 0x8
    beq @L_8021AA80
    mr r3, r30
    li r4, 0x20
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021AA80
    mr r3, r30
    li r4, 0x21
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021AA80
    mr r3, r30
    li r4, 0x10
    bl fn_80203B5C
    rlwinm r28, r3, 0, 16, 31
    b @L_8021AA84
@L_8021AA80:
    li r28, 0x0
@L_8021AA84:
    cmplwi r27, 0x4
    bne @L_8021AAE0
    mr r3, r30
    li r4, 0xf
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021AADC
    mr r3, r30
    li r4, 0x20
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021AADC
    mr r3, r30
    li r4, 0x21
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021AADC
    mr r3, r30
    li r4, 0x10
    bl fn_80203B5C
    rlwinm r28, r3, 0, 16, 31
    b @L_8021AAE0
@L_8021AADC:
    li r28, 0x0
@L_8021AAE0:
    mr r3, r29
    mr r7, r28
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
