    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r26, 0x18(r1)
    mr r29, r6
    mr r27, r4
    mr r28, r5
    mr r26, r3
    mr r5, r29
    li r31, 0x0
    li r4, 0x0
    li r6, 0x0
    bl fn_80235BE4
    mr r30, r3
    mr r4, r29
    li r3, 0x0
    bl fn_801F4354
    li r0, 0x0
    addi r4, r1, 0x8
    sth r0, 0x8(r1)
    bl fn_801F8A18
    cmplwi r3, 0x0
    bne @L_8024BAA4
    li r30, 0x1
@L_8024BAA4:
    clrlwi. r0, r30, 24
    bne @L_8024BAF4
    mr r4, r26
    li r3, 0x0
    li r5, 0x9f
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x9f
    bl fn_80239EE8
@L_8024BAF4:
    mr r3, r26
    mr r4, r29
    li r5, 0x3
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024BB58
    mr r3, r31
    mr r4, r26
    li r5, 0xa0
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xa0
    bl fn_80239EE8
@L_8024BB58:
    mr r3, r26
    mr r4, r29
    li r5, 0x4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024BBBC
    mr r3, r31
    mr r4, r26
    li r5, 0xa1
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xa1
    bl fn_80239EE8
@L_8024BBBC:
    mr r3, r26
    mr r4, r29
    li r5, 0x6
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024BC20
    mr r3, r31
    mr r4, r26
    li r5, 0xa2
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xa2
    bl fn_80239EE8
@L_8024BC20:
    mr r3, r26
    mr r4, r29
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024BC84
    mr r3, r31
    mr r4, r26
    li r5, 0xa4
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xa4
    bl fn_80239EE8
@L_8024BC84:
    mr r3, r26
    mr r4, r29
    li r5, 0xa
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024BCE8
    mr r3, r31
    mr r4, r26
    li r5, 0xa5
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xa5
    bl fn_80239EE8
@L_8024BCE8:
    mr r3, r26
    mr r4, r29
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024BD4C
    mr r3, r31
    mr r4, r26
    li r5, 0xa6
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xa6
    bl fn_80239EE8
@L_8024BD4C:
    mr r3, r26
    mr r4, r29
    li r5, 0x1e
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024BDB0
    mr r3, r31
    mr r4, r26
    li r5, 0xa7
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xa7
    bl fn_80239EE8
@L_8024BDB0:
    mr r3, r26
    mr r4, r29
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024BE14
    mr r3, r31
    mr r4, r26
    li r5, 0xa3
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xa3
    bl fn_80239EE8
@L_8024BE14:
    clrlwi. r0, r30, 24
    beq @L_8024BE64
    mr r3, r31
    mr r4, r26
    li r5, 0xa8
    bl fn_80239984
    mr r0, r3
    mr r3, r27
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r26
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0xa8
    bl fn_80239EE8
@L_8024BE64:
    mr r3, r31
    lmw r26, 0x18(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
