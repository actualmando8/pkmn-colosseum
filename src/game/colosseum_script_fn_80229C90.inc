    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    bl fn_801F025C
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0xe
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r27, r3, 0, 24, 31
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x2b
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229E34
    cmplwi r27, 0x1
    bne @L_80229E34
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r28, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r29, r3
    mr r3, r30
    bl fn_802096E8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80229D90
    mr r3, r29
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    li r7, 0x0
    bl fn_801254B4
    mr r3, r29
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    li r7, 0x9
    bl fn_801254B4
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80229E24
@L_80229D90:
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r4, r3
    li r3, 0x2
    bl fn_801F025C
    mr r30, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r27, r3
    li r3, 0x2
    mr r4, r27
    bl fn_801F025C
    mr r31, r3
    mr r3, r27
    li r4, 0x15
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80229E00
    cmplw r30, r31
    beq @L_80229E00
    lwz r3, lbl_8047B618(r13)
    rlwinm. r0, r3, 0, 7, 7
    bne @L_80229E00
    ori r0, r3, 0x40
    stw r0, lbl_8047B618(r13)
@L_80229E00:
    mr r3, r28
    mr r4, r29
    li r5, 0x0
    bl fn_8022DCB8
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80229E24
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_80229E24:
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x6(r3)
    b @L_80229E40
@L_80229E34:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_80229E40:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
