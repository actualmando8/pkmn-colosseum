    nofralloc
    stwu r1, -0x80(r1)
    mflr r0
    stw r0, 0x84(r1)
    stmw r24, 0x60(r1)
    mr r25, r5
    mr r30, r4
    mr r3, r25
    bl fn_8020E204
    bl fn_8020E1A4
    rlwinm r26, r3, 0, 24, 31
    mr r3, r30
    addi r4, r1, 0x14
    bl fn_801FE3F8
    li r3, 0x0
    bl fn_801F18DC
    stb r3, 0x58(r1)
    mr r3, r30
    bl fn_80205B8C
    mr r28, r3
@L_80264DA4:
    mr r3, r30
    li r4, 0x0
    li r5, 0x101
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    addi r3, r1, 0x14
    mr r4, r0
    li r5, 0x1
    bl fn_800117BC
    mr. r31, r3
    bge @L_80264DE4
    li r3, 0x1
    bl fn_80011700
    li r3, 0x0
    b @L_80265028
@L_80264DE4:
    mr r3, r30
    rlwinm r4, r31, 0, 16, 31
    addi r6, r1, 0x10
    li r5, 0x1
    bl fn_801FFEC8
    mr r29, r3
    mr r3, r28
    rlwinm r6, r31, 0, 16, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm. r0, r29, 0, 24, 31
    rlwinm r24, r3, 0, 16, 31
    beq @L_80264E70
    mr r4, r30
    li r3, 0x11
    bl fn_80132A38
    mr r4, r24
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x28
    bl fn_80132A38
    mr r3, r30
    bl fn_802040E8
    mr r0, r3
    li r3, 0x0
    rlwinm r7, r0, 0, 16, 31
    li r4, 0x0
    li r5, 0x56
    li r6, 0x0
    bl fn_801F4C14
@L_80264E70:
    rlwinm r0, r29, 0, 24, 31
    cmplwi r0, 0x6
    bne @L_80264E84
    li r27, 0x7661
    b @L_80264EF4
@L_80264E84:
    cmplwi r0, 0x5
    bne @L_80264EB8
    lhz r4, 0x10(r1)
    li r3, 0x0
    li r5, 0x1
    li r6, 0x0
    bl fn_8011BEB4
    bl fn_800FA280
    mr r4, r3
    li r3, 0x28
    bl fn_80132A38
    li r27, 0x76bb
    b @L_80264EF4
@L_80264EB8:
    cmplwi r0, 0x4
    bne @L_80264EC8
    li r27, 0x7600
    b @L_80264EF4
@L_80264EC8:
    cmplwi r0, 0x3
    bne @L_80264ED8
    li r27, 0x75ff
    b @L_80264EF4
@L_80264ED8:
    cmplwi r0, 0x2
    bne @L_80264EE8
    li r27, 0x75fe
    b @L_80264EF4
@L_80264EE8:
    cmplwi r0, 0x1
    bne @L_80264EF4
    li r27, 0x75fd
@L_80264EF4:
    rlwinm. r0, r29, 0, 24, 31
    beq @L_80264F28
    cmplwi r27, 0x0
    beq @L_80264F14
    mr r3, r27
    li r4, 0x1
    li r5, 0x1
    bl fn_80106394
@L_80264F14:
    li r3, 0x40
    bl fn_801F000C
    li r3, 0x0
    bl fn_80106080
    b @L_80264DA4
@L_80264F28:
    mr r3, r28
    rlwinm r6, r31, 0, 16, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm. r29, r3, 0, 16, 31
    beq @L_80264DA4
    lwz r3, -31272(r13)
    lwz r0, 0x0(r3)
    cmplw r29, r0
    bge @L_80264DA4
    cmplwi r29, 0x165
    beq @L_80264DA4
    li r0, 0x0
    lis r3, -32730
    stb r0, -20904(r13)
    addi r6, r3, 20540
    mr r3, r30
    mr r4, r29
    mr r5, r25
    li r7, 0x1
    li r8, 0x0
    li r9, -0x1
    bl fn_8022B2CC
    mr r0, r3
    li r3, 0x0
    mr r24, r0
    bl fn_801F18DC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80264FD0
    lbz r0, -20904(r13)
    cmplwi r0, 0x0
    bne @L_80264FD0
    cmplwi r26, 0x2
    blt @L_80264FD0
    mr r3, r30
    mr r4, r29
    mr r5, r25
    bl fn_8026503C
    cmplwi r3, 0x0
    beq @L_80264DA4
@L_80264FD0:
    cmplwi r24, 0x0
    beq @L_80264DA4
    mr r3, r24
    mr r4, r25
    bl fn_801F0134
    mr r0, r3
    li r3, 0x1
    mr r24, r0
    bl fn_80011700
    li r0, 0x0
    lis r3, -32713
    stw r0, 0x8(r1)
    addi r7, r3, 23720
    mr r3, r30
    mr r8, r29
    mr r9, r24
    extsb r10, r31
    li r4, 0x0
    li r5, 0x13
    li r6, 0x0
    bl fn_80204F6C
    li r3, 0x1
@L_80265028:
    lmw r24, 0x60(r1)
    lwz r0, 0x84(r1)
    mtlr r0
    addi r1, r1, 0x80
    blr
