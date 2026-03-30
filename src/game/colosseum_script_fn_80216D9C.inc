    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x34(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r25, 0x14(r1)
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r31
    mr r31, r3
    bl fn_801F0134
    mr r0, r3
    mr r3, r31
    mr r29, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r30, r0
    bl fn_80205184
    mr r0, r3
    mr r3, r30
    mr r28, r0
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r26, r3, 0, 16, 31
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r30, r0
    mr r4, r30
    bl fn_801F025C
    mr r0, r3
    mr r3, r30
    mr r25, r0
    li r4, 0x34
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    beq @L_80216E90
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80216F3C
@L_80216E90:
    mr r3, r30
    mr r5, r29
    li r4, 0x34
    bl fn_8020248C
    mr r3, r30
    mr r5, r28
    li r4, 0x34
    bl fn_80201B2C
    mr r3, r31
    mr r4, r30
    mr r5, r25
    mr r6, r28
    mr r7, r27
    mr r8, r26
    bl fn_80232110
    mr r0, r3
    mr r3, r31
    mr r26, r0
    li r4, 0x32
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80216EF8
    mulli r3, r26, 0xf
    li r0, 0xa
    divw r26, r3, r0
@L_80216EF8:
    mr r3, r30
    mr r5, r26
    li r4, 0x34
    bl fn_80201764
    rlwinm r0, r28, 0, 16, 31
    cmplwi r0, 0x161
    bne @L_80216F24
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_80216F30
@L_80216F24:
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_80216F30:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_80216F3C:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
