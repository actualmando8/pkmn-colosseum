    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r4, 0x0
    li r5, 0x14
    stw r0, 0x34(r1)
    li r6, 0x0
    stmw r25, 0x14(r1)
    mr r31, r3
    li r3, 0x0
    bl fn_801F54A4
    rlwinm r27, r3, 0, 16, 31
    mr r3, r31
    bl fn_8020D908
    li r4, 0x0
    mr r30, r3
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r26, r3
    bl fn_80209CB4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020B78C
    li r3, 0x0
    b @L_8020B8FC
@L_8020B78C:
    mr r3, r26
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r3, r3, 0, 16, 31
    mr r4, r27
    bl fn_801F00D0
    mr r29, r3
    mr r7, r30
    li r3, 0x0
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F4C14
    mr r7, r29
    li r3, 0x0
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F4C14
    mr r3, r30
    bl fn_80205B8C
    mr r0, r3
    mr r3, r30
    mr r25, r0
    li r4, 0x0
    li r5, 0xd9
    li r6, 0x0
    bl fn_8012640C
    mr r28, r3
    mr r3, r30
    bl fn_80205224
    mr r0, r3
    mr r3, r28
    mr r26, r0
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_8011BEB4
    extsb r29, r3
    mr r3, r28
    li r4, 0x0
    li r5, 0x32
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm. r0, r3, 0, 24, 31
    bne @L_8020B8EC
    rlwinm r29, r29, 0, 24, 31
    mr r3, r25
    li r4, 0x0
    li r5, 0x7f
    mr r6, r29
    bl fn_8012640C
    rlwinm r3, r3, 0, 16, 31
    rlwinm r0, r26, 0, 16, 31
    cmplw r0, r3
    beq @L_8020B8EC
    mr r3, r25
    mr r6, r29
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    rlwinm r26, r3, 0, 16, 31
    mr r3, r28
    mr r7, r26
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r28
    mr r4, r26
    bl fn_80209C1C
    mr r3, r30
    mr r4, r26
    mr r5, r27
    li r6, 0x0
    li r7, 0x1
    li r8, 0x0
    li r9, -0x1
    bl fn_8022B2CC
    mr r0, r3
    li r3, 0x0
    mr r7, r0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801F4C14
@L_8020B8EC:
    mr r3, r31
    mr r4, r26
    bl fn_802128D0
    li r3, 0x1
@L_8020B8FC:
    lmw r25, 0x14(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
