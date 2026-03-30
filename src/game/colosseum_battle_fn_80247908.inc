    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr r30, r4
    mr r31, r5
    mr r29, r3
    li r27, 0x0
    li r4, 0x0
    li r5, 0x1
    bl fn_80235B04
    mr r28, r3
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024798C
    mr r4, r29
    li r3, 0x0
    li r5, 0x120
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
    li r10, 0x120
    bl fn_80239EE8
@L_8024798C:
    mr r4, r29
    mr r8, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802479FC
    mr r3, r27
    mr r4, r29
    li r5, 0x121
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
    li r10, 0x121
    bl fn_80239EE8
@L_802479FC:
    clrlwi r0, r28, 24
    cmplwi r0, 0x2
    beq @L_80247A18
    cmplwi r0, 0x4
    beq @L_80247A18
    cmplwi r0, 0x3
    bne @L_80247A60
@L_80247A18:
    mr r3, r27
    mr r4, r29
    li r5, 0x122
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
    li r10, 0x122
    bl fn_80239EE8
@L_80247A60:
    clrlwi. r0, r28, 24
    bne @L_80247AB0
    mr r3, r27
    mr r4, r29
    li r5, 0x123
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
    li r10, 0x123
    bl fn_80239EE8
@L_80247AB0:
    mr r3, r27
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
