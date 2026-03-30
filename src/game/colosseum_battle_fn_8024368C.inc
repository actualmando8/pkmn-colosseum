    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r30, r3
    mr r26, r4
    mr r31, r5
    mr r27, r6
    li r28, 0x0
    bl fn_802387C8
    mr r29, r3
    mr r3, r30
    mr r4, r27
    bl fn_802387C8
    mulli r0, r29, 0x3
    cmpw r0, r3
    bgt @L_8024371C
    mr r4, r30
    li r3, 0x0
    li r5, 0x17d
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x17d
    bl fn_80239EE8
    b @L_80243820
@L_8024371C:
    slwi r0, r29, 1
    cmpw r0, r3
    bgt @L_80243774
    mr r4, r30
    li r3, 0x0
    li r5, 0x17c
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x17c
    bl fn_80239EE8
    b @L_80243820
@L_80243774:
    mulli r0, r3, 0x3
    cmpw r29, r0
    blt @L_802437CC
    mr r4, r30
    li r3, 0x0
    li r5, 0x17f
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x17f
    bl fn_80239EE8
    b @L_80243820
@L_802437CC:
    slwi r0, r3, 1
    cmpw r29, r0
    blt @L_80243820
    mr r4, r30
    li r3, 0x0
    li r5, 0x17e
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r28, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x17e
    bl fn_80239EE8
@L_80243820:
    mr r3, r28
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
