    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    lwz r3, 0x8(r3)
    bl HSD_MObjSetCurrent
    rlwinm. r0, r30, 0, 5, 5
    bne @L_80199104
    lwz r5, 0x8(r27)
    mr r4, r30
    lwz r3, 0x8(r27)
    lwz r5, 0x0(r5)
    lwz r12, 0x3c(r5)
    mtctr r12
    bctrl
@L_80199104:
    lwz r31, 0xc(r27)
    b @L_80199130
@L_8019910C:
    lwz r6, 0x0(r31)
    mr r3, r31
    mr r4, r28
    mr r5, r29
    lwz r12, 0x3c(r6)
    mr r6, r30
    mtctr r12
    bctrl
    lwz r31, 0x4(r31)
@L_80199130:
    cmplwi r31, 0x0
    bne @L_8019910C
    rlwinm. r0, r30, 0, 5, 5
    bne @L_8019915C
    lwz r5, 0x8(r27)
    mr r4, r30
    lwz r3, 0x8(r27)
    lwz r5, 0x0(r5)
    lwz r12, 0x50(r5)
    mtctr r12
    bctrl
@L_8019915C:
    li r3, 0x0
    bl HSD_MObjSetCurrent
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
