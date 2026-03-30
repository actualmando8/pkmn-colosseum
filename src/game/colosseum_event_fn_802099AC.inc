    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r27, r3
    mr r28, r4
    mr r29, r5
    mr r31, r6
    mr r30, r7
    beq @L_80209C08
    beq @L_80209B34
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    li r7, -0x1
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x28
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x2a
    li r6, 0x0
    bl fn_8011BEB4
    li r4, 0x9
    bl fn_8011B950
    li r3, 0x3f
    bl fn_80119ED0
    rlwinm r0, r3, 0, 16, 31
    cmplwi r0, 0x2a
    bne @L_80209A74
    mr r3, r27
    li r4, 0x3f
    li r5, 0x0
    bl fn_8011B2C0
@L_80209A74:
    mr r3, r27
    li r4, 0x0
    li r5, 0x2b
    li r6, 0x0
    li r7, 0x1
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x2c
    li r6, 0x0
    li r7, 0x1
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x2d
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x2e
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    li r7, 0x9
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x31
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
    mr r3, r27
    li r4, 0x0
    li r5, 0x32
    li r6, 0x0
    li r7, 0x0
    bl fn_8011BBD8
@L_80209B34:
    mr r3, r27
    extsb r7, r28
    li r4, 0x0
    li r5, 0x26
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r27
    rlwinm r7, r31, 0, 16, 31
    li r4, 0x0
    li r5, 0x29
    li r6, 0x0
    bl fn_8011BBD8
    rlwinm r31, r29, 0, 16, 31
    mr r3, r27
    mr r7, r31
    li r4, 0x0
    li r5, 0x27
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r27
    mr r7, r31
    li r4, 0x0
    li r5, 0x28
    li r6, 0x0
    bl fn_8011BBD8
    mr r4, r29
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r7, r3, 0, 16, 31
    mr r3, r27
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    mr r4, r29
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r7, r3, 0, 16, 31
    mr r3, r27
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BBD8
    mr r3, r27
    rlwinm r7, r30, 0, 24, 31
    li r4, 0x0
    li r5, 0x32
    li r6, 0x0
    bl fn_8011BBD8
@L_80209C08:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
