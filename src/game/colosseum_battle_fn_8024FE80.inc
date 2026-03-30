    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    li r5, 0x1
    stw r0, 0x74(r1)
    stmw r22, 0x48(r1)
    mr r30, r4
    mr r29, r3
    li r4, 0x0
    bl fn_80235B04
    lis r4, 0x8028
    li r0, 0x2
    addi r4, r4, -23520
    mr r31, r3
    addi r6, r1, 0x4
    subi r4, r4, 0x4
    mtctr r0
@L_8024FEC0:
    lwz r3, 0x4(r4)
    lwzu r0, 0x8(r4)
    stw r3, 0x4(r6)
    stwu r0, 0x8(r6)
    bdnz @L_8024FEC0
    lwz r0, 0x4(r4)
    mr r4, r29
    addi r5, r1, 0x1c
    li r3, 0x0
    stw r0, 0x4(r6)
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r28, r3
    mr r3, r29
    mr r4, r30
    li r5, 0x16
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024FF6C
    addi r25, r1, 0x1c
    clrlwi r26, r28, 16
    li r24, 0x0
    b @L_8024FF60
@L_8024FF24:
    clrlslwi r27, r24, 16, 2
    mr r3, r29
    lwzx r4, r25, r27
    bl fn_8023715C
    lwzx r4, r25, r27
    mr r27, r3
    mr r3, r29
    bl fn_80236FFC
    clrlwi r4, r27, 16
    clrlwi r0, r3, 16
    cmplw r4, r0
    blt @L_8024FF5C
    li r3, 0x25
    b @L_8025005C
@L_8024FF5C:
    addi r24, r24, 0x1
@L_8024FF60:
    clrlwi r0, r24, 16
    cmplw r0, r26
    blt @L_8024FF24
@L_8024FF6C:
    mr r3, r29
    mr r4, r30
    li r5, 0x24
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250008
    addi r27, r1, 0x8
    addi r25, r1, 0x1c
    clrlwi r28, r28, 16
    li r24, 0x0
    b @L_8024FFFC
@L_8024FF9C:
    clrlslwi r26, r24, 16, 2
    li r22, 0x0
    li r23, 0x0
    b @L_8024FFD8
@L_8024FFAC:
    clrlslwi r0, r23, 16, 1
    lwzx r4, r25, r26
    lhzx r5, r27, r0
    mr r3, r29
    bl fn_80237F74
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024FFD4
    li r22, 0x1
    b @L_8024FFE4
@L_8024FFD4:
    addi r23, r23, 0x1
@L_8024FFD8:
    clrlwi r0, r23, 16
    cmplwi r0, 0xa
    blt @L_8024FFAC
@L_8024FFE4:
    clrlwi r0, r22, 24
    cmplwi r0, 0x1
    beq @L_8024FFF8
    li r3, 0x26
    b @L_8025005C
@L_8024FFF8:
    addi r24, r24, 0x1
@L_8024FFFC:
    clrlwi r0, r24, 16
    cmplw r0, r28
    blt @L_8024FF9C
@L_80250008:
    clrlwi. r0, r31, 24
    beq @L_80250058
    mr r3, r29
    mr r4, r30
    li r5, 0x4d
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250034
    li r3, 0x27
    b @L_8025005C
@L_80250034:
    mr r3, r29
    mr r4, r30
    li r5, 0xd
    bl fn_80239058
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80250058
    li r3, 0x28
    b @L_8025005C
@L_80250058:
    li r3, 0x0
@L_8025005C:
    lmw r22, 0x48(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
