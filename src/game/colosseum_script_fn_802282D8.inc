    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r26, 0x8(r1)
    bl fn_801F54A4
    lwz r0, lbl_8047B618(r13)
    rlwinm r31, r3, 0, 16, 31
    rlwinm. r0, r0, 0, 21, 22
    bne @L_80228484
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r28, r0
    mr r4, r28
    bl fn_801F4354
    mr r0, r3
    mr r3, r28
    mr r29, r0
    bl fn_802037DC
    mr r3, r28
    bl fn_80205184
    mr r0, r3
    mr r3, r28
    mr r27, r0
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r29
    mr r26, r0
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FB1C0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0xee
    li r6, 0x0
    bl fn_8012640C
    mr. r29, r3
    beq @L_802283E8
    li r4, 0x9e
    li r5, 0x4
    li r6, 0x0
    bl fn_801DDD28
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802283E8
    mr r3, r29
    li r4, 0x9e
    li r5, 0x4
    bl fn_801DA9E8
    mr r3, r28
    mr r4, r30
    li r5, 0x1
    bl fn_80265598
@L_802283E8:
    mr r3, r28
    mr r4, r31
    li r5, 0x1
    bl fn_80265598
    mr r3, r28
    mr r4, r27
    bl fn_802623B4
    cmplwi r26, 0x0
    mr r30, r3
    beq @L_80228444
@L_80228410:
    mr r3, r26
    li r4, 0x9e
    li r5, 0x4
    bl fn_801DA94C
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80228430
    bl fn_800F0308
    b @L_80228410
@L_80228430:
    mr r3, r26
    li r4, 0x9e
    li r5, 0x4
    bl fn_801DA8C4
    b @L_80228458
@L_80228444:
    lis r4, lbl_80378964@ha
    lwz r3, lbl_8047B62C(r13)
    addi r4, r4, lbl_80378964@l
    li r5, 0x0
    bl fn_80211B94
@L_80228458:
    rlwinm r0, r30, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80228468
    bl fn_8026246C
@L_80228468:
    mr r3, r28
    mr r4, r31
    li r5, 0x0
    bl fn_8026532C
    lwz r0, lbl_8047B618(r13)
    ori r0, r0, 0x400
    stw r0, lbl_8047B618(r13)
@L_80228484:
    lwz r4, lbl_8047B610(r13)
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    addi r4, r4, 0x1
    stb r0, 0x7(r3)
    stw r4, lbl_8047B610(r13)
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
