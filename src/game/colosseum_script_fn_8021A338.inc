    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    li r4, 0x4
    bl fn_80203B5C
    rlwinm r28, r3, 0, 16, 31
    cmpw r29, r28
    bgt @L_8021A3CC
@L_8021A3A4:
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021A458
@L_8021A3CC:
    mr r3, r31
    li r4, 0x14
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_8021A3A4
    mr r3, r30
    mr r7, r28
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r31
    li r4, 0x14
    li r5, 0x0
    bl fn_8020248C
    mr r3, r31
    mr r5, r28
    li r4, 0x14
    bl fn_80201764
    mr r3, r31
    li r4, 0xe
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021A440
    mr r3, r31
    li r4, 0xe
    bl fn_80202810
@L_8021A440:
    lwz r0, lbl_8047B618(r13)
    li r4, 0x0
    la r3, lbl_80478D78(r13)
    ori r0, r0, 0x100
    stb r4, 0x5(r3)
    stw r0, lbl_8047B618(r13)
@L_8021A458:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
