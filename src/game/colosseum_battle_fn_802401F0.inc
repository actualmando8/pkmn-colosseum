    nofralloc
    stwu r1, -0x50(r1)
    mflr r0
    li r6, 0x1
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
    li r3, 0x0
    li r4, 0x0
    li r5, 0x18
    li r6, 0x0
    bl fn_801F54A4
    clrlwi r0, r3, 16
    cmplwi r0, 0x2
    blt @L_80240294
    mr r4, r27
    li r3, 0x0
    li r5, 0x1c7
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
    li r10, 0x1c7
    bl fn_80239EE8
@L_80240294:
    addi r26, r1, 0x8
    clrlwi r25, r31, 16
    li r24, 0x0
    b @L_80240320
@L_802402A4:
    clrlslwi r0, r24, 16, 2
    lwzx r4, r26, r0
    cmplw r28, r4
    beq @L_8024031C
    mr r3, r27
    bl fn_8023831C
    clrlwi r0, r3, 16
    cmplwi r0, 0x8
    beq @L_802402D0
    cmplwi r0, 0x9
    bne @L_8024031C
@L_802402D0:
    mr r3, r30
    mr r4, r27
    li r5, 0x1c8
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
    li r10, 0x1c8
    bl fn_80239EE8
    b @L_8024032C
@L_8024031C:
    addi r24, r24, 0x1
@L_80240320:
    clrlwi r0, r24, 16
    cmplw r0, r25
    blt @L_802402A4
@L_8024032C:
    addi r25, r1, 0x8
    clrlwi r26, r31, 16
    li r24, 0x0
    b @L_802403B4
@L_8024033C:
    clrlslwi r0, r24, 16, 2
    lwzx r4, r25, r0
    cmplw r28, r4
    beq @L_802403B0
    mr r3, r27
    li r5, 0x14
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802403B0
    mr r3, r30
    mr r4, r27
    li r5, 0x1c9
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
    li r10, 0x1c9
    bl fn_80239EE8
    b @L_802403C0
@L_802403B0:
    addi r24, r24, 0x1
@L_802403B4:
    clrlwi r0, r24, 16
    cmplw r0, r26
    blt @L_8024033C
@L_802403C0:
    addi r4, r1, 0x8
    clrlwi r0, r31, 16
    li r5, 0x0
    b @L_80240430
@L_802403D0:
    clrlslwi r3, r5, 16, 2
    lwzx r3, r4, r3
    cmplw r28, r3
    beq @L_8024042C
    mr r3, r30
    mr r4, r27
    li r5, 0x1ca
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
    li r10, 0x1ca
    bl fn_80239EE8
    b @L_8024043C
@L_8024042C:
    addi r5, r5, 0x1
@L_80240430:
    clrlwi r3, r5, 16
    cmplw r3, r0
    blt @L_802403D0
@L_8024043C:
    mr r3, r30
    lmw r24, 0x30(r1)
    lwz r0, 0x54(r1)
    mtlr r0
    addi r1, r1, 0x50
    blr
