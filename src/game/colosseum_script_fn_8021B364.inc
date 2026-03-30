    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r30, r3
    mr r31, r4
    bl fn_80205B8C
    mr r26, r3
    li r4, 0x0
    li r3, -0x1
    b @L_8021B39C
@L_8021B390:
    clrlslwi r0, r4, 24, 2
    addi r4, r4, 0x1
    stwx r3, r31, r0
@L_8021B39C:
    rlwinm r0, r4, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8021B390
    li r27, 0x0
    li r28, 0x0
    b @L_8021B460
@L_8021B3B4:
    rlwinm r29, r28, 0, 24, 31
    mr r3, r26
    mr r4, r29
    bl fn_80123CD4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021B45C
    mr r3, r26
    mr r6, r29
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r4, r3, 0, 16, 31
    cmplwi r4, 0xa5
    beq @L_8021B45C
    cmplwi r4, 0x164
    beq @L_8021B45C
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r29, r3, 0, 16, 31
    cmplwi r29, 0x9
    bne @L_8021B438
    mr r3, r30
    li r4, 0x7
    bl fn_80207AE0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8021B434
    li r29, 0x7
    b @L_8021B438
@L_8021B434:
    li r29, 0x0
@L_8021B438:
    mr r3, r30
    mr r4, r29
    bl fn_80207AE0
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8021B45C
    clrlslwi r0, r27, 24, 2
    rlwinm r3, r29, 0, 16, 31
    stwx r3, r31, r0
    addi r27, r27, 0x1
@L_8021B45C:
    addi r28, r28, 0x1
@L_8021B460:
    rlwinm r0, r28, 0, 24, 31
    cmplwi r0, 0x4
    blt @L_8021B3B4
    mr r3, r27
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
