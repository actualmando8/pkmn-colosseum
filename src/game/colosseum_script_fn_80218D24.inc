    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x0
    li r4, 0x0
    stw r0, 0x34(r1)
    li r5, 0x14
    li r6, 0x0
    stmw r25, 0x14(r1)
    bl fn_801F54A4
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x11
    li r4, 0x0
    bl fn_801F025C
    mr r30, r3
    bl fn_80205B8C
    mr r0, r3
    mr r3, r30
    mr r26, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r27, r3
    addi r4, r1, 0x8
    li r5, 0x0
    li r3, -0x1
    b @L_80218D9C
@L_80218D90:
    clrlslwi r0, r5, 24, 1
    addi r5, r5, 0x1
    sthx r3, r4, r0
@L_80218D9C:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_80218D90
    li r29, 0x0
    li r28, 0x0
    b @L_80218EDC
@L_80218DB4:
    rlwinm r25, r28, 0, 16, 31
    mr r3, r26
    mr r4, r25
    bl fn_80123CD4
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80218ED8
    mr r3, r30
    mr r4, r25
    li r5, 0x0
    li r6, 0x0
    bl fn_801FFEC8
    rlwinm. r0, r3, 0, 24, 31
    beq @L_80218DF0
    cmplwi r0, 0x6
    bne @L_80218ED8
@L_80218DF0:
    mr r3, r26
    mr r6, r25
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm. r4, r3, 0, 16, 31
    beq @L_80218ED8
    cmplwi r4, 0x165
    beq @L_80218ED8
    cmplwi r4, 0x163
    beq @L_80218ED8
    cmplwi r4, 0x0
    beq @L_80218E4C
    cmplwi r4, 0x165
    beq @L_80218E4C
    cmplwi r4, 0xd6
    beq @L_80218E4C
    cmplwi r4, 0x112
    beq @L_80218E4C
    cmplwi r4, 0x77
    beq @L_80218E4C
    cmplwi r4, 0x76
    bne @L_80218E54
@L_80218E4C:
    li r0, 0x1
    b @L_80218E58
@L_80218E54:
    li r0, 0x0
@L_80218E58:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80218ED8
    cmplwi r4, 0x108
    beq @L_80218ED8
    cmplwi r4, 0xfd
    beq @L_80218ED8
    li r3, 0x0
    li r5, 0x9
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x91
    beq @L_80218EB4
    cmplwi r0, 0x27
    beq @L_80218EB4
    cmplwi r0, 0x4b
    beq @L_80218EB4
    cmplwi r0, 0x97
    beq @L_80218EB4
    cmplwi r0, 0x9b
    beq @L_80218EB4
    cmplwi r0, 0x1a
    bne @L_80218EBC
@L_80218EB4:
    li r0, 0x1
    b @L_80218EC0
@L_80218EBC:
    li r0, 0x0
@L_80218EC0:
    rlwinm. r0, r0, 0, 24, 31
    bne @L_80218ED8
    clrlslwi r0, r29, 24, 1
    addi r3, r1, 0x8
    sthx r28, r3, r0
    addi r29, r29, 0x1
@L_80218ED8:
    addi r28, r28, 0x1
@L_80218EDC:
    extsh r0, r28
    cmpwi r0, 0x4
    blt @L_80218DB4
    rlwinm. r0, r29, 0, 24, 31
    bne @L_80218F00
@L_80218EF0:
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
    b @L_80218FC8
@L_80218F00:
    bl fn_800E0C54
    rlwinm r5, r3, 0, 16, 31
    rlwinm r4, r29, 0, 24, 31
    divw r0, r5, r4
    addi r3, r1, 0x8
    mullw r0, r0, r4
    subf r0, r0, r5
    clrlslwi r0, r0, 24, 1
    lhax r25, r3, r0
    extsh. r0, r25
    blt @L_80218EF0
    mr r3, r26
    rlwinm r6, r25, 0, 16, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm. r0, r3, 0, 16, 31
    beq @L_80218EF0
    cmplwi r0, 0x165
    beq @L_80218EF0
    cmplwi r0, 0x163
    beq @L_80218EF0
    sth r0, lbl_8047B60C(r13)
    mr r3, r27
    mr r7, r25
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_8011BBD8
    lwz r0, lbl_8047B618(r13)
    mr r3, r30
    lhz r4, lbl_8047B60C(r13)
    mr r5, r31
    rlwinm r0, r0, 0, 22, 20
    li r6, 0x0
    stw r0, lbl_8047B618(r13)
    li r7, 0x1
    li r8, 0x1
    li r9, -0x1
    bl fn_8022B2CC
    mr r0, r3
    li r3, 0x0
    mr r7, r0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
@L_80218FC8:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
