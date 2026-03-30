    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r25, 0x14(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x12
    mr r30, r0
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r31, r0
    mr r4, r31
    bl fn_801F025C
    mr r0, r3
    mr r3, r30
    mr r29, r0
    bl fn_80205184
    mr r0, r3
    mr r3, r30
    mr r28, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    mr r25, r3
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r25
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r26, r3, 0, 16, 31
    mr r3, r30
    mr r4, r31
    mr r5, r29
    mr r6, r28
    mr r7, r27
    mr r8, r26
    bl fn_80232110
    mr r31, r3
    mr r3, r25
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    mr r3, r25
    mullw r27, r31, r0
    li r4, 0x0
    li r5, 0x2c
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 24, 31
    mr r3, r30
    mullw r27, r27, r0
    li r4, 0x24
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227D5C
    cmplwi r26, 0xd
    bne @L_80227D5C
    rlwinm r27, r27, 1, 0, 30
@L_80227D5C:
    mr r3, r30
    li r4, 0x32
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80227D80
    mulli r3, r27, 0xf
    li r0, 0xa
    divw r27, r3, r0
@L_80227D80:
    mr r3, r25
    mr r7, r27
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
