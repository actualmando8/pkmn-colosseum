    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x24(r1)
    mr r0, r4
    li r4, 0x0
    stmw r29, 0x14(r1)
    mr r29, r3
    mr r30, r5
    mr r3, r0
    li r5, 0xd6
    bl fn_8012640C
    mr r0, r3
    li r3, 0x0
    mr r31, r0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
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
    bl fn_80205BE8
    mr r3, r31
    bl fn_80205BE8
    rlwinm r6, r30, 0, 24, 31
    li r4, 0x0
    li r5, 0x7f
    bl fn_8012640C
    lmw r29, 0x14(r1)
    rlwinm r3, r3, 0, 16, 31
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
