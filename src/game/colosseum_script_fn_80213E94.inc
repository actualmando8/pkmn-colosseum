    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x64(r1)
    li r5, 0xd
    li r6, 0x0
    stmw r19, 0x2c(r1)
    li r27, 0x0
    li r20, 0x0
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0xf
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r22, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r19, r3
    li r5, 0xe5
    li r6, 0x0
    bl fn_8012640C
    li r4, 0x0
    mr r21, r3
    li r5, 0x20
    li r6, 0x0
    bl fn_80142CF4
    mr r3, r21
    li r4, 0x0
    li r5, 0x1e
    li r6, 0x0
    bl fn_80142CF4
    rlwinm r23, r3, 0, 16, 31
    li r3, 0x12
    li r4, 0x0
    bl fn_801F025C
    mr r26, r3
    bl fn_80205B8C
    li r4, 0x0
    mr r25, r3
    li r5, 0xc3
    li r6, 0x0
    bl fn_8012640C
    rlwinm r24, r3, 0, 16, 31
    mr r3, r25
    li r4, 0x0
    li r5, 0x6e
    li r6, 0x0
    bl fn_8012640C
    rlwinm r30, r3, 0, 16, 31
    mr r3, r26
    bl fn_80203E0C
    rlwinm r21, r3, 0, 24, 31
    mr r3, r25
    li r4, 0x0
    li r5, 0x83
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    mr r3, r25
    li r4, 0x0
    li r5, 0x87
    li r6, 0x0
    bl fn_8012640C
    mr r29, r3
    mr r4, r19
    li r3, 0x0
    bl fn_801F4354
    mr r0, r3
    mr r3, r25
    mr r19, r0
    bl fn_8011FC74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80213FE0
    mr r3, r24
    bl fn_801EEE44
    mr r30, r3
    b @L_80213FF8
@L_80213FE0:
    mr r4, r30
    li r3, 0x0
    li r5, 0x12
    li r6, 0x0
    bl fn_8012640C
    rlwinm r30, r3, 0, 24, 31
@L_80213FF8:
    cmplwi r23, 0xc
    bgt @L_80214164
    lis r3, jumptable_80399FF4@ha
    rlwinm r0, r23, 2, 0, 29
    addi r3, r3, jumptable_80399FF4@l
    lwzx r0, r3, r0
    mtctr r0
    bctr
    li r27, 0x14
    b @L_80214164
    li r27, 0xf
    b @L_80214164
    li r27, 0xa
    b @L_80214164
    mr r3, r26
    li r4, 0xb
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80214060
    mr r3, r26
    li r4, 0x6
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214068
@L_80214060:
    li r27, 0x1e
    b @L_80214164
@L_80214068:
    li r27, 0xa
    b @L_80214164
    subi r0, r22, 0x3
    rlwinm r0, r0, 0, 16, 31
    cmplwi r0, 0x1
    ble @L_80214088
    cmplwi r22, 0x5
    bne @L_80214090
@L_80214088:
    li r27, 0x23
    b @L_80214164
@L_80214090:
    li r27, 0xa
    b @L_80214164
    cmplwi r21, 0x28
    bge @L_802140B8
    subfic r0, r21, 0x28
    rlwinm r27, r0, 0, 24, 31
    cmplwi r27, 0xa
    bge @L_80214164
    li r27, 0xa
    b @L_80214164
@L_802140B8:
    li r27, 0xa
    b @L_80214164
    mr r3, r25
    bl fn_8011FC74
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802140F0
    mr r3, r24
    bl fn_801EEAD0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214114
    li r20, 0x1
    b @L_80214114
@L_802140F0:
    mr r3, r25
    li r4, 0x0
    li r5, 0x63
    li r6, 0x0
    bl fn_8012640C
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214114
    li r20, 0x1
@L_80214114:
    rlwinm r0, r20, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214128
    li r27, 0x1e
    b @L_80214164
@L_80214128:
    li r27, 0xa
    b @L_80214164
    li r3, 0x0
    li r4, 0x0
    li r5, 0xc
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r3, r3, 0, 16, 31
    addi r0, r3, 0xa
    rlwinm r27, r0, 0, 24, 31
    cmplwi r27, 0x28
    ble @L_80214164
    li r27, 0x28
    b @L_80214164
    li r27, 0xa
@L_80214164:
    cmplwi r23, 0x1
    beq @L_80214174
    cmplwi r31, 0x11
    bne @L_8021417C
@L_80214174:
    li r27, 0x4
    b @L_802142D0
