    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r5, 0xd9
    li r6, 0x0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    mr r29, r4
    mr r28, r3
    li r4, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r28
    mr r24, r0
    bl fn_802040E8
    mr r25, r3
    mr r3, r28
    bl fn_80203FE4
    mr r27, r3
    mr r3, r28
    bl fn_80203EDC
    mr r31, r3
    mr r3, r29
    bl fn_802040E8
    mr r3, r29
    bl fn_80203FE4
    mr r3, r29
    bl fn_80203EDC
    mr r3, r24
    li r30, 0x0
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BEB4
    cmpwi r3, 0x0
    bne @L_8022BC18
    li r3, 0x0
    b @L_8022BE18
@L_8022BC18:
    mr r3, r28
    bl fn_80205184
    mr r0, r3
    mr r3, r24
    mr r21, r0
    bl fn_802096E8
    mr r26, r3
    mr r4, r21
    li r3, 0x0
    li r5, 0x12
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r23, r3, 0, 24, 31
    mr r3, r29
    li r4, 0x0
    li r5, 0x11c
    li r6, 0x0
    bl fn_8012640C
    extsh r22, r3
    mr r3, r29
    li r4, 0x0
    li r5, 0x11e
    li r6, 0x0
    bl fn_8012640C
    extsh r21, r3
    mr r3, r29
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r27, 0, 16, 31
    mr r27, r3
    cmpwi r0, 0x3e
    beq @L_8022BD2C
    bge @L_8022BE14
    cmpwi r0, 0x1e
    beq @L_8022BCB0
    b @L_8022BE14
@L_8022BCB0:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022BE14
    extsh. r0, r22
    bne @L_8022BCCC
    extsh. r0, r21
    beq @L_8022BE14
@L_8022BCCC:
    cmplwi r23, 0x1
    bne @L_8022BE14
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    cmpw r0, r31
    bge @L_8022BE14
    mr r3, r29
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022BE14
    li r0, 0x8
    la r3, lbl_80478D78(r13)
    stb r0, 0x3(r3)
    li r3, 0x0
    lwz r21, lbl_8047B610(r13)
    li r4, 0x0
    bl fn_802249B8
    stw r21, lbl_8047B610(r13)
    b @L_8022BE14
@L_8022BD2C:
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022BE14
    cmpwi r27, 0x0
    beq @L_8022BE14
    addis r0, r27, 0x0
    cmplwi r0, 0xffff
    beq @L_8022BE14
    cmplw r28, r29
    beq @L_8022BE14
    mr r3, r28
    bl fn_80201704
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8022BE14
    mr r3, r28
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8022BE14
    rlwinm r7, r25, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r28
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    divw r0, r27, r31
    neg. r7, r0
    bne @L_8022BDD0
    li r7, -0x1
@L_8022BDD0:
    mr r3, r24
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r29
    li r4, 0x0
    li r5, 0x11b
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    lis r4, lbl_80379B61@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80379B61@l
    li r5, 0x0
    bl fn_80211B94
    li r30, 0x1
@L_8022BE14:
    mr r3, r30
@L_8022BE18:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
