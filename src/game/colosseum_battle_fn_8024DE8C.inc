    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r28, 0x10(r1)
    mr r29, r4
    mr r28, r3
    mr r30, r5
    mr r4, r6
    li r31, 0x0
    bl fn_802357CC
    clrlwi r0, r3, 24
    cmplwi r0, 0x6
    ble @L_8024DF08
    mr r4, r28
    li r3, 0x0
    li r5, 0x58
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x58
    bl fn_80239EE8
@L_8024DF08:
    mr r3, r28
    mr r4, r29
    bl fn_802358AC
    clrlwi r0, r3, 24
    cmplwi r0, 0x6
    bge @L_8024DF68
    mr r3, r31
    mr r4, r28
    li r5, 0x59
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x59
    bl fn_80239EE8
@L_8024DF68:
    mr r3, r31
    mr r4, r28
    li r5, 0x5a
    bl fn_80239984
    mr r31, r3
    mr r3, r29
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x5a
    bl fn_80239EE8
    mr r3, r31
    lmw r28, 0x10(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
