    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x43
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r31, r4
    mr r29, r3
    li r4, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r30, r3, 0, 16, 31
    mr r3, r29
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r31
    bl fn_8020156C
    mr r31, r3
    mr r4, r30
    li r3, 0x0
    li r5, 0x2f
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r5, r3, 0, 24, 31
    rlwinm r0, r31, 0, 16, 31
    srawi r4, r5, 31
    lmw r29, 0x14(r1)
    rlwinm r3, r0, 1, 31, 31
    subfc r0, r0, r5
    adde r0, r4, r3
    rlwinm r3, r0, 0, 24, 31
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
