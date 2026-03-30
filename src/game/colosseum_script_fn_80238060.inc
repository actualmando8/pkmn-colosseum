    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x24(r1)
    mr r0, r4
    li r4, 0x0
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r29, r5
    mr r3, r0
    li r5, 0xd6
    bl fn_8012640C
    mr r0, r3
    mr r3, r27
    mr r28, r0
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r31, r3, 0, 16, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r27
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r28
    bl fn_80205BE8
    mr r3, r28
    bl fn_80205BE8
    rlwinm r29, r29, 0, 24, 31
    li r4, 0x0
    mr r6, r29
    li r5, 0x80
    bl fn_8012640C
    rlwinm r30, r3, 0, 24, 31
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    mr r3, r27
    li r4, 0x0
    li r5, 0x43
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    mr r3, r28
    bl fn_80205BE8
    mr r3, r28
    bl fn_80205BE8
    mr r4, r29
    bl fn_80123E70
    rlwinm r29, r3, 0, 24, 31
    mr r4, r31
    li r3, 0x0
    li r5, 0x37
    li r6, 0x0
    bl fn_801FB1C0
    mulli r0, r30, 0x64
    rlwinm r5, r3, 0, 24, 31
    srawi r4, r5, 31
    divw r0, r0, r29
    rlwinm r3, r0, 1, 31, 31
    subfc r0, r0, r5
    adde r3, r4, r3
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
