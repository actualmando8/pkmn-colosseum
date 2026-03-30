    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    bl fn_801F025C
    mr r25, r3
    bl fn_80205184
    mr r31, r3
    mr r4, r25
    li r3, 0xe
    bl fn_801F025C
    mr r30, r3
    mr r4, r25
    li r3, 0x2
    bl fn_801F025C
    mr r0, r3
    li r3, 0x0
    mr r29, r0
    li r4, 0x0
    li r5, 0x16
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r0, r31, 0, 16, 31
    rlwinm r31, r3, 0, 16, 31
    cmplwi r0, 0xd7
    bne @L_802186B0
    li r0, 0x0
    la r28, lbl_80478D78(r13)
    stb r0, 0x5(r28)
    mr r3, r25
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2b
    beq @L_8021855C
    mr r3, r25
    bl fn_80205B8C
    bl fn_801252E0
    mr r3, r25
    li r4, 0x0
    bl fn_80202998
    mr r3, r25
    li r4, 0x17
    bl fn_80202810
    mr r3, r25
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_80218568
    mr r3, r25
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
    b @L_80218568
@L_8021855C:
    lbz r0, 0x5(r28)
    ori r0, r0, 0x1
    stb r0, 0x5(r28)
@L_80218568:
    cmplwi r30, 0x0
    beq @L_8021860C
    mr r3, r30
    bl fn_80206780
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021860C
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    bl fn_80207BF4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2b
    beq @L_80218600
    mr r3, r30
    bl fn_80205B8C
    bl fn_801252E0
    mr r3, r30
    li r4, 0x0
    bl fn_80202998
    mr r3, r30
    li r4, 0x17
    bl fn_80202810
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021860C
    mr r3, r30
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
    b @L_8021860C
@L_80218600:
    lbz r0, 0x5(r28)
    ori r0, r0, 0x2
    stb r0, 0x5(r28)
@L_8021860C:
    li r26, 0x0
    b @L_802186A4
@L_80218614:
    mr r3, r29
    rlwinm r4, r26, 0, 16, 31
    bl fn_801F7258
    mr. r30, r3
    beq @L_802186A0
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    li r27, 0x0
    mr r24, r3
@L_80218640:
    mr r3, r24
    rlwinm r5, r27, 0, 16, 31
    li r4, 0x3
    bl fn_8012A5B0
    mr r4, r3
    mr r3, r30
    bl fn_801F9930
    mr. r25, r3
    beq @L_80218694
    bl fn_80206A04
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80218694
    mr r3, r25
    bl fn_80205BE8
    mr r28, r3
    bl fn_801248C4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2b
    beq @L_80218694
    mr r3, r28
    bl fn_801252E0
@L_80218694:
    addi r27, r27, 0x1
    cmplwi r27, 0x6
    blt @L_80218640
@L_802186A0:
    addi r26, r26, 0x1
@L_802186A4:
    cmplw r26, r31
    blt @L_80218614
    b @L_80218804
@L_802186B0:
    li r0, 0x4
    la r3, lbl_80478D78(r13)
    stb r0, 0x5(r3)
    mr r3, r25
    bl fn_80205B8C
    bl fn_801252E0
    mr r3, r25
    li r4, 0x17
    bl fn_80202810
    mr r3, r25
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802186FC
    mr r3, r25
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_802186FC:
    cmplwi r30, 0x0
    beq @L_8021877C
    mr r3, r30
    bl fn_80206780
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021877C
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    bl fn_80205B8C
    bl fn_801252E0
    mr r3, r30
    li r4, 0x0
    bl fn_80202998
    mr r3, r30
    li r4, 0x17
    bl fn_80202810
    mr r3, r30
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021877C
    mr r3, r30
    li r4, 0x7c
    li r5, 0x0
    li r6, 0x0
    bl fn_801FE7EC
@L_8021877C:
    li r28, 0x0
    b @L_802187FC
@L_80218784:
    mr r3, r29
    rlwinm r4, r28, 0, 16, 31
    bl fn_801F7258
    mr. r30, r3
    beq @L_802187F8
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    li r26, 0x0
    mr r27, r3
@L_802187B0:
    mr r3, r27
    rlwinm r5, r26, 0, 16, 31
    li r4, 0x3
    bl fn_8012A5B0
    mr r4, r3
    mr r3, r30
    bl fn_801F9930
    mr. r25, r3
    beq @L_802187EC
    bl fn_80206A04
    rlwinm. r0, r3, 0, 24, 31
    beq @L_802187EC
    mr r3, r25
    bl fn_80205BE8
    bl fn_801252E0
@L_802187EC:
    addi r26, r26, 0x1
    cmplwi r26, 0x6
    blt @L_802187B0
@L_802187F8:
    addi r28, r28, 0x1
@L_802187FC:
    cmplw r28, r31
    blt @L_80218784
@L_80218804:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x1
    stw r0, lbl_8047B610(r13)
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
