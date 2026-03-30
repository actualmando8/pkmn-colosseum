    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    stw r0, 0x34(r1)
    stmw r24, 0x10(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    mr r24, r6
    li r27, 0x0
    bl fn_802383A4
    mr r28, r3
    mr r3, r29
    mr r4, r24
    bl fn_802383A4
    mr r0, r3
    mr r3, r30
    mr r25, r0
    li r26, 0x1
    bl fn_80216048
    clrlwi. r0, r3, 24
    bne @L_80247808
    li r26, 0x0
@L_80247808:
    clrlwi. r0, r28, 16
    bne @L_80247834
    clrlwi r0, r25, 16
    cmplwi r0, 0xaf
    beq @L_80247834
    cmplwi r0, 0x0
    beq @L_80247834
    mr r3, r25
    bl fn_80142984
    clrlwi. r0, r3, 24
    bne @L_80247838
@L_80247834:
    li r26, 0x0
@L_80247838:
    clrlwi r0, r26, 24
    cmplwi r0, 0x1
    bne @L_8024788C
    mr r4, r29
    li r3, 0x0
    li r5, 0x124
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
    li r10, 0x124
    bl fn_80239EE8
@L_8024788C:
    mr r3, r29
    mr r4, r24
    li r5, 0x3c
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802478F0
    mr r3, r27
    mr r4, r29
    li r5, 0x125
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
    li r10, 0x125
    bl fn_80239EE8
@L_802478F0:
    mr r3, r27
    lmw r24, 0x10(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
