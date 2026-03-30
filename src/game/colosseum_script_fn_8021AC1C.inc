    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x44(r1)
    stmw r21, 0x14(r1)
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r21, r0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r21
    bl fn_801F0134
    mr r28, r3
    mr r3, r21
    bl fn_80205184
    mr r0, r3
    mr r3, r21
    mr r26, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r21
    mr r24, r0
    bl fn_80203E0C
    mr r29, r3
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r27, r3
    bl fn_802040E8
    mr r30, r3
    mr r3, r27
    bl fn_80203FE4
    mr r21, r3
    mr r3, r27
    bl fn_80203EDC
    mr r22, r3
    mr r3, r27
    bl fn_80205B8C
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    rlwinm r25, r3, 0, 16, 31
    mr r3, r27
    bl fn_80207BF4
    mr r23, r3
    mr r3, r27
    bl fn_80203E0C
    mr r31, r3
    mr r7, r27
    li r3, 0x0
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801F4C14
    rlwinm r0, r21, 0, 16, 31
    cmplwi r0, 0x27
    bne @L_8021AD5C
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r0, r0, r4
    cmpw r0, r22
    bge @L_8021AD5C
    mr r3, r27
    li r4, 0x0
    li r5, 0x11a
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
@L_8021AD5C:
    rlwinm r0, r23, 0, 16, 31
    cmplwi r0, 0x5
    bne @L_8021AD90
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    lis r3, lbl_803797A0@ha
    addi r0, r3, lbl_803797A0@l
    stw r0, lbl_8047B610(r13)
    b @L_8021AF98
@L_8021AD90:
    mr r3, r27
    li r4, 0x1d
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021ADE4
    mr r3, r27
    li r4, 0x1d
    bl fn_80201D84
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r28, 0, 16, 31
    cmplw r0, r3
    bne @L_8021ADE4
    rlwinm r3, r29, 0, 24, 31
    rlwinm r0, r31, 0, 24, 31
    cmplw r3, r0
    blt @L_8021ADDC
    li r0, 0x1
    b @L_8021AE50
@L_8021ADDC:
    li r0, 0x0
    b @L_8021AE50
@L_8021ADE4:
    mr r4, r26
    li r3, 0x0
    li r5, 0x6
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r4, r31, 0, 24, 31
    rlwinm r0, r29, 0, 24, 31
    rlwinm r3, r3, 0, 24, 31
    subf r0, r4, r0
    add r0, r0, r3
    rlwinm r21, r0, 0, 16, 31
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x64
    divw r0, r4, r3
    mullw r0, r0, r3
    subf r3, r0, r4
    addi r0, r3, 0x1
    cmpw r0, r21
    bge @L_8021AE4C
    rlwinm r3, r29, 0, 24, 31
    rlwinm r0, r31, 0, 24, 31
    cmplw r3, r0
    blt @L_8021AE4C
    li r0, 0x1
    b @L_8021AE50
@L_8021AE4C:
    li r0, 0x0
@L_8021AE50:
    rlwinm. r0, r0, 0, 16, 31
    beq @L_8021AF48
    mr r3, r27
    li r4, 0x2c
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8021AEA0
    mr r3, r24
    subi r7, r25, 0x1
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x46
    bl fn_801F4C14
    b @L_8021AF38
@L_8021AEA0:
    mr r3, r27
    li r4, 0x0
    li r5, 0x11a
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    beq @L_8021AF08
    mr r3, r24
    subi r7, r25, 0x1
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x47
    bl fn_801F4C14
    rlwinm r7, r30, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
    b @L_8021AF38
@L_8021AF08:
    mr r3, r24
    mr r7, r25
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    bl fn_8011BBD8
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x44
    bl fn_801F4C14
@L_8021AF38:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_8021AF98
@L_8021AF48:
    li r3, 0x0
    li r4, 0x0
    li r5, 0x3b
    li r6, 0x0
    li r7, 0x40
    bl fn_801F4C14
    rlwinm r3, r29, 0, 24, 31
    rlwinm r0, r31, 0, 24, 31
    cmplw r3, r0
    blt @L_8021AF80
    li r0, 0x0
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    b @L_8021AF8C
@L_8021AF80:
    li r0, 0x1
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
@L_8021AF8C:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_8021AF98:
    lmw r21, 0x14(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
