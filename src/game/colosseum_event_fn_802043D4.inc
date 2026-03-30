    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr. r31, r3
    mr r24, r4
    mr r26, r5
    mr r23, r6
    mr r27, r7
    bne @L_80204404
    li r29, 0x0
    b @L_80204438
@L_80204404:
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    cmplwi r3, 0x0
    bne @L_80204424
    li r3, 0x0
    b @L_80204434
@L_80204424:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_80204434:
    mr r29, r3
@L_80204438:
    cmplwi r29, 0x0
    bne @L_80204448
    li r3, 0x0
    b @L_80204840
@L_80204448:
    mr r3, r31
    li r4, 0x0
    li r5, 0x100
    li r6, 0x0
    bl fn_8012640C
    rlwinm r28, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r30, r3
    bne @L_80204484
    li r25, 0x0
    b @L_80204498
@L_80204484:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r25, r3
@L_80204498:
    cmplwi r25, 0x0
    bne @L_802044A8
    li r30, 0x0
    b @L_80204548
@L_802044A8:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_802044D0
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_80204500
@L_802044D0:
    cmplwi r30, 0x0
    bne @L_802044E0
    li r3, 0x0
    b @L_802044F4
@L_802044E0:
    mr r3, r30
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802044F4:
    li r4, 0x3d
    bl fn_80121ADC
    b @L_80204528
@L_80204500:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_8020451C
    li r3, 0x0
    b @L_80204528
@L_8020451C:
    mr r3, r30
    li r4, 0x3d
    bl fn_8011B67C
@L_80204528:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020453C
    li r30, 0x0
    b @L_80204548
@L_8020453C:
    mr r3, r25
    bl fn_80123090
    mr r30, r3
@L_80204548:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr. r22, r3
    bne @L_8020456C
    li r25, 0x0
    b @L_80204580
@L_8020456C:
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
    mr r25, r3
@L_80204580:
    cmplwi r25, 0x0
    bne @L_80204590
    li r25, 0x0
    b @L_80204630
@L_80204590:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_802045B8
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_802045E8
@L_802045B8:
    cmplwi r22, 0x0
    bne @L_802045C8
    li r3, 0x0
    b @L_802045DC
@L_802045C8:
    mr r3, r22
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_802045DC:
    li r4, 0x3d
    bl fn_80121ADC
    b @L_80204610
@L_802045E8:
    li r3, 0x3d
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_80204604
    li r3, 0x0
    b @L_80204610
@L_80204604:
    mr r3, r22
    li r4, 0x3d
    bl fn_8011B67C
@L_80204610:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80204624
    li r25, 0x0
    b @L_80204630
@L_80204624:
    mr r3, r25
    bl fn_80122FF4
    rlwinm r25, r3, 0, 16, 31
@L_80204630:
    mr r3, r31
    li r4, 0x0
    li r5, 0xea
    li r6, 0x0
    bl fn_8012640C
    cmplwi r27, 0x0
    rlwinm r22, r3, 0, 24, 31
    beq @L_80204668
    mr r3, r27
    li r4, 0x11
    li r5, 0x0
    bl fn_8012A5B0
    rlwinm r27, r3, 0, 24, 31
    b @L_8020466C
@L_80204668:
    li r27, 0x0
@L_8020466C:
    mr r3, r29
    li r4, 0x0
    li r5, 0x8c
    li r6, 0x0
    bl fn_8012640C
    cmplwi r28, 0x21
    rlwinm r3, r3, 0, 16, 31
    mr r4, r3
    bne @L_802046A4
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x2
    bne @L_802046A4
    rlwinm r4, r3, 1, 0, 30
    b @L_802046BC
@L_802046A4:
    cmplwi r28, 0x22
    bne @L_802046BC
    rlwinm r0, r26, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802046BC
    rlwinm r4, r4, 1, 0, 30
@L_802046BC:
    mr r3, r22
    bl fn_8020E4E8
    rlwinm r0, r24, 0, 24, 31
    mr r24, r3
    cmplwi r0, 0x1
    bne @L_802046EC
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802046EC
    mulli r3, r24, 0x6e
    li r0, 0x64
    divwu r24, r3, r0
@L_802046EC:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x18
    bne @L_802046FC
    rlwinm r24, r24, 31, 1, 31
@L_802046FC:
    li r3, 0x5
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80204738
    li r3, 0x5
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    beq @L_80204738
    li r3, 0x5
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    bne @L_802047D4
@L_80204738:
    mr r3, r31
    li r4, 0x0
    li r5, 0xd6
    li r6, 0x0
    bl fn_8012640C
    mr r22, r3
    li r3, 0x5
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x7c
    beq @L_80204778
    li r3, 0x5
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xc8
    bne @L_802047A8
@L_80204778:
    cmplwi r22, 0x0
    bne @L_80204788
    li r3, 0x0
    b @L_8020479C
@L_80204788:
    mr r3, r22
    li r4, 0x0
    li r5, 0xcc
    li r6, 0x0
    bl fn_8012640C
@L_8020479C:
    li r4, 0x5
    bl fn_80121ADC
    b @L_802047FC
@L_802047A8:
    li r3, 0x5
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xcd
    beq @L_802047C4
    li r3, 0x0
    b @L_802047FC
@L_802047C4:
    mr r3, r22
    li r4, 0x5
    bl fn_8011B67C
    b @L_802047FC
@L_802047D4:
    li r3, 0x5
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0xd8
    beq @L_802047F0
    li r3, 0x0
    b @L_802047FC
@L_802047F0:
    mr r3, r31
    li r4, 0x5
    bl fn_8011B67C
@L_802047FC:
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020480C
    rlwinm r24, r24, 30, 2, 31
@L_8020480C:
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x1a
    bne @L_8020483C
    lis r3, 0x1
    li r0, 0x64
    subi r3, r3, 0x1
    rlwinm r4, r23, 0, 16, 31
    mullw r3, r25, r3
    divw r0, r3, r0
    cmpw r4, r0
    bge @L_8020483C
    li r24, -0x1
@L_8020483C:
    mr r3, r24
@L_80204840:
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
