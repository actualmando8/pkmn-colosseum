    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    lwz r5, lbl_8047B610(r13)
    lwz r27, 0x1(r5)
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
    mr r0, r3
    mr r3, r30
    mr r26, r0
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r25, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r24, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x2d
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021C3A4
    li r29, 0x0
    b @L_8021C3B4
@L_8021C3A4:
    mr r3, r31
    li r4, 0x2d
    bl fn_80202360
    mr r29, r3
@L_8021C3B4:
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r0, r3
    li r3, 0x2
    mr r28, r0
    mr r4, r28
    bl fn_801F025C
    extsh. r0, r29
    mr r5, r3
    bgt @L_8021C3E8
    stw r27, lbl_8047B610(r13)
    b @L_8021C47C
@L_8021C3E8:
    la r3, lbl_80478D78(r13)
    lbz r0, 0x6(r3)
    cmplwi r0, 0x1
    beq @L_8021C464
    mr r3, r31
    mr r4, r28
    mr r6, r26
    mr r7, r25
    mr r8, r24
    bl fn_80232110
    extsh r0, r29
    lis r4, lbl_80379F58@ha
    mullw r28, r0, r3
    mr r3, r31
    addi r4, r4, lbl_80379F58@l
    addis r4, r4, 0x1
    stb r29, 0x6002(r4)
    li r4, 0x32
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021C44C
    mulli r3, r28, 0xf
    li r0, 0xa
    divw r28, r3, r0
@L_8021C44C:
    mr r3, r30
    mr r7, r28
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
@L_8021C464:
    mr r3, r31
    li r4, 0x2d
    bl fn_80202810
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021C47C:
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
