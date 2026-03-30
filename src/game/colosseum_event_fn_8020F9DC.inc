    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r31, r7
    mr r30, r5
    mr r28, r3
    mr r29, r4
    mr r6, r31
    mr r3, r29
    mr r4, r28
    li r5, 0x1
    bl fn_80208750
    mr r3, r30
    mr r4, r28
    mr r6, r31
    li r5, 0x2
    bl fn_80208750
    li r3, 0x11
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r29
    mr r4, r28
    li r5, 0x1
    li r6, 0x1
    bl fn_802085C4
    li r3, 0x12
    bl fn_801F0234
    bl fn_801F0204
    mr r7, r3
    mr r3, r30
    mr r4, r28
    li r5, 0x2
    li r6, 0x0
    bl fn_802085C4
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
