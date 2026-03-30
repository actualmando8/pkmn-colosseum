    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r6, 0x0
    li r7, 0x1
    stw r0, 0x54(r1)
    stmw r24, 0x30(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    addi r5, r1, 0x8
    mr r4, r27
    li r30, 0x0
    li r3, 0x0
    bl fn_801F1C18
    mr r31, r3
    mr r3, r27
    mr r4, r28
    bl fn_80236520
    clrlwi r0, r3, 16
    cmplwi r0, 0x117
    bne @L_8024DD18
    mr r4, r27
    li r3, 0x0
    li r5, 0x5b
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x5b
    bl fn_80239EE8
@L_8024DD18:
    addi r25, r1, 0x8
    clrlwi r26, r31, 16
    li r24, 0x0
    b @L_8024DD98
@L_8024DD28:
    clrlslwi r0, r24, 16, 2
    mr r3, r27
    lwzx r4, r25, r0
    li r5, 0x8
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024DD94
    mr r3, r30
    mr r4, r27
    li r5, 0x5c
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x5c
    bl fn_80239EE8
    b @L_8024DDA4
@L_8024DD94:
    addi r24, r24, 0x1
@L_8024DD98:
    clrlwi r0, r24, 16
    cmplw r0, r26
    blt @L_8024DD28
@L_8024DDA4:
    addi r26, r1, 0x8
    clrlwi r31, r31, 16
    li r25, 0x0
    b @L_8024DE24
@L_8024DDB4:
    clrlslwi r0, r25, 16, 2
    mr r3, r27
    lwzx r4, r26, r0
    li r5, 0x7
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024DE20
    mr r3, r30
    mr r4, r27
    li r5, 0x5d
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x5d
    bl fn_80239EE8
    b @L_8024DE30
@L_8024DE20:
    addi r25, r25, 0x1
@L_8024DE24:
    clrlwi r0, r25, 16
    cmplw r0, r31
    blt @L_8024DDB4
@L_8024DE30:
    mr r3, r30
    mr r4, r27
    li r5, 0x5e
    bl fn_80239984
    mr r25, r3
    mr r3, r28
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x5e
    bl fn_80239EE8
    mr r3, r25
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
