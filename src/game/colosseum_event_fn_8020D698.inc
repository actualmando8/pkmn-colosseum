    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    bl fn_801EF624
    mr r29, r3
    bl fn_8020E0F8
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    li r4, 0x0
    li r5, 0x0
    li r6, 0x0
    bl fn_801F54A4
    mr r4, r29
    bl fn_801F4860
    li r3, 0x0
    bl fn_801F17B0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r29, r3, 0, 16, 31
    mr r3, r31
    bl fn_8020E0B0
    mr r30, r3
    mr r5, r29
    li r3, 0x4
    li r4, 0x0
    bl fn_801F02AC
    mr r31, r3
    mr r4, r30
    li r3, 0x0
    li r5, 0x3
    li r6, 0x0
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r31
    bl fn_801F7480
    mr r5, r29
    li r3, 0x5
    li r4, 0x0
    bl fn_801F02AC
    mr r31, r3
    mr r4, r30
    li r3, 0x0
    li r5, 0x3
    li r6, 0x1
    bl fn_801F54A4
    rlwinm r4, r3, 0, 16, 31
    mr r3, r31
    bl fn_801F7480
    lmw r29, 0x14(r1)
    li r3, 0x1
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
