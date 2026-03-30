    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r3, 0x11
    li r4, 0x0
    stw r0, 0x34(r1)
    stmw r27, 0x1c(r1)
    bl fn_801F025C
    mr r31, r3
    bl fn_80205B8C
    mr r29, r3
    addi r4, r1, 0x8
    li r5, 0x0
    li r3, -0x1
    b @L_8021B1E8
@L_8021B1DC:
    clrlslwi r0, r5, 24, 2
    addi r5, r5, 0x1
    stwx r3, r4, r0
@L_8021B1E8:
    rlwinm r0, r5, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8021B1DC
    li r28, 0x0
    mr r27, r28
    b @L_8021B2B0
@L_8021B200:
    rlwinm r30, r27, 0, 24, 31
    mr r3, r29
    mr r4, r30
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021B2AC
    mr r3, r29
    mr r6, r30
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    cmplwi r4, 0xa5
    beq @L_8021B2AC
    cmplwi r4, 0x164
    beq @L_8021B2AC
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r30, r3, 0, 16, 31
    cmplwi r30, 0x9
    bne @L_8021B284
    mr r3, r31
    li r4, 0x7
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021B280
    li r30, 0x7
    b @L_8021B284
@L_8021B280:
    li r30, 0x0
@L_8021B284:
    mr r3, r31
    mr r4, r30
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021B2AC
    clrlslwi r0, r28, 24, 2
    rlwinm r4, r30, 0, 16, 31
    addi r3, r1, 0x8
    addi r28, r28, 0x1
    stwx r4, r3, r0
@L_8021B2AC:
    addi r27, r27, 0x1
@L_8021B2B0:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8021B200
    rlwinm. r0, r28, 0, 24, 31
    bne @L_8021B2D4
@L_8021B2C4:
    lwz r3, lbl_8047B610(r13)
    lwz r0, 0x1(r3)
    stw r0, lbl_8047B610(r13)
    b @L_8021B350
@L_8021B2D4:
    bl fn_800E0C54
    rlwinm r4, r3, 0, 16, 31
    rlwinm r3, r28, 0, 24, 31
    divw r0, r4, r3
    addi r5, r1, 0x8
    mullw r0, r0, r3
    subf r0, r0, r4
    clrlslwi r0, r0, 24, 2
    lwzx r0, r5, r0
    cmpwi r0, 0x0
    blt @L_8021B2C4
    rlwinm r28, r0, 0, 16, 31
    li r27, 0x0
    b @L_8021B320
@L_8021B30C:
    mr r3, r31
    mr r4, r27
    mr r5, r28
    bl fn_80207B5C
    addi r27, r27, 0x1
@L_8021B320:
    rlwinm r0, r27, 0, 24, 31
    cmplwi r0, 0x2
    blt @L_8021B30C
    mr r3, r28
    bl fn_8010C4D4
    bl fn_800FA280
    mr r4, r3
    li r3, 0xd
    bl fn_80132A38
    lwz r3, lbl_8047B610(r13)
    addi r0, r3, 0x5
    stw r0, lbl_8047B610(r13)
@L_8021B350:
    lmw r27, 0x1c(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
