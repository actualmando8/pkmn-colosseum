    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    rlwinm r7, r4, 0, 16, 31
    li r5, 0x28
    stw r0, 0x14(r1)
    li r6, 0x0
    stmw r30, 0x8(r1)
    mr r31, r4
    mr r30, r3
    li r4, 0x0
    bl fn_8011BBD8
    mr r4, r31
    li r3, 0x0
    li r5, 0x7
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r7, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_8011BBD8
    mr r4, r31
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_8011BEB4
    rlwinm r7, r3, 0, 16, 31
    mr r3, r30
    li r4, 0x0
    li r5, 0x30
    li r6, 0x0
    bl fn_8011BBD8
    lmw r30, 0x8(r1)
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
