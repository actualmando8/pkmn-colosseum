    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    lwz r0, lbl_8047B618(r13)
    rlwinm. r0, r0, 0, 22, 22
    bne @L_802157E8
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80205184
    mr r3, r30
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r28, r3, 0, 16, 31
    mr r3, r31
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    mullw r0, r29, r28
    rlwinm r3, r3, 0, 16, 31
    divw r0, r0, r3
    rlwinm. r0, r0, 0, 16, 31
    bne @L_802157D0
    li r0, 0x1
@L_802157D0:
    mr r3, r30
    rlwinm r7, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
@L_802157E8:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
