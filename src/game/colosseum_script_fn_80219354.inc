    nofralloc
    stwu r1, -0x60(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x64(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r28, 0x50(r1)
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    li r4, 0x0
    mr r31, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r3, r31
    li r4, 0x0
    li r5, 0xf3
    li r6, 0x0
    bl fn_8012640C
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0x102
    li r6, 0x0
    bl fn_8012640C
    mr r3, r31
    li r4, 0x0
    li r5, 0x104
    li r6, 0x0
    bl fn_8012640C
    mr r3, r31
    li r4, 0x0
    li r5, 0xf4
    li r6, 0x0
    bl fn_8012640C
    rlwinm r28, r3, 0, 16, 31
    mr r3, r31
    li r4, 0x0
    li r5, 0xf2
    li r6, 0x0
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    mr r4, r30
    bl fn_801F00D0
    cmplwi r29, 0x0
    mr r30, r3
    beq @L_80219438
    cmplwi r29, 0x165
    beq @L_80219438
    cmplwi r29, 0xffff
    beq @L_80219438
    cmplwi r29, 0x164
    bne @L_80219448
@L_80219438:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021958C
@L_80219448:
    cmplwi r30, 0x0
    beq @L_802194D4
    mr r4, r29
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x91
    beq @L_80219498
    cmplwi r0, 0x27
    beq @L_80219498
    cmplwi r0, 0x4b
    beq @L_80219498
    cmplwi r0, 0x97
    beq @L_80219498
    cmplwi r0, 0x9b
    beq @L_80219498
    cmplwi r0, 0x1a
    bne @L_802194A0
@L_80219498:
    li r0, 0x1
    b @L_802194A4
@L_802194A0:
    li r0, 0x0
@L_802194A4:
    rlwinm. r0, r0, 0, 24, 31
    beq @L_802194D4
    mr r3, r30
    li r4, 0x22
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_802194D4
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021958C
@L_802194D4:
    mr r3, r31
    mr r4, r28
    addi r5, r1, 0x8
    bl fn_802079DC
    rlwinm. r0, r3, 0, 16, 31
    mr r30, r3
    bne @L_80219500
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021958C
@L_80219500:
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r3, r30, 0, 16, 31
    divw r0, r4, r3
    addi r5, r1, 0x8
    mullw r0, r0, r3
    subf r0, r0, r4
    clrlslwi r0, r0, 16, 2
    lwzx r0, r5, r0
    cmpwi r0, 0x0
    bge @L_8021953C
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021958C
@L_8021953C:
    rlwinm r28, r0, 0, 16, 31
    li r29, 0x0
    b @L_8021955C
@L_80219548:
    mr r3, r31
    mr r5, r28
    rlwinm r4, r29, 0, 24, 31
    bl fn_80207B5C
    addi r29, r29, 0x1
@L_8021955C:
    rlwinm r0, r29, 0, 16, 31
    cmplwi r0, 0x2
    blt @L_80219548
    mr r3, r28
    bl fn_8010C4D4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021958C:
    lmw r28, 0x50(r1)
    lwz r0, 0x64(r1)
    mtlr r0
    addi r1, r1, 0x60
    blr
