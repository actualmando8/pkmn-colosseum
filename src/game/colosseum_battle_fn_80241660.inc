    nofralloc
    stwu r1, -0x70(r1)
    mflr r0
    li r6, 0x1
    li r7, 0x1
    stw r0, 0x74(r1)
    stmw r21, 0x44(r1)
    mr r28, r3
    mr r29, r4
    mr r30, r5
    addi r5, r1, 0x1c
    mr r4, r28
    li r31, 0x0
    li r3, 0x0
    bl fn_801F1C18
    mr r26, r3
    li r21, 0x0
    li r22, 0x0
    b @L_80241708
@L_802416A8:
    clrlwi. r0, r22, 16
    beq @L_80241704
    cmplwi r0, 0x165
    beq @L_80241704
    cmplwi r0, 0x163
    beq @L_80241704
    mr r3, r28
    mr r4, r22
    li r5, 0x1
    bl fn_8023943C
    clrlwi. r0, r3, 24
    beq @L_80241704
    mr r4, r28
    mr r7, r22
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r8, 0x0
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_80241704
    li r21, 0x1
@L_80241704:
    addi r22, r22, 0x1
@L_80241708:
    lwz r3, -31272(r13)
    clrlwi r4, r22, 16
    lwz r0, 0x0(r3)
    cmplw r4, r0
    blt @L_802416A8
    clrlwi r0, r21, 24
    cmplwi r0, 0x1
    bne @L_80241770
    mr r4, r28
    li r3, 0x0
    li r5, 0x1b0
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b0
    bl fn_80239EE8
@L_80241770:
    addi r27, r1, 0x1c
    clrlwi r22, r26, 16
    li r24, 0x0
    b @L_80241834
@L_80241780:
    clrlslwi r0, r24, 16, 2
    lwzx r4, r27, r0
    cmplw r29, r4
    beq @L_80241830
    mr r3, r28
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    clrlwi. r25, r3, 16
    beq @L_80241830
    addi r23, r1, 0x8
    li r21, 0x0
    b @L_80241824
@L_802417B8:
    clrlslwi r0, r21, 16, 1
    mr r3, r28
    lhzx r4, r23, r0
    li r5, 0x1
    bl fn_8023943C
    clrlwi. r0, r3, 24
    beq @L_80241820
    mr r3, r31
    mr r4, r28
    li r5, 0x1b1
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b1
    bl fn_80239EE8
    b @L_80241830
@L_80241820:
    addi r21, r21, 0x1
@L_80241824:
    clrlwi r0, r21, 16
    cmplw r0, r25
    blt @L_802417B8
@L_80241830:
    addi r24, r24, 0x1
@L_80241834:
    clrlwi r0, r24, 16
    cmplw r0, r22
    blt @L_80241780
    addi r27, r1, 0x1c
    clrlwi r23, r26, 16
    li r25, 0x1
    li r24, 0x0
    b @L_802418D4
@L_80241854:
    clrlslwi r22, r24, 16, 2
    lwzx r3, r27, r22
    cmplw r29, r3
    beq @L_802418D0
    li r4, 0x0
    li r5, 0xfe
    li r6, 0x0
    bl fn_8012640C
    mr. r21, r3
    beq @L_802418D0
    bl fn_801F1170
    clrlwi. r0, r3, 24
    beq @L_802418D0
    mr r3, r21
    bl fn_801F0898
    clrlwi r0, r3, 16
    cmplwi r0, 0x13
    beq @L_802418A4
    li r25, 0x0
    b @L_802418E0
@L_802418A4:
    lwzx r3, r27, r22
    bl fn_80205184
    mr r0, r3
    mr r3, r28
    mr r4, r0
    li r5, 0x1
    bl fn_8023943C
    clrlwi. r0, r3, 24
    bne @L_802418D0
    li r25, 0x0
    b @L_802418E0
@L_802418D0:
    addi r24, r24, 0x1
@L_802418D4:
    clrlwi r0, r24, 16
    cmplw r0, r23
    blt @L_80241854
@L_802418E0:
    clrlwi. r0, r25, 24
    bne @L_80241930
    mr r3, r31
    mr r4, r28
    li r5, 0x1b2
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b2
    bl fn_80239EE8
@L_80241930:
    addi r25, r1, 0x1c
    clrlwi r24, r26, 16
    li r22, 0x0
    b @L_802419B8
@L_80241940:
    clrlslwi r0, r22, 16, 2
    lwzx r4, r25, r0
    cmplw r29, r4
    beq @L_802419B4
    mr r3, r28
    li r5, 0x12
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_802419B4
    mr r3, r31
    mr r4, r28
    li r5, 0x1b3
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b3
    bl fn_80239EE8
    b @L_802419C4
@L_802419B4:
    addi r22, r22, 0x1
@L_802419B8:
    clrlwi r0, r22, 16
    cmplw r0, r24
    blt @L_80241940
@L_802419C4:
    addi r25, r1, 0x1c
    clrlwi r24, r26, 16
    li r22, 0x0
    b @L_80241A4C
@L_802419D4:
    clrlslwi r0, r22, 16, 2
    lwzx r3, r25, r0
    cmplw r29, r3
    beq @L_80241A48
    li r4, 0x0
    li r5, 0xf9
    li r6, 0x0
    bl fn_8012640C
    clrlwi. r0, r3, 24
    beq @L_80241A48
    mr r3, r31
    mr r4, r28
    li r5, 0x1b4
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b4
    bl fn_80239EE8
    b @L_80241A58
@L_80241A48:
    addi r22, r22, 0x1
@L_80241A4C:
    clrlwi r0, r22, 16
    cmplw r0, r24
    blt @L_802419D4
@L_80241A58:
    addi r27, r1, 0x1c
    clrlwi r24, r26, 16
    li r25, 0x0
    b @L_80241B4C
@L_80241A68:
    clrlslwi r0, r25, 16, 2
    lwzx r23, r27, r0
    cmplw r29, r23
    beq @L_80241B48
    mr r3, r28
    mr r4, r23
    li r5, 0x8
    bl fn_80236BFC
    clrlwi. r0, r3, 24
    bne @L_80241A98
    li r0, -0x1
    b @L_80241AF4
@L_80241A98:
    mr r3, r28
    mr r4, r23
    li r5, 0x30
    bl fn_80237F74
    clrlwi r0, r3, 24
    mr r3, r23
    subfic r0, r0, 0x1
    li r4, 0x8
    cntlzw r0, r0
    srwi r5, r0, 5
    addi r26, r5, 0x1
    bl fn_80202108
    add r22, r3, r26
    mr r3, r23
    li r4, 0x8
    bl fn_80202234
    extsb r3, r3
    extsb r0, r22
    cmpw r0, r3
    blt @L_80241AF0
    li r0, 0x1
    b @L_80241AF4
@L_80241AF0:
    li r0, 0x0
@L_80241AF4:
    extsb. r0, r0
    bne @L_80241B48
    mr r3, r31
    mr r4, r28
    li r5, 0x1b5
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r28
    mr r8, r30
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1b5
    bl fn_80239EE8
    b @L_80241B58
@L_80241B48:
    addi r25, r25, 0x1
@L_80241B4C:
    clrlwi r0, r25, 16
    cmplw r0, r24
    blt @L_80241A68
@L_80241B58:
    mr r3, r31
    lmw r21, 0x44(r1)
    lwz r0, 0x74(r1)
    mtlr r0
    addi r1, r1, 0x70
    blr
