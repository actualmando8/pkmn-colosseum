    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r30, r4
    mr r31, r5
    mr r29, r3
    li r4, 0x0
    li r5, 0x43
    bl fn_801FB1C0
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm. r0, r31, 0, 16, 31
    bne @L_80237FC4
    li r3, 0x0
    b @L_8023804C
@L_80237FC4:
    li r3, 0x0
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
    rlwinm r4, r3, 0, 16, 31
    li r3, 0x0
    li r5, 0x2b
    li r6, 0x0
    bl fn_801FB1C0
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8023802C
    mr r3, r30
    bl fn_80207BF4
    b @L_80238030
@L_8023802C:
    li r3, 0x0
@L_80238030:
    rlwinm r4, r31, 0, 16, 31
    rlwinm r0, r3, 0, 16, 31
    cmplw r4, r0
    bne @L_80238048
    li r3, 0x1
    b @L_8023804C
@L_80238048:
    li r3, 0x0
@L_8023804C:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
