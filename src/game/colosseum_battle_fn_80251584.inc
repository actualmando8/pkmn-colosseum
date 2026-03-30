    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r29, r5
    mr r27, r3
    mr r28, r4
    mr r30, r6
    li r31, 0x1
    li r5, 0x2d
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802515CC
    mr r3, r28
    li r4, 0x2d
    bl fn_80202360
    mr r31, r3
@L_802515CC:
    mr r3, r27
    mr r4, r29
    mr r5, r28
    mr r6, r30
    li r7, 0x0
    li r8, 0x0
    li r9, 0x0
    li r10, 0x0
    bl fn_80211170
    extsh r0, r31
    lmw r27, 0xc(r1)
    mullw r3, r0, r3
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
