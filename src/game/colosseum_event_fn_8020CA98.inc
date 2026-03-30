    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r19, 0xc(r1)
    bl fn_8020D920
    bl fn_8020D814
    rlwinm r21, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r28, r3, 0, 16, 31
    mr r3, r21
    mr r5, r29
    li r4, 0x0
    bl fn_801F02AC
    mr r30, r3
    bl fn_801F7404
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020CB24
    li r3, 0x0
    b @L_8020CFCC
@L_8020CB24:
    mr r3, r30
    bl fn_801F7388
    rlwinm r27, r3, 0, 24, 31
    li r23, 0x0
    b @L_8020CC6C
@L_8020CB38:
    mr r3, r30
    mr r4, r23
    bl fn_801F7258
    mr. r25, r3
    beq @L_8020CC68
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    bl fn_801FB1C0
    mr. r21, r3
    beq @L_8020CC68
    mr r3, r25
    bl fn_801F98BC
    li r24, 0x0
    li r22, 0x0
    b @L_8020CC5C
@L_8020CB78:
    rlwinm r0, r24, 0, 16, 31
    cmplw r0, r31
    bge @L_8020CC68
    cmplwi r0, 0x2
    bge @L_8020CC68
    mr r3, r25
    mr r6, r22
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    mr r20, r3
    bl fn_80206608
    rlwinm. r0, r3, 0, 24, 31
    beq @L_8020CC58
    mr r3, r20
    li r4, 0x0
    li r5, 0x0
    bl fn_8010AE2C
    mr r3, r20
    bl fn_80205BE8
    bl fn_80121C18
    mr r0, r3
    mr r3, r25
    mr r19, r0
    mr r6, r22
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r4, r20
    mr r26, r3
    mr r5, r19
    bl fn_802068C8
    mr r3, r26
    addi r24, r24, 0x1
    bl fn_80208028
    li r3, 0x0
    li r4, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020CC4C
    mr r3, r26
    bl fn_80204A10
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020CC4C
    mr r3, r26
    li r4, 0x0
    bl fn_80205AD4
    mr r3, r26
    li r4, 0x0
    bl fn_80205A7C
@L_8020CC4C:
    mr r3, r21
    mr r4, r19
    bl fn_801C3E3C
@L_8020CC58:
    addi r22, r22, 0x1
@L_8020CC5C:
    rlwinm r0, r22, 0, 16, 31
    cmplwi r0, 0x6
    blt @L_8020CB78
@L_8020CC68:
    addi r23, r23, 0x1
@L_8020CC6C:
    rlwinm r0, r23, 0, 16, 31
    cmplw r0, r28
    blt @L_8020CB38
    li r21, 0x0
    b @L_8020CD08
@L_8020CC80:
    mr r3, r30
    mr r4, r21
    bl fn_801F7258
    mr. r22, r3
    beq @L_8020CD04
    li r23, 0x0
    b @L_8020CCB4
@L_8020CC9C:
    mr r3, r22
    mr r4, r23
    bl fn_801F981C
    mr. r26, r3
    bne @L_8020CCC0
    addi r23, r23, 0x1
@L_8020CCB4:
    rlwinm r0, r23, 0, 16, 31
    cmplw r0, r31
    blt @L_8020CC9C
@L_8020CCC0:
    mr r3, r22
    mr r4, r26
    li r5, 0x0
    bl fn_801FBC20
    li r23, 0x0
    b @L_8020CCF8
@L_8020CCD8:
    mr r3, r22
    mr r4, r23
    bl fn_801F981C
    mr. r26, r3
    beq @L_8020CCF4
    li r4, 0x0
    bl fn_80208C18
@L_8020CCF4:
    addi r23, r23, 0x1
@L_8020CCF8:
    rlwinm r0, r23, 0, 16, 31
    cmplw r0, r31
    blt @L_8020CCD8
@L_8020CD04:
    addi r21, r21, 0x1
@L_8020CD08:
    rlwinm r0, r21, 0, 16, 31
    cmplw r0, r28
    blt @L_8020CC80
    li r25, 0x0
    b @L_8020CF1C
@L_8020CD1C:
    mr r3, r30
    mr r4, r25
    bl fn_801F7258
    mr. r24, r3
    beq @L_8020CF18
    bl fn_801F98BC
    rlwinm r19, r3, 0, 24, 31
    li r21, 0x0
    b @L_8020CD78
