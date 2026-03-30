    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    li r3, 0x12
    mr r28, r0
    li r4, 0x0
    bl fn_801F025C
    mr r30, r3
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x6f
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x75
    li r6, 0x0
    bl fn_8012640C
    lis r5, 0x1
    mr r29, r3
    mr r3, r28
    li r4, 0x0
    subi r7, r5, 0x1
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r30
    li r4, 0x10
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_8021A554
    mr r3, r30
    bl fn_802010C8
    rlwinm. r0, r3, 0, 16, 31
    bne @L_8021A554
    mr r3, r30
    bl fn_80205B8C
    bl fn_8011FC74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021A588
@L_8021A554:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x45
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    li r5, 0x1
    la r4, lbl_80478D78(r13)
    addi r0, r3, 0x1
    stb r5, 0x5(r4)
    stw r0, lbl_8047B610(r13)
    b @L_8021A6B8
@L_8021A588:
    mr r3, r31
    bl fn_801FECD4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021A5A8
    mr r3, r31
    bl fn_801FEC10
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021A554
@L_8021A5A8:
    mr r3, r31
    li r4, 0x10
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021A5CC
    mr r3, r31
    li r4, 0x10
    li r5, 0x0
    bl fn_8020248C
@L_8021A5CC:
    mr r3, r31
    mr r5, r27
    li r4, 0x10
    bl fn_80201764
    rlwinm r0, r29, 0, 24, 31
    mr r3, r31
    extsb r5, r0
    li r4, 0x10
    bl fn_80201EB0
    rlwinm r0, r29, 24, 24, 31
    mr r3, r31
    li r4, 0x10
    extsb r5, r0
    bl fn_80201FDC
    rlwinm r5, r29, 16, 16, 31
    mr r3, r31
    li r4, 0x10
    bl fn_80201B2C
    mr r3, r31
    li r4, 0x29
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021A638
    mr r3, r31
    li r4, 0x29
    bl fn_80202810
@L_8021A638:
    mr r3, r31
    li r4, 0x31
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021A65C
    mr r3, r31
    li r4, 0x31
    bl fn_80202810
@L_8021A65C:
    mr r3, r30
    bl fn_80203758
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    mr r3, r30
    mr r4, r31
    bl fn_801FE91C
    mr r3, r31
    li r4, 0x0
    li r5, 0x101
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    beq @L_8021A6AC
    li r0, 0x0
    stw r0, 0x0(r3)
@L_8021A6AC:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
@L_8021A6B8:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
