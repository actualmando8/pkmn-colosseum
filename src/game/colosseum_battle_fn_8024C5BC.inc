    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    li r7, 0x1
    stw r0, 0x74(r1)
    stmw r21, 0x44(r1)
    mr r31, r3
    mr r30, r4
    mr r29, r5
    mr r27, r6
    mr r4, r31
    addi r5, r1, 0x1c
    li r26, 0x0
    li r3, 0x0
    li r6, 0x0
    bl fn_801F1C18
    mr r28, r3
    mr r3, r31
    mr r4, r27
    li r5, 0x3
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C660
    mr r4, r31
    li r3, 0x0
    li r5, 0x7e
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x7e
    bl fn_80239EE8
@L_8024C660:
    mr r3, r31
    mr r4, r27
    li r5, 0x4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C6C4
    mr r3, r26
    mr r4, r31
    li r5, 0x7f
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x7f
    bl fn_80239EE8
@L_8024C6C4:
    mr r3, r31
    mr r4, r27
    li r5, 0x6
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C728
    mr r3, r26
    mr r4, r31
    li r5, 0x80
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x80
    bl fn_80239EE8
@L_8024C728:
    mr r3, r31
    mr r4, r27
    li r5, 0x5
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C78C
    mr r3, r26
    mr r4, r31
    li r5, 0x81
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x81
    bl fn_80239EE8
@L_8024C78C:
    mr r3, r31
    mr r4, r27
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C7F0
    mr r3, r26
    mr r4, r31
    li r5, 0x82
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x82
    bl fn_80239EE8
@L_8024C7F0:
    mr r3, r31
    mr r4, r27
    li r5, 0xa
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C854
    mr r3, r26
    mr r4, r31
    li r5, 0x83
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x83
    bl fn_80239EE8
@L_8024C854:
    mr r3, r31
    mr r4, r27
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C8B8
    mr r3, r26
    mr r4, r31
    li r5, 0x84
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x84
    bl fn_80239EE8
@L_8024C8B8:
    mr r3, r31
    mr r4, r27
    li r5, 0x1e
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C91C
    mr r3, r26
    mr r4, r31
    li r5, 0x85
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x85
    bl fn_80239EE8
@L_8024C91C:
    mr r3, r31
    mr r4, r27
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C980
    mr r3, r26
    mr r4, r31
    li r5, 0x86
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x86
    bl fn_80239EE8
@L_8024C980:
    mr r3, r31
    mr r4, r30
    mr r5, r27
    bl fn_80236D60
    cmpwi r3, 0x0
    ble @L_8024C9E0
    mr r3, r26
    mr r4, r31
    li r5, 0x87
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r26, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x87
    bl fn_80239EE8
@L_8024C9E0:
    mr r3, r26
    mr r4, r31
    li r5, 0x88
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x88
    bl fn_80239EE8
    mr r3, r31
    mr r4, r30
    li r5, 0x3
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CA84
    mr r3, r27
    mr r4, r31
    li r5, 0x89
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x89
    bl fn_80239EE8
@L_8024CA84:
    mr r3, r31
    mr r4, r30
    li r5, 0x4
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CAE4
    mr r3, r27
    mr r4, r31
    li r5, 0x8a
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x8a
    bl fn_80239EE8
@L_8024CAE4:
    mr r3, r31
    mr r4, r30
    li r5, 0x6
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CB44
    mr r3, r27
    mr r4, r31
    li r5, 0x8b
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x8b
    bl fn_80239EE8
@L_8024CB44:
    mr r3, r31
    mr r4, r30
    li r5, 0x5
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CBA4
    mr r3, r27
    mr r4, r31
    li r5, 0x8c
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x8c
    bl fn_80239EE8
@L_8024CBA4:
    mr r3, r31
    mr r4, r30
    li r5, 0x9
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CC04
    mr r3, r27
    mr r4, r31
    li r5, 0x8d
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x8d
    bl fn_80239EE8
@L_8024CC04:
    mr r3, r31
    mr r4, r30
    li r5, 0xa
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CC64
    mr r3, r27
    mr r4, r31
    li r5, 0x8e
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x8e
    bl fn_80239EE8
@L_8024CC64:
    mr r3, r31
    mr r4, r30
    li r5, 0x18
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CCC4
    mr r3, r27
    mr r4, r31
    li r5, 0x8f
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x8f
    bl fn_80239EE8
@L_8024CCC4:
    mr r3, r31
    mr r4, r30
    li r5, 0x1e
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CD24
    mr r3, r27
    mr r4, r31
    li r5, 0x90
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x90
    bl fn_80239EE8
@L_8024CD24:
    mr r3, r31
    mr r4, r30
    li r5, 0x1c
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CD84
    mr r3, r27
    mr r4, r31
    li r5, 0x91
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x91
    bl fn_80239EE8
@L_8024CD84:
    addi r26, r1, 0x1c
    clrlwi r25, r28, 16
    li r28, 0x0
    b @L_8024CE54
@L_8024CD94:
    clrlslwi r0, r28, 16, 2
    mr r3, r31
    lwzx r4, r26, r0
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x0
    bl fn_802367CC
    clrlwi. r23, r3, 16
    beq @L_8024CE50
    addi r24, r1, 0x8
    li r21, 0x0
    li r22, 0x0
    b @L_8024CE38
@L_8024CDC8:
    clrlslwi r0, r22, 16, 1
    mr r3, r31
    lhzx r5, r24, r0
    li r4, 0x1f
    bl fn_8025C5A4
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CE34
    mr r3, r27
    mr r4, r31
    li r5, 0x92
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x92
    bl fn_80239EE8
    li r21, 0x1
    b @L_8024CE44
@L_8024CE34:
    addi r22, r22, 0x1
@L_8024CE38:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_8024CDC8
@L_8024CE44:
    clrlwi r0, r21, 24
    cmplwi r0, 0x1
    beq @L_8024CE60
@L_8024CE50:
    addi r28, r28, 0x1
@L_8024CE54:
    clrlwi r0, r28, 16
    cmplw r0, r25
    blt @L_8024CD94
@L_8024CE60:
    mr r3, r31
    mr r4, r30
    li r5, 0x1d
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024CEC0
    mr r3, r27
    mr r4, r31
    li r5, 0x93
    bl fn_80239984
    mr r27, r3
    mr r3, r30
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r31
    mr r8, r29
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x93
    bl fn_80239EE8
@L_8024CEC0:
    mr r3, r27
    lmw r21, 0x44(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
