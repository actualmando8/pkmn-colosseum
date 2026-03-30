    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r26, 0x8(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    mr r26, r6
    li r27, 0x0
    li r3, 0x0
    li r4, 0x0
    li r5, 0x14
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r4, r3, 16
    mr r3, r30
    bl fn_801F0134
    mr r28, r3
    mr r3, r29
    mr r4, r26
    li r5, 0x1d
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248388
    mr r3, r26
    li r4, 0x1d
    bl fn_80201D84
    clrlwi r3, r3, 16
    clrlwi r0, r28, 16
    cmplw r0, r3
    bne @L_80248388
    mr r4, r29
    li r3, 0x0
    li r5, 0x10a
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r27, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x10a
    bl fn_80239EE8
@L_80248388:
    mr r3, r27
    mr r4, r29
    li r5, 0x10b
    bl fn_80239984
    mr r28, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x10b
    bl fn_80239EE8
    mr r3, r29
    mr r4, r26
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80248404
    mr r3, r29
    mr r4, r26
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248448
@L_80248404:
    mr r3, r28
    mr r4, r29
    li r5, 0x10c
    bl fn_80239984
    mr r28, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x10c
    bl fn_80239EE8
@L_80248448:
    mr r3, r28
    lmw r26, 0x8(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
