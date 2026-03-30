    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x14
    li r6, 0x0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r31, r3
    mr r27, r4
    mr r29, r7
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    rlwinm r30, r3, 0, 16, 31
    mr r4, r27
    mr r5, r30
    li r3, 0xf
    bl fn_801F02AC
    mr r28, r3
    mr r4, r27
    mr r5, r30
    li r3, 0x10
    bl fn_801F02AC
    mr r0, r3
    mr r3, r27
    mr r30, r0
    mr r4, r31
    mr r6, r29
    li r5, 0x1
    bl fn_80208750
    mr r3, r28
    mr r4, r31
    mr r6, r29
    li r5, 0x2
    bl fn_80208750
    mr r3, r30
    mr r4, r31
    mr r6, r29
    li r5, 0x2
    bl fn_80208750
    mr r3, r28
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020F008
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020F008
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r27
    mr r4, r31
    li r5, 0x1
    li r6, 0x1
    bl fn_802085C4
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r28
    mr r4, r31
    li r5, 0x2
    li r6, 0x1
    bl fn_802085C4
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r30
    mr r4, r31
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
    b @L_8020F0EC
@L_8020F008:
    mr r3, r30
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020F068
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r27
    mr r4, r31
    li r5, 0x1
    li r6, 0x1
    bl fn_802085C4
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r30
    mr r4, r31
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
    b @L_8020F0EC
@L_8020F068:
    mr r3, r28
    bl fn_802062FC
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020F0C8
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r27
    mr r4, r31
    li r5, 0x1
    li r6, 0x1
    bl fn_802085C4
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r28
    mr r4, r31
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
    b @L_8020F0EC
@L_8020F0C8:
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r27
    mr r4, r31
    li r5, 0x1
    li r6, 0x1
    bl fn_802085C4
@L_8020F0EC:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
