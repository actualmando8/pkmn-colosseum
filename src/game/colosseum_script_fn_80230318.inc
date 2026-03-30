    nofralloc
    stwu r1, -0xd0(r1)
    mflr r0
    stw r0, 0xd4(r1)
    stmw r27, 0xbc(r1)
    mr r30, r4
    mr r31, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80230344
    li r3, 0x1
    b @L_80230554
@L_80230344:
    lwz r0, lbl_8047B618(r13)
    mr r3, r31
    li r4, 0x34
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023053C
    mr r3, r31
    li r4, 0x34
    bl fn_80202234
    mr r29, r3
    mr r3, r31
    li r4, 0x34
    bl fn_80202108
    extsb r0, r29
    extsb r4, r3
    cmpw r4, r0
    blt @L_80230528
    mr r3, r31
    li r4, 0x34
    bl fn_80201890
    mr r0, r3
    mr r3, r31
    mr r28, r0
    li r4, 0x34
    bl fn_80201C58
    mr r0, r3
    mr r3, r31
    mr r27, r0
    li r4, 0x34
    bl fn_80201D84
    mr r4, r30
    bl fn_801F00D0
    mr r29, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802303F4
    mr r4, r29
    li r3, 0xe
    bl fn_801F025C
    mr r29, r3
@L_802303F4:
    rlwinm r0, r27, 0, 16, 31
    cmplwi r0, 0xf8
    bne @L_80230410
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8023041C
@L_80230410:
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8023041C:
    mr r4, r27
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r7, r31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r31
    mr r4, r30
    bl fn_801F0134
    mr r30, r3
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r29
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    addi r3, r1, 0x8
    mr r29, r0
    mr r4, r29
    bl fn_8020A2B8
    mr r3, r29
    mr r5, r27
    mr r6, r30
    li r4, -0x1
    li r7, 0x0
    bl fn_802099AC
    mr r3, r29
    mr r7, r28
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lis r4, 0x1
    mr r3, r31
    subi r7, r4, 0x1
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    bl fn_801254B4
    mr r3, r31
    li r4, 0x34
    bl fn_80202810
    lis r4, lbl_80378F39@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378F39@l
    li r5, 0x0
    bl fn_80211B94
    mr r3, r29
    addi r4, r1, 0x8
    bl fn_8020A2B8
    bl fn_8022FE80
    b @L_8023053C
@L_80230528:
    addi r0, r3, 0x1
    mr r3, r31
    extsb r5, r0
    li r4, 0x34
    bl fn_80201FDC
@L_8023053C:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    li r3, 0x1
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
@L_80230554:
    lmw r27, 0xbc(r1)
    lwz r0, 0xd4(r1)
    mtlr r0
    addi r1, r1, 0xd0
    blr