@L_8021417C:
    rlwinm r3, r30, 0, 24, 31
    rlwinm r0, r27, 0, 24, 31
    mullw r6, r3, r0
    li r5, 0xa
    rlwinm r0, r28, 1, 0, 30
    mr r3, r26
    li r4, 0x8
    divw r5, r6, r5
    mulli r6, r29, 0x3
    subf r0, r0, r6
    mullw r0, r5, r0
    divw r20, r0, r6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_802141D4
    mr r3, r26
    li r4, 0x7
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802141D8
@L_802141D4:
    rlwinm r20, r20, 1, 0, 30
@L_802141D8:
    mr r3, r26
    li r4, 0x3
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80214238
    mr r3, r26
    li r4, 0x4
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80214238
    mr r3, r26
    li r4, 0x5
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    beq @L_80214238
    mr r3, r26
    li r4, 0x6
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80214244
@L_80214238:
    mulli r3, r20, 0xf
    li r0, 0xa
    divwu r20, r3, r0
@L_80214244:
    cmplwi r20, 0xff
    blt @L_80214254
    li r27, 0x4
    b @L_802142D0
@L_80214254:
    lis r3, 0xff
    lis r0, 0x4330
    divwu r3, r3, r20
    stw r0, 0x10(r1)
    lfd f1, -20624(r2)
    stw r3, 0x14(r1)
    lfd f0, 0x10(r1)
    fsub f1, f0, f1
    bl fn_800CE77C
    bl fn_800C46B0
    lis r0, 0x4330
    stw r3, 0x1c(r1)
    lfd f1, -20624(r2)
    stw r0, 0x18(r1)
    lfd f0, 0x18(r1)
    fsub f1, f0, f1
    bl fn_800CE77C
    bl fn_800C46B0
    lis r4, 0x10
    li r27, 0x0
    subi r0, r4, 0x10
    divwu r20, r0, r3
    b @L_802142C4
@L_802142B0:
    bl fn_800E0C54
    rlwinm r0, r3, 0, 16, 31
    cmplw r0, r20
    bge @L_802142D0
    addi r27, r27, 0x1
@L_802142C4:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_802142B0
@L_802142D0:
    mr r3, r19
    mr r4, r23
    li r5, 0x0
    bl fn_801FBAD4
    mr r3, r26
    mr r4, r27
    mr r5, r23
    addi r7, r1, 0x8
    li r6, 0x0
    bl fn_802087C0
    mr r3, r19
    mr r4, r23
    li r5, 0x1
    bl fn_801FBAD4
    mr r3, r19
    mr r4, r23
    li r5, 0x3
    bl fn_801FBAD4
    mr r3, r26
    mr r4, r27
    mr r5, r23
    addi r7, r1, 0x8
    li r6, 0x1
    bl fn_802087C0
    mr r3, r26
    bl fn_802037DC
    mr r4, r3
    li r3, 0x16
    bl fn_80132A38
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_802143A8
    mr r3, r19
    mr r4, r23
    li r5, 0x4
    bl fn_801FBAD4
    li r3, 0x5d
    li r4, 0x0
    bl fn_80132A38
    li r3, 0x771f
    bl fn_802624CC
    li r3, 0x3f6
    li r4, 0x0
    li r5, 0xff
    bl fn_80165668
    mr r3, r19
    mr r4, r23
    li r5, 0x5
    bl fn_801FBAD4
    bl fn_8026246C
    lis r3, lbl_80375E51@ha
    addi r0, r3, lbl_80375E51@l
    stw r0, lbl_8047B610(r13)
    b @L_80214404
@L_802143A8:
    mr r3, r26
    mr r4, r27
    mr r5, r23
    addi r7, r1, 0x8
    li r6, 0x2
    bl fn_802087C0
    lis r3, lbl_80279E1C@ha
    la r4, lbl_80478D78(r13)
    clrlslwi r0, r27, 24, 2
    stb r27, 0x5(r4)
    addi r3, r3, lbl_80279E1C@l
    lwzx r3, r3, r0
    bl fn_802624CC
    mr r3, r26
    mr r4, r27
    mr r5, r23
    addi r7, r1, 0x8
    li r6, 0x3
    bl fn_802087C0
    bl fn_8026246C
    lis r3, lbl_80375E5F@ha
    addi r0, r3, lbl_80375E5F@l
    stw r0, lbl_8047B610(r13)
@L_80214404:
    mr r3, r19
    mr r4, r23
    li r5, 0x2
    bl fn_801FBAD4
    mr r3, r19
    mr r4, r23
    li r5, 0x6
    bl fn_801FBAD4
    mr r3, r26
    mr r4, r27
    mr r5, r23
    addi r7, r1, 0x8
    li r6, 0x4
    bl fn_802087C0
    lmw r19, 0x2c(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
