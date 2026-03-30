    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r28, r6
    mr r26, r4
    mr r27, r5
    mr r31, r3
    mr r5, r28
    li r30, 0x0
    li r4, 0x0
    li r6, 0x0
    bl fn_80235BE4
    mr r29, r3
    mr r4, r28
    li r3, 0x0
    bl fn_801F4354
    li r0, 0x0
    addi r4, r1, 0x8
    sth r0, 0x8(r1)
    bl fn_801F8A18
    cmplwi r3, 0x0
    bne @L_80249024
    li r29, 0x1
@L_80249024:
    clrlwi. r0, r29, 24
    bne @L_80249074
    mr r4, r31
    li r3, 0x0
    li r5, 0xeb
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xeb
    bl fn_80239EE8
@L_80249074:
    mr r3, r31
    mr r4, r28
    li r5, 0x3
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802490D8
    mr r3, r30
    mr r4, r31
    li r5, 0xec
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xec
    bl fn_80239EE8
@L_802490D8:
    mr r3, r31
    mr r4, r28
    li r5, 0x4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024913C
    mr r3, r30
    mr r4, r31
    li r5, 0xed
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xed
    bl fn_80239EE8
@L_8024913C:
    mr r3, r31
    mr r4, r28
    li r5, 0x6
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802491A0
    mr r3, r30
    mr r4, r31
    li r5, 0xee
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xee
    bl fn_80239EE8
@L_802491A0:
    mr r3, r31
    mr r4, r28
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80249204
    mr r3, r30
    mr r4, r31
    li r5, 0xef
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xef
    bl fn_80239EE8
@L_80249204:
    mr r3, r31
    mr r4, r28
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80249268
    mr r3, r30
    mr r4, r31
    li r5, 0xf0
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xf0
    bl fn_80239EE8
@L_80249268:
    mr r3, r31
    mr r4, r28
    li r5, 0xa
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802492CC
    mr r3, r30
    mr r4, r31
    li r5, 0xf1
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xf1
    bl fn_80239EE8
@L_802492CC:
    mr r3, r31
    mr r4, r28
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80249330
    mr r3, r30
    mr r4, r31
    li r5, 0xf2
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xf2
    bl fn_80239EE8
@L_80249330:
    mr r3, r31
    mr r4, r28
    li r5, 0x1e
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80249394
    mr r3, r30
    mr r4, r31
    li r5, 0xf3
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xf3
    bl fn_80239EE8
@L_80249394:
    mr r3, r31
    mr r4, r28
    li r5, 0x7
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802493F8
    mr r3, r30
    mr r4, r31
    li r5, 0xf4
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xf4
    bl fn_80239EE8
@L_802493F8:
    clrlwi. r0, r29, 24
    beq @L_80249448
    mr r3, r30
    mr r4, r31
    li r5, 0xf5
    bl fn_80239984
    mr r0, r3
    mr r3, r26
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r27
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xf5
    bl fn_80239EE8
@L_80249448:
    mr r3, r30
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
