    nofralloc
    stwu r1, -0xa0(r1)
    mflr r0
    li r6, 0x1
    li r7, 0x1
    stw r0, 0xa4(r1)
    stmw r14, 0x58(r1)
    mr r29, r3
    mr r30, r4
    mr r31, r5
    addi r5, r1, 0x2c
    mr r4, r29
    li r20, 0x0
    li r3, 0x0
    bl fn_801F1C18
    mr r22, r3
    mr r4, r29
    addi r5, r1, 0xc
    li r3, 0x0
    li r6, 0x0
    li r7, 0x1
    bl fn_801F1C18
    mr r23, r3
    addi r21, r1, 0x2c
    clrlwi r26, r22, 16
    addi r15, r1, 0x8
    addi r14, r1, 0xc
    clrlwi r28, r3, 16
    li r19, 0x0
    li r18, 0x0
    b @L_802420EC
@L_8024205C:
    clrlslwi r0, r18, 16, 2
    mr r3, r29
    lwzx r4, r14, r0
    addi r5, r1, 0x8
    bl fn_80237CB8
    clrlwi r27, r3, 16
    li r17, 0x0
    b @L_802420DC
@L_8024207C:
    clrlslwi r24, r17, 16, 1
    li r16, 0x0
    b @L_802420CC
@L_80242088:
    clrlslwi r25, r16, 16, 2
    lwzx r0, r21, r25
    cmplw r30, r0
    beq @L_802420C8
    mr r3, r29
    mr r4, r31
    bl fn_80239500
    lwzx r4, r21, r25
    mr r6, r3
    lhzx r5, r15, r24
    mr r3, r29
    bl fn_8023793C
    clrlwi r0, r3, 16
    cmplwi r0, 0x41
    bne @L_802420C8
    li r19, 0x1
@L_802420C8:
    addi r16, r16, 0x1
@L_802420CC:
    clrlwi r0, r16, 16
    cmplw r0, r26
    blt @L_80242088
    addi r17, r17, 0x1
@L_802420DC:
    clrlwi r0, r17, 16
    cmplw r0, r27
    blt @L_8024207C
    addi r18, r18, 0x1
@L_802420EC:
    clrlwi r0, r18, 16
    cmplw r0, r28
    blt @L_8024205C
    clrlwi r0, r19, 24
    cmplwi r0, 0x1
    bne @L_8024214C
    mr r4, r29
    li r3, 0x0
    li r5, 0x1a6
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r20, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1a6
    bl fn_80239EE8
@L_8024214C:
    addi r15, r1, 0x2c
    clrlwi r14, r22, 16
    li r16, 0x0
    b @L_802421D0
@L_8024215C:
    clrlslwi r0, r16, 16, 2
    lwzx r4, r15, r0
    cmplw r30, r4
    beq @L_802421CC
    mr r3, r29
    bl fn_8023785C
    clrlwi r0, r3, 24
    cmplwi r0, 0x2
    bne @L_802421CC
    mr r3, r20
    mr r4, r29
    li r5, 0x1a7
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r20, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1a7
    bl fn_80239EE8
    b @L_802421DC
@L_802421CC:
    addi r16, r16, 0x1
@L_802421D0:
    clrlwi r0, r16, 16
    cmplw r0, r14
    blt @L_8024215C
@L_802421DC:
    addi r15, r1, 0xc
    clrlwi r14, r23, 16
    li r16, 0x0
    b @L_80242268
@L_802421EC:
    clrlslwi r0, r16, 16, 2
    mr r3, r29
    lwzx r4, r15, r0
    bl fn_802377E8
    clrlwi r0, r3, 16
    cmplwi r0, 0x12e
    beq @L_80242218
    cmplwi r0, 0xd4
    beq @L_80242218
    cmplwi r0, 0x177
    bne @L_80242264
@L_80242218:
    mr r3, r20
    mr r4, r29
    li r5, 0x1a8
    bl fn_80239984
    mr r0, r3
    mr r3, r30
    mr r20, r0
    bl fn_80205B8C
    lis r6, 0x1
    mr r5, r3
    subi r3, r6, 0x139c
    mr r4, r29
    mr r8, r31
    li r6, 0x0
    li r7, 0x0
    li r9, 0x0
    li r10, 0x1a8
    bl fn_80239EE8
    b @L_80242274
@L_80242264:
    addi r16, r16, 0x1
@L_80242268:
    clrlwi r0, r16, 16
    cmplw r0, r14
    blt @L_802421EC
@L_80242274:
    mr r3, r20
    lmw r14, 0x58(r1)
    lwz r0, 0xa4(r1)
    mtlr r0
    addi r1, r1, 0xa0
    blr
