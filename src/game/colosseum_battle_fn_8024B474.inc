    nofralloc
    stwu r1, -0xf0(r1)
    mflr r0
    stw r0, 0xf4(r1)
    stmw r22, 0xc8(r1)
    mr r29, r4
    mr r28, r5
    mr r30, r3
    li r31, 0x0
    mr r5, r29
    mr r4, r28
    bl fn_802395C8
    mr r24, r3
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
    addi r25, r1, 0x3c
    clrlwi r22, r27, 16
    li r26, 0x0
    b @L_8024B588
@L_8024B500:
    clrlslwi r23, r26, 16, 2
    lwzx r0, r25, r23
    cmplw r29, r0
    beq @L_8024B584
    mr r3, r30
    mr r4, r28
    bl fn_80239500
    lwzx r4, r25, r23
    mr r6, r3
    mr r3, r30
    mr r5, r24
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x43
    bne @L_8024B584
    mr r3, r31
    mr r4, r30
    li r5, 0xa9
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
    li r10, 0xa9
    bl fn_80239EE8
@L_8024B584:
    addi r26, r26, 0x1
@L_8024B588:
    clrlwi r0, r26, 16
    cmplw r0, r22
    blt @L_8024B500
    addi r25, r1, 0x3c
    clrlwi r22, r27, 16
    li r26, 0x0
    b @L_8024B62C
@L_8024B5A4:
    clrlslwi r23, r26, 16, 2
    lwzx r0, r25, r23
    cmplw r29, r0
    beq @L_8024B628
    mr r3, r30
    mr r4, r28
    bl fn_80239500
    lwzx r4, r25, r23
    mr r6, r3
    mr r3, r30
    mr r5, r24
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x42
    bne @L_8024B628
    mr r3, r31
    mr r4, r30
    li r5, 0xaa
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
    li r10, 0xaa
    bl fn_80239EE8
@L_8024B628:
    addi r26, r26, 0x1
@L_8024B62C:
    clrlwi r0, r26, 16
    cmplw r0, r22
    blt @L_8024B5A4
    addi r25, r1, 0x3c
    clrlwi r22, r27, 16
    li r24, 0x0
    b @L_8024B714
@L_8024B648:
    clrlslwi r0, r24, 16, 2
    lwzx r4, r25, r0
    cmplw r29, r4
    beq @L_8024B710
    mr r3, r30
    addi r5, r1, 0x8
    li r6, 0x0
    li r7, 0x1
    bl fn_802367CC
    clrlwi. r4, r3, 16
    mr r26, r3
    beq @L_8024B710
    addi r3, r1, 0x8
    li r23, 0x0
    b @L_8024B6F4
@L_8024B684:
    clrlslwi r0, r23, 16, 1
    lhzx r0, r3, r0
    cmplwi r0, 0xb6
    beq @L_8024B6A4
    cmplwi r0, 0xc5
    beq @L_8024B6A4
    cmplwi r0, 0xcb
    bne @L_8024B6F0
@L_8024B6A4:
    mr r3, r31
    mr r4, r30
    li r5, 0xab
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
    li r10, 0xab
    bl fn_80239EE8
    b @L_8024B700
@L_8024B6F0:
    addi r23, r23, 0x1
@L_8024B6F4:
    clrlwi r0, r23, 16
    cmplw r0, r4
    blt @L_8024B684
@L_8024B700:
    clrlwi r3, r23, 16
    clrlwi r0, r26, 16
    cmplw r3, r0
    blt @L_8024B720
@L_8024B710:
    addi r24, r24, 0x1
@L_8024B714:
    clrlwi r0, r24, 16
    cmplw r0, r22
    blt @L_8024B648
@L_8024B720:
    mr r4, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0xb6
    li r8, 0x0
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024B794
    mr r3, r31
    mr r4, r30
    li r5, 0xac
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
    li r10, 0xac
    bl fn_80239EE8
    b @L_8024B878
@L_8024B794:
    mr r4, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0xc5
    li r8, 0x0
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024B808
    mr r3, r31
    mr r4, r30
    li r5, 0xac
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
    li r10, 0xac
    bl fn_80239EE8
    b @L_8024B878
@L_8024B808:
    mr r4, r30
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0xcb
    li r8, 0x0
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024B878
    mr r3, r31
    mr r4, r30
    li r5, 0xac
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
    li r10, 0xac
    bl fn_80239EE8
@L_8024B878:
    mr r4, r30
    mr r8, r29
    li r3, 0x0
    li r5, 0x1
    li r6, 0x1
    li r7, 0x10e
    bl fn_801F1990
    clrlwi r0, r3, 24
    cmplwi r0, 0x1
    bne @L_8024B8E8
    mr r3, r31
    mr r4, r30
    li r5, 0xad
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
    li r10, 0xad
    bl fn_80239EE8
@L_8024B8E8:
    addi r24, r1, 0x3c
    clrlwi r23, r27, 16
    li r22, 0x0
    b @L_8024B978
@L_8024B8F8:
    clrlslwi r0, r22, 16, 2
    lwzx r4, r24, r0
    cmplw r29, r4
    beq @L_8024B974
    mr r3, r30
    bl fn_80236520
    clrlwi r0, r3, 16
    cmplwi r0, 0xb6
    beq @L_8024B92C
    cmplwi r0, 0xc5
    beq @L_8024B92C
    cmplwi r0, 0xcb
    bne @L_8024B974
@L_8024B92C:
    mr r3, r31
    mr r4, r30
    li r5, 0xae
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
    li r10, 0xae
    bl fn_80239EE8
@L_8024B974:
    addi r22, r22, 0x1
@L_8024B978:
    clrlwi r0, r22, 16
    cmplw r0, r23
    blt @L_8024B8F8
    addi r22, r1, 0x3c
    clrlwi r23, r27, 16
    li r25, 0x0
    b @L_8024BA20
@L_8024B994:
    clrlslwi r24, r25, 16, 2
    lwzx r4, r22, r24
    cmplw r29, r4
    beq @L_8024BA1C
    mr r3, r30
    bl fn_802376EC
    lwzx r6, r22, r24
    mr r24, r3
    mr r3, r30
    mr r4, r29
    mr r5, r28
    li r7, 0x0
    bl fn_8023C370
    clrlwi r0, r24, 16
    cmpw r0, r3
    bgt @L_8024BA1C
    mr r3, r31
    mr r4, r30
    li r5, 0xaf
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
    li r10, 0xaf
    bl fn_80239EE8
@L_8024BA1C:
    addi r25, r25, 0x1
@L_8024BA20:
    clrlwi r0, r25, 16
    cmplw r0, r23
    blt @L_8024B994
    mr r3, r31
    lmw r22, 0xc8(r1)
    lwz r0, 0xf4(r1)
    mtlr r0
    addi r1, r1, 0xf0
    blr
