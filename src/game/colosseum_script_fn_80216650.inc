    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x24(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r27, 0xc(r1)
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
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
    mr r4, r27
    li r5, 0x1
    bl fn_802656AC
    mr r0, r3
    li r3, 0x12
    mr r27, r0
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r29, r3
    li r5, 0xe6
    li r6, 0x0
    bl fn_8012640C
    rlwinm r28, r3, 0, 24, 31
    mr r3, r29
    li r4, 0x0
    li r5, 0xe8
    li r6, 0x0
    bl fn_8012640C
    cmplwi r28, 0x0
    rlwinm r0, r3, 0, 24, 31
    bne @L_80216724
    cmplwi r0, 0x0
    bne @L_80216724
    la r3, lbl_80478D78(r13)
    lbz r0, 0x6(r3)
    cmplwi r0, 0x1
    beq @L_80216724
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021676C
@L_80216724:
    mr r3, r31
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r7, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    bl fn_80011E68
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021676C:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
