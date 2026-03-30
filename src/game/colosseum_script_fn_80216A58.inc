    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r23, 0xc(r1)
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r25, r0
    bl fn_80205184
    mr r27, r3
    mr r4, r31
    li r3, 0x0
    bl fn_801F4354
    li r4, 0x0
    mr r29, r3
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    mr r0, r3
    li r3, 0x12
    mr r28, r0
    li r4, 0x0
    bl fn_801F025C
    mr r30, r3
    bl fn_802062FC
    rlwinm. r0, r3, 0, 24, 31
    bne @L_80216AF0
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80216CE4
@L_80216AF0:
    lbz r26, lbl_80478D78(r13)
    li r23, 0x0
    b @L_80216B6C
@L_80216AFC:
    mr r3, r28
    li r4, 0x3
    bl fn_8012A5B0
    mr r0, r3
    mr r3, r29
    mr r23, r0
    mr r4, r23
    bl fn_801F9930
    mr. r24, r3
    beq @L_80216B60
    bl fn_80206A04
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80216B60
    mr r3, r24
    bl fn_80205BE8
    mr r23, r3
    bl fn_801233F4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80216B60
    mr r3, r23
    bl fn_80122DDC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80216B78
@L_80216B60:
    lbz r3, lbl_80478D78(r13)
    addi r0, r3, 0x1
    stb r0, lbl_80478D78(r13)
@L_80216B6C:
    lbz r5, lbl_80478D78(r13)
    cmplwi r5, 0x6
    blt @L_80216AFC
@L_80216B78:
    lbz r0, lbl_80478D78(r13)
    cmplwi r0, 0x6
    bge @L_80216CC0
    mr r3, r23
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r24, r3, 0, 16, 31
    mr r3, r23
    li r4, 0x0
    li r5, 0x7a
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 24, 31
    mr r3, r30
    bl fn_80203D3C
    mr r0, r3
    mr r3, r30
    mr r28, r0
    bl fn_80203E0C
    mr r3, r23
    li r4, 0x0
    li r5, 0x77
    li r6, 0x0
    bl fn_8012640C
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    mr r4, r24
    li r3, 0x0
    li r5, 0x4
    li r6, 0x0
    bl fn_8012640C
    mr r26, r3
    mr r4, r27
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    clrlslwi r4, r29, 24, 1
    li r0, 0x5
    divw r7, r4, r0
    rlwinm r0, r3, 0, 16, 31
    mr r4, r28
    li r3, 0x0
    li r5, 0x5
    li r6, 0x0
    mullw r26, r26, r0
    addi r0, r7, 0x2
    mullw r26, r26, r0
    bl fn_8012640C
    cmpwi r3, 0x0
    bgt @L_80216C54
    li r3, 0x1
@L_80216C54:
    divw r26, r26, r3
    li r0, 0x32
    mr r3, r31
    li r4, 0x32
    divw r5, r26, r0
    addi r26, r5, 0x2
    mr r23, r26
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80216C8C
    mulli r3, r26, 0xf
    li r0, 0xa
    divw r23, r3, r0
@L_80216C8C:
    mr r3, r25
    mr r7, r23
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    lbz r4, lbl_80478D78(r13)
    lwz r3, lbl_8047B610(r13)
    addi r4, r4, 0x1
    addi r0, r3, 0x9
    stb r4, lbl_80478D78(r13)
    stw r0, lbl_8047B610(r13)
    b @L_80216CE4
@L_80216CC0:
    cmplwi r26, 0x0
    beq @L_80216CD8
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_80216CE4
@L_80216CD8:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x5(r3)
    stw r0, lbl_8047B610(r13)
@L_80216CE4:
    lmw r23, 0xc(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
