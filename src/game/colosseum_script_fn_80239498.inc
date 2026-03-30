    nofralloc
    stwu r1, -0x10(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x14(r1)
    stmw r30, 0x8(r1)
    mr r30, r4
    mr r31, r5
    li r4, 0x0
    li r5, 0x43
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r4, r30
    rlwinm r6, r31, 0, 24, 31
    li r3, 0x0
    li r5, 0x1a
    bl fn_8011BEB4
    lmw r30, 0x8(r1)
    rlwinm r3, r3, 0, 24, 31
    lwz r0, 0x14(r1)
    mtlr r0
    addi r1, r1, 0x10
    blr
