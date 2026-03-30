    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    mr r0, r4
    stmw r28, 0x10(r1)
    mr r29, r5
    mr r28, r3
    mr r30, r6
    mr r4, r29
    mr r5, r0
    bl fn_802395C8
    mr r0, r3
    mr r3, r28
    mr r31, r0
    mr r4, r29
    bl fn_80239500
    mr r6, r3
    mr r3, r28
    mr r4, r30
    mr r5, r31
    bl fn_8023793C
    clrlwi r0, r3, 16
    lmw r28, 0x10(r1)
    subfic r3, r0, 0x43
    subic r0, r3, 0x1
    subfe r3, r0, r3
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
