    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    li r27, 0x1
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r0, r3
    mr r3, r31
    mr r28, r0
    bl fn_80205B8C
    mr r0, r3
    li r3, 0x12
    mr r26, r0
    li r4, 0x0
    bl fn_801F025C
    mr r29, r3
    bl fn_80207BF4
    lwz r0, lbl_8047B618(r13)
    mr r30, r3
    andi. r0, r0, 0xa00
    bne @L_802282AC
    mr r3, r31
    bl fn_80205184
    mr r0, r3
    li r3, 0x0
    mr r4, r0
    li r5, 0x5
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r24, r3, 0, 24, 31
    mr r3, r28
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_8011BEB4
    extsb r28, r3
    mr r3, r26
    li r4, 0x0
    li r5, 0x80
    mr r6, r28
    bl fn_8012640C
    rlwinm r25, r3, 0, 24, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0x118
    li r6, 0x0
    bl fn_8012640C
    cmpwi r3, 0x0
    bne @L_802281E8
    cmpwi r24, 0x6
    beq @L_80228190
    bge @L_80228184
    cmpwi r24, 0x4
    beq @L_802281B0
    b @L_802281D0
@L_80228184:
    cmpwi r24, 0x8
    bge @L_802281D0
    b @L_802281B0
@L_80228190:
    mr r6, r31
    li r3, 0x0
    li r4, 0x2e
    li r5, 0x0
    bl fn_801F3624
    addi r0, r3, 0x1
    rlwinm r27, r0, 0, 24, 31
    b @L_802281E8
@L_802281B0:
    mr r6, r31
    li r3, 0x0
    li r4, 0x2e
    li r5, 0x2
    bl fn_801F3624
    addi r0, r3, 0x1
    rlwinm r27, r0, 0, 24, 31
    b @L_802281E8
@L_802281D0:
    cmplw r31, r29
    beq @L_802281E8
    rlwinm r0, r30, 0, 16, 31
    cmplwi r0, 0x2e
    bne @L_802281E8
    li r27, 0x2
@L_802281E8:
    cmplwi r25, 0x0
    beq @L_802282AC
    mr r3, r31
    li r4, 0x0
    li r5, 0x111
    li r6, 0x0
    li r7, 0x1
    bl fn_801254B4
    rlwinm r3, r27, 0, 24, 31
    subf r0, r27, r25
    subfc r4, r25, r3
    mr r3, r26
    subfe r4, r4, r4
    rlwinm r0, r0, 0, 24, 31
    and r25, r0, r4
    mr r6, r28
    rlwinm r7, r25, 0, 24, 31
    li r4, 0x0
    li r5, 0x80
    bl fn_801254B4
    mr r3, r31
    li r4, 0x10
    bl fn_802026E4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_802282AC
    mr r3, r31
    li r4, 0x31
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802282AC
    mr r3, r31
    li r4, 0x31
    bl fn_80201890
    rlwinm r28, r28, 0, 24, 31
    li r0, 0x1
    slw r0, r0, r28
    and. r0, r3, r0
    bne @L_802282AC
    mr r3, r31
    bl fn_801FECD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802282AC
    mr r3, r31
    mr r5, r28
    li r4, 0x80
    li r6, 0x0
    bl fn_801FE7EC
@L_802282AC:
    lwz r0, lbl_8047B618(r13)
    lwz r3, lbl_8047B610(r13)
    rlwinm r4, r0, 0, 21, 19
    addi r0, r3, 0x1
    stw r4, lbl_8047B618(r13)
    stw r0, lbl_8047B610(r13)
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
