    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    lis r7, 0x8025
    li r8, 0x0
    stw r0, 0x24(r1)
    addi r9, r7, 9848
    li r7, 0x0
    li r10, 0x0
    stmw r27, 0xc(r1)
    mr r28, r4
    mr r29, r5
    mr r27, r3
    mr r30, r6
    mr r5, r28
    mr r4, r29
    bl fn_80211170
    lis r4, 0x8025
    mr r31, r3
    addi r9, r4, 9780
    mr r3, r27
    mr r4, r29
    mr r5, r28
    mr r6, r30
    li r7, 0x0
    li r8, 0x0
    li r10, 0x0
    bl fn_80211170
    lis r4, 0x8025
    add r31, r31, r3
    addi r9, r4, 9712
    mr r3, r27
    mr r4, r29
    mr r5, r28
    mr r6, r30
    li r7, 0x0
    li r8, 0x0
    li r10, 0x0
    bl fn_80211170
    add r31, r31, r3
    lwz r0, 0x24(r1)
    mr r3, r31
    lmw r27, 0xc(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
