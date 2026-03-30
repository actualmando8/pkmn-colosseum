    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    li r5, 0x1
    stw r0, 0x24(r1)
    stmw r29, 0x14(r1)
    mr r31, r6
    mr r29, r3
    mr r30, r4
    mr r3, r30
    mr r6, r7
    mr r4, r29
    bl fn_80208750
    rlwinm. r0, r31, 0, 16, 31
    bne @L_8020FB1C
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r30
    mr r4, r29
    li r5, 0x1
    li r6, 0x0
    bl fn_802085C4
@L_8020FB1C:
    lmw r29, 0x14(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
