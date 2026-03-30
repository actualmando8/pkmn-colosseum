    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x94(r1)
    stmw r24, 0x70(r1)
    mr r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r4, r27
    addi r5, r1, 0x10
    li r3, 0x0
    li r6, 0x0
    bl fn_801F1A6C
    mr r26, r3
    mr r3, r27
    mr r4, r29
    bl fn_80239564
    clrlwi r25, r3, 24
    li r3, 0x0
    li r4, 0x100
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r25, r25, r3
    li r3, 0x0
    mr r4, r25
    bl fn_802399FC
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r25, 0x8(r1)
    mr r5, r3
    mr r4, r27
    subi r3, r6, 0x139c
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x100
    bl fn_80239CCC
    mr r4, r27
    mr r8, r28
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248978
    mr r3, r31
    mr r4, r27
    li r5, 0x101
    bl fn_80239984
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r27
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x101
    bl fn_80239EE8
@L_80248978:
    addi r25, r1, 0x10
    clrlwi r26, r26, 16
    li r24, 0x0
    b @L_80248A3C
@L_80248988:
    mr r3, r30
    li r4, 0x0
    li r5, 0xd5
    li r6, 0x0
    bl fn_8012640C
    clrlslwi r0, r24, 16, 2
    lwzx r4, r25, r0
    cmplw r3, r4
    beq @L_80248A38
    mr r3, r27
    li r5, 0x7
    bl fn_802384B4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248A38
    mr r3, r27
    mr r4, r29
    bl fn_80239564
    clrlwi r25, r3, 24
    li r3, 0x0
    li r4, 0x102
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r25, r25, r3
    mr r3, r31
    mr r4, r25
    bl fn_802399FC
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r25, 0x8(r1)
    mr r5, r3
    mr r4, r27
    subi r3, r6, 0x139c
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x102
    bl fn_80239CCC
    b @L_80248A48
@L_80248A38:
    addi r24, r24, 0x1
@L_80248A3C:
    clrlwi r0, r24, 16
    cmplw r0, r26
    blt @L_80248988
@L_80248A48:
    mr r3, r27
    mr r4, r30
    bl fn_80237310
    clrlwi. r0, r3, 24
    beq @L_80248A94
    mr r3, r27
    mr r4, r30
    li r5, 0x28
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    beq @L_80248A94
    mr r3, r27
    mr r4, r30
    li r5, 0x13
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80248B04
@L_80248A94:
    mr r3, r27
    mr r4, r29
    bl fn_80239564
    clrlwi r25, r3, 24
    li r3, 0x0
    li r4, 0x103
    li r5, 0x3e
    li r6, 0x0
    bl fn_801FB1C0
    divw r25, r25, r3
    mr r3, r31
    mr r4, r25
    bl fn_802399FC
    mr r0, r3
    mr r3, r28
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    stw r25, 0x8(r1)
    mr r5, r3
    mr r4, r27
    subi r3, r6, 0x139c
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x103
    bl fn_80239CCC
@L_80248B04:
    mr r3, r31
    lmw r24, 0x70(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
