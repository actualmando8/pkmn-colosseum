    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80207BF4
    mr r3, r31
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    li r30, 0x0
    cmplwi r0, 0xc9
    bne @L_80214A48
    mr r3, r31
    li r4, 0x38
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80214A80
    mr r3, r31
    li r4, 0x38
    li r5, 0x0
    bl fn_8020248C
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    li r30, 0x1
    b @L_80214A80
@L_80214A48:
    mr r3, r31
    li r4, 0x39
    bl fn_802025B8
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_80214A80
    mr r3, r31
    li r4, 0x39
    li r5, 0x0
    bl fn_8020248C
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    li r30, 0x1
@L_80214A80:
    rlwinm. r0, r30, 0, 24, 31
    beq @L_80214A94
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    b @L_80214A9C
@L_80214A94:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
@L_80214A9C:
    stw r0, lbl_8047B610(r13)
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
