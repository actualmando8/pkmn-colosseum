    nofralloc
    stwu r1, -0xf0(r1)
    mflr r0
    stw r0, 0xf4(r1)
    stmw r21, 0xc4(r1)
    mr r29, r4
    mr r28, r5
    mr r30, r3
    li r31, 0x0
    mr r5, r29
    mr r4, r28
    bl fn_802395C8
    mr r25, r3
    mr r4, r30
    addi r5, r1, 0x5c
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1A6C
    mr r4, r30
    addi r5, r1, 0x3c
    li r3, 0x0
    li r6, 0x1
    li r7, 0x1
    bl fn_801F1C18
    mr r27, r3
    mr r4, r30
    addi r5, r1, 0x1c
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r26, r3
    addi r24, r1, 0x3c
    clrlwi r21, r27, 16
    li r23, 0x0
    b @L_8024C0D8
@L_8024C050:
    clrlslwi r22, r23, 16, 2
    lwzx r0, r24, r22
    cmplw r29, r0
    beq @L_8024C0D4
    mr r3, r30
    mr r4, r28
    bl fn_80239500
    lwzx r4, r24, r22
    mr r6, r3
    mr r3, r30
    mr r5, r25
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x43
    bne @L_8024C0D4
    mr r3, r31
    mr r4, r30
    li r5, 0x94
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x94
    bl fn_80239EE8
@L_8024C0D4:
    addi r23, r23, 0x1
@L_8024C0D8:
    clrlwi r0, r23, 16
    cmplw r0, r21
    blt @L_8024C050
    addi r24, r1, 0x3c
    clrlwi r21, r27, 16
    li r23, 0x0
    b @L_8024C17C
@L_8024C0F4:
    clrlslwi r22, r23, 16, 2
    lwzx r0, r24, r22
    cmplw r29, r0
    beq @L_8024C178
    mr r3, r30
    mr r4, r28
    bl fn_80239500
    lwzx r4, r24, r22
    mr r6, r3
    mr r3, r30
    mr r5, r25
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x42
    bne @L_8024C178
    mr r3, r31
    mr r4, r30
    li r5, 0x95
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x95
    bl fn_80239EE8
@L_8024C178:
    addi r23, r23, 0x1
@L_8024C17C:
    clrlwi r0, r23, 16
    cmplw r0, r21
    blt @L_8024C0F4
    addi r24, r1, 0x3c
    clrlwi r21, r27, 16
    li r23, 0x0
    b @L_8024C264
@L_8024C198:
    clrlslwi r0, r23, 16, 2
    lwzx r4, r24, r0
    cmplw r29, r4
    beq @L_8024C260
    mr r3, r30
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    clrlwi. r4, r3, 16
    mr r25, r3
    beq @L_8024C260
    addi r3, r1, 0x8
    li r22, 0x0
    b @L_8024C244
@L_8024C1D4:
    clrlslwi r0, r22, 16, 1
    lhzx r0, r3, r0
    cmplwi r0, 0xb6
    beq @L_8024C1F4
    cmplwi r0, 0xc5
    beq @L_8024C1F4
    cmplwi r0, 0xcb
    bne @L_8024C240
@L_8024C1F4:
    mr r3, r31
    mr r4, r30
    li r5, 0x96
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x96
    bl fn_80239EE8
    b @L_8024C250
@L_8024C240:
    addi r22, r22, 0x1
@L_8024C244:
    clrlwi r0, r22, 16
    cmplw r0, r4
    blt @L_8024C1D4
@L_8024C250:
    clrlwi r3, r22, 16
    clrlwi r0, r25, 16
    cmplw r3, r0
    blt @L_8024C270
@L_8024C260:
    addi r23, r23, 0x1
@L_8024C264:
    clrlwi r0, r23, 16
    cmplw r0, r21
    blt @L_8024C198
@L_8024C270:
    mr r4, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0xb6
    li r8, 0x0
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C2E4
    mr r3, r31
    mr r4, r30
    li r5, 0x97
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x97
    bl fn_80239EE8
    b @L_8024C3C8
@L_8024C2E4:
    mr r4, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0xc5
    li r8, 0x0
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C358
    mr r3, r31
    mr r4, r30
    li r5, 0x97
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x97
    bl fn_80239EE8
    b @L_8024C3C8
@L_8024C358:
    mr r4, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0xcb
    li r8, 0x0
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C3C8
    mr r3, r31
    mr r4, r30
    li r5, 0x97
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x97
    bl fn_80239EE8
@L_8024C3C8:
    addi r25, r1, 0x1c
    clrlwi r22, r26, 16
    li r24, 0x0
    b @L_8024C454
@L_8024C3D8:
    clrlslwi r0, r24, 16, 2
    mr r3, r30
    lwzx r4, r25, r0
    li r5, 0x14
    bl fn_80236BFC
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024C450
    clrlwi r0, r24, 16
    mr r3, r31
    subic r0, r0, 0x1
    mr r4, r30
    subfe r5, r0, r0
    addi r23, r5, 0x99
    mr r5, r23
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r28
    mr r10, r23
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    bl fn_80239EE8
@L_8024C450:
    addi r24, r24, 0x1
@L_8024C454:
    clrlwi r0, r24, 16
    cmplw r0, r22
    blt @L_8024C3D8
    addi r23, r1, 0x3c
    clrlwi r22, r27, 16
    li r21, 0x0
    b @L_8024C4F0
@L_8024C470:
    clrlslwi r0, r21, 16, 2
    lwzx r4, r23, r0
    cmplw r29, r4
    beq @L_8024C4EC
    mr r3, r30
    bl fn_80236520
    clrlwi r0, r3, 16
    cmplwi r0, 0xb6
    beq @L_8024C4A4
    cmplwi r0, 0xc5
    beq @L_8024C4A4
    cmplwi r0, 0xcb
    bne @L_8024C4EC
@L_8024C4A4:
    mr r3, r31
    mr r4, r30
    li r5, 0x9a
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x9a
    bl fn_80239EE8
@L_8024C4EC:
    addi r21, r21, 0x1
@L_8024C4F0:
    clrlwi r0, r21, 16
    cmplw r0, r22
    blt @L_8024C470
    addi r21, r1, 0x3c
    clrlwi r22, r27, 16
    li r24, 0x0
    b @L_8024C598
@L_8024C50C:
    clrlslwi r23, r24, 16, 2
    lwzx r4, r21, r23
    cmplw r29, r4
    beq @L_8024C594
    mr r3, r30
    bl fn_802376EC
    lwzx r6, r21, r23
    mr r23, r3
    mr r3, r30
    mr r4, r29
    mr r5, r28
    li r7, 0x0
    bl fn_8023C370
    clrlwi r0, r23, 16
    cmpw r0, r3
    bgt @L_8024C594
    mr r3, r31
    mr r4, r30
    li r5, 0x9b
    bl fn_80239984
    mr r0, r3
    mr r3, r29
    mr r31, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r30
    mr r8, r28
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x9b
    bl fn_80239EE8
@L_8024C594:
    addi r24, r24, 0x1
@L_8024C598:
    clrlwi r0, r24, 16
    cmplw r0, r22
    blt @L_8024C50C
    mr r3, r31
    lmw r21, 0xc4(r1)
    lwz r0, 0xf4(r1)
    mtlr r0
    addi r1, r1, 0xf0
    blr
