    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x74(r1)
    stmw r20, 0x40(r1)
    mr r31, r3
    mr r29, r4
    mr r28, r5
    mr r25, r6
    mr r4, r31
    addi r5, r1, 0x1c
    li r30, 0x0
    li r27, 0x0
    li r3, 0x0
    li r6, 0x0
    bl fn_801F1C18
    mr r26, r3
    mr r3, r31
    mr r4, r25
    li r5, 0x3
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CF80
    mr r4, r31
    li r3, 0x0
    li r5, 0x69
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x69
    bl fn_80239EE8
@L_8024CF80:
    mr r3, r31
    mr r4, r25
    li r5, 0x4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CFE4
    mr r3, r30
    mr r4, r31
    li r5, 0x6a
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x6a
    bl fn_80239EE8
@L_8024CFE4:
    mr r3, r31
    mr r4, r25
    li r5, 0x6
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D048
    mr r3, r30
    mr r4, r31
    li r5, 0x6b
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x6b
    bl fn_80239EE8
@L_8024D048:
    mr r3, r31
    mr r4, r25
    li r5, 0x5
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D0AC
    mr r3, r30
    mr r4, r31
    li r5, 0x6c
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x6c
    bl fn_80239EE8
@L_8024D0AC:
    mr r3, r31
    mr r4, r25
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D110
    mr r3, r30
    mr r4, r31
    li r5, 0x6d
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x6d
    bl fn_80239EE8
@L_8024D110:
    mr r3, r31
    mr r4, r25
    li r5, 0xa
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D174
    mr r3, r30
    mr r4, r31
    li r5, 0x6e
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x6e
    bl fn_80239EE8
@L_8024D174:
    mr r3, r31
    mr r4, r25
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D1D8
    mr r3, r30
    mr r4, r31
    li r5, 0x6f
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x6f
    bl fn_80239EE8
@L_8024D1D8:
    mr r3, r31
    mr r4, r25
    li r5, 0x1e
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D23C
    mr r3, r30
    mr r4, r31
    li r5, 0x70
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x70
    bl fn_80239EE8
@L_8024D23C:
    mr r3, r31
    mr r4, r25
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D2A0
    mr r3, r30
    mr r4, r31
    li r5, 0x71
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x71
    bl fn_80239EE8
@L_8024D2A0:
    mr r3, r31
    mr r4, r29
    mr r5, r25
    bl fn_80236D60
    cmpwi r3, 0x0
    ble @L_8024D300
    mr r3, r30
    mr r4, r31
    li r5, 0x72
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x72
    bl fn_80239EE8
@L_8024D300:
    mr r3, r31
    mr r4, r29
    li r5, 0x3
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D364
    mr r3, r30
    mr r4, r31
    li r5, 0x73
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x73
    bl fn_80239EE8
@L_8024D364:
    mr r3, r31
    mr r4, r29
    li r5, 0x4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D3C8
    mr r3, r30
    mr r4, r31
    li r5, 0x74
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x74
    bl fn_80239EE8
@L_8024D3C8:
    mr r3, r31
    mr r4, r29
    li r5, 0x6
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D42C
    mr r3, r30
    mr r4, r31
    li r5, 0x75
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x75
    bl fn_80239EE8
@L_8024D42C:
    mr r3, r31
    mr r4, r29
    li r5, 0x5
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D490
    mr r3, r30
    mr r4, r31
    li r5, 0x76
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x76
    bl fn_80239EE8
@L_8024D490:
    mr r3, r31
    mr r4, r29
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D4F4
    mr r3, r30
    mr r4, r31
    li r5, 0x77
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x77
    bl fn_80239EE8
@L_8024D4F4:
    mr r3, r31
    mr r4, r29
    li r5, 0xa
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D558
    mr r3, r30
    mr r4, r31
    li r5, 0x78
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x78
    bl fn_80239EE8
@L_8024D558:
    mr r3, r31
    mr r4, r29
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D5BC
    mr r3, r30
    mr r4, r31
    li r5, 0x79
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x79
    bl fn_80239EE8
@L_8024D5BC:
    mr r3, r31
    mr r4, r29
    li r5, 0x1e
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D620
    mr r3, r30
    mr r4, r31
    li r5, 0x7a
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x7a
    bl fn_80239EE8
@L_8024D620:
    mr r3, r31
    mr r4, r29
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D684
    mr r3, r30
    mr r4, r31
    li r5, 0x7b
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x7b
    bl fn_80239EE8
@L_8024D684:
    clrlwi r0, r28, 16
    cmplwi r0, 0x13
    beq @L_8024D698
    cmplwi r0, 0x154
    bne @L_8024D6A0
@L_8024D698:
    li r27, 0x1f
    b @L_8024D6BC
@L_8024D6A0:
    cmplwi r0, 0x5b
    bne @L_8024D6B0
    li r27, 0x20
    b @L_8024D6BC
@L_8024D6B0:
    cmplwi r0, 0x123
    bne @L_8024D6BC
    li r27, 0x21
@L_8024D6BC:
    addi r25, r1, 0x1c
    clrlwi r24, r26, 16
    li r26, 0x0
    b @L_8024D790
@L_8024D6CC:
    clrlslwi r0, r26, 16, 2
    mr r3, r31
    lwzx r4, r25, r0
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    bl fn_802367CC
    clrlwi. r22, r3, 16
    beq @L_8024D78C
    addi r23, r1, 0x8
    li r20, 0x0
    li r21, 0x0
    b @L_8024D774
@L_8024D700:
    clrlslwi r0, r21, 16, 1
    mr r3, r31
    lhzx r5, r23, r0
    mr r4, r27
    bl fn_8025C5A4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D770
    mr r3, r30
    mr r4, r31
    li r5, 0x7c
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x7c
    bl fn_80239EE8
    li r20, 0x1
    b @L_8024D780
@L_8024D770:
    addi r21, r21, 0x1
@L_8024D774:
    clrlwi r0, r21, 16
    cmplw r0, r22
    blt @L_8024D700
@L_8024D780:
    clrlwi r0, r20, 24
    cmplwi r0, 0x1
    beq @L_8024D79C
@L_8024D78C:
    addi r26, r26, 0x1
@L_8024D790:
    clrlwi r0, r26, 16
    cmplw r0, r24
    blt @L_8024D6CC
@L_8024D79C:
    mr r3, r31
    mr r4, r29
    li r5, 0x1d
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024D800
    mr r3, r30
    mr r4, r31
    li r5, 0x7d
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r30, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x7d
    bl fn_80239EE8
@L_8024D800:
    mr r3, r30
    lmw r20, 0x40(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
