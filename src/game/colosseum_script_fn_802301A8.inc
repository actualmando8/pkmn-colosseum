    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r28, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802301D0
    li r3, 0x1
    b @L_80230304
@L_802301D0:
    lwz r0, lbl_8047B618(r13)
    mr r3, r28
    li r4, 0x1e
    oris r0, r0, 0x100
    ori r0, r0, 0x20
    stw r0, lbl_8047B618(r13)
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802302EC
    mr r3, r28
    li r4, 0x1e
    bl fn_80202234
    mr r30, r3
    mr r3, r28
    li r4, 0x1e
    bl fn_80202108
    mr r31, r3
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r28
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    extsb r29, r31
    extsb r0, r30
    li r3, 0x2f
    mr r27, r4
    subf r4, r29, r0
    bl fn_80132A38
    mr r3, r29
    extsb r0, r30
    cmpw r3, r0
    blt @L_802302BC
    mr r3, r28
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r27
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r28
    li r4, 0x1e
    bl fn_80202810
    lis r3, lbl_80378E1E@ha
    addi r0, r3, lbl_80378E1E@l
    mr r4, r0
    b @L_802302DC
@L_802302BC:
    addi r0, r29, 0x1
    mr r3, r28
    extsb r5, r0
    li r4, 0x1e
    bl fn_80201FDC
    lis r3, lbl_80378E46@ha
    addi r0, r3, lbl_80378E46@l
    mr r4, r0
@L_802302DC:
    lwz r3, lbl_8047B62C(r13)
    li r5, 0x0
    bl fn_80211B94
    bl fn_8022FE80
@L_802302EC:
    lis r3, 0xff00
    lwz r4, lbl_8047B618(r13)
    subi r0, r3, 0x21
    li r3, 0x1
    and r0, r4, r0
    stw r0, lbl_8047B618(r13)
@L_80230304:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