@L_8020CD40:
    mr r3, r24
    mr r4, r21
    bl fn_801F981C
    mr. r26, r3
    beq @L_8020CD74
    mr r3, r24
    mr r4, r26
    mr r5, r27
    mr r6, r19
    mr r7, r25
    mr r8, r21
    li r9, 0x0
    bl fn_8020CFE0
@L_8020CD74:
    addi r21, r21, 0x1
@L_8020CD78:
    rlwinm r0, r21, 0, 16, 31
    cmplw r0, r31
    blt @L_8020CD40
    li r21, 0x0
    b @L_8020CDA4
@L_8020CD8C:
    mr r3, r24
    mr r4, r21
    bl fn_801F981C
    mr. r26, r3
    bne @L_8020CDB0
    addi r21, r21, 0x1
@L_8020CDA4:
    rlwinm r0, r21, 0, 16, 31
    cmplw r0, r31
    blt @L_8020CD8C
@L_8020CDB0:
    bl fn_801C3430
    mr r3, r24
    mr r4, r26
    li r5, 0x1
    bl fn_801FBC20
    mr r3, r24
    bl fn_801F8000
    mr r4, r3
    li r3, 0x22
    bl fn_80132A38
    mr r3, r24
    bl fn_801F8100
    mr r4, r3
    li r3, 0x23
    bl fn_80132A38
    mr r3, r24
    bl fn_801F8100
    mr r4, r3
    li r3, 0x25
    bl fn_80132A38
    mr r3, r24
    mr r4, r26
    mr r5, r27
    mr r6, r19
    mr r7, r25
    mr r8, r21
    li r9, 0x1
    bl fn_8020CFE0
    mr r3, r24
    mr r4, r26
    li r5, 0x2
    bl fn_801FBC20
    li r21, 0x0
    b @L_8020CF0C
@L_8020CE38:
    mr r3, r24
    mr r4, r21
    bl fn_801F981C
    mr. r26, r3
    beq @L_8020CF08
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F54A4
    mr r20, r3
    mr r7, r26
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r26
    li r4, 0x1
    bl fn_80208C18
    mr r3, r26
    bl fn_80204A10
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020CEAC
    mr r3, r26
    mr r4, r29
    li r5, 0x0
    bl fn_80265598
    b @L_8020CEBC
@L_8020CEAC:
    mr r3, r26
    mr r4, r29
    li r5, 0x1
    bl fn_80265598
@L_8020CEBC:
    mr r3, r26
    li r4, 0x2
    bl fn_80208C18
    mr r3, r26
    li r4, 0x3
    bl fn_80208C18
    mr r3, r26
    li r4, 0x4
    bl fn_80208C18
    mr r3, r26
    mr r4, r29
    li r5, 0x0
    bl fn_8026532C
    mr r7, r20
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
@L_8020CF08:
    addi r21, r21, 0x1
@L_8020CF0C:
    rlwinm r0, r21, 0, 16, 31
    cmplw r0, r31
    blt @L_8020CE38
@L_8020CF18:
    addi r25, r25, 0x1
@L_8020CF1C:
    rlwinm r0, r25, 0, 16, 31
    cmplw r0, r28
    blt @L_8020CD1C
    bl fn_8026246C
    li r21, 0x0
    b @L_8020CFBC
@L_8020CF34:
    mr r3, r30
    mr r4, r21
    bl fn_801F7258
    mr. r22, r3
    beq @L_8020CFB8
    li r23, 0x0
    b @L_8020CF68
@L_8020CF50:
    mr r3, r22
    mr r4, r23
    bl fn_801F981C
    mr. r26, r3
    bne @L_8020CF74
    addi r23, r23, 0x1
@L_8020CF68:
    rlwinm r0, r23, 0, 16, 31
    cmplw r0, r31
    blt @L_8020CF50
@L_8020CF74:
    mr r3, r22
    mr r4, r26
    li r5, 0x3
    bl fn_801FBC20
    li r23, 0x0
    b @L_8020CFAC
@L_8020CF8C:
    mr r3, r22
    mr r4, r23
    bl fn_801F981C
    mr. r26, r3
    beq @L_8020CFA8
    li r4, 0x5
    bl fn_80208C18
@L_8020CFA8:
    addi r23, r23, 0x1
@L_8020CFAC:
    rlwinm r0, r23, 0, 16, 31
    cmplw r0, r31
    blt @L_8020CF8C
@L_8020CFB8:
    addi r21, r21, 0x1
@L_8020CFBC:
    rlwinm r0, r21, 0, 16, 31
    cmplw r0, r28
    blt @L_8020CF34
    li r3, 0x1
@L_8020CFCC:
    lmw r19, 0xc(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
