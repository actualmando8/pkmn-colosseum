    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    mr r6, r7
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r31, r5
    mr r29, r3
    mr r30, r4
    mr r3, r30
    li r5, 0x1
    mr r4, r29
    bl fn_80208750
    mr r3, r31
    li r4, 0x37
    bl fn_802026E4
    rlwinm r0, r3, 0, 24, 31
    cmplwi r0, 0x1
    bne @L_8020F3D4
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r30
    mr r4, r29
    li r5, 0x1
    li r6, 0x0
    bl fn_802085C4
@L_8020F3D4:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
