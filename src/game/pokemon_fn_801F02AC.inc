    nofralloc
    stwu r1, -0x90(r1)
    mflr r0
    stw r0, 0x94(r1)
    clrlwi. r0, r3, 16
    stmw r14, 0x48(r1)
    mr r15, r3
    mr r16, r4
    mr r18, r5
    bne @L_801F02D8
    li r3, 0x0
    b @L_801F0704
    @L_801F02D8:
    bl fn_801F61BC
    clrlwi r4, r15, 16
    stw r3, 0x40(r1)
    cmplwi r4, 0x1
    bne @L_801F02F0
    b @L_801F0704
    @L_801F02F0:
    subi r0, r4, 0x11
    cmplwi r0, 0xc
    bgt @L_801F0418
    lis r4, jumptable_803752F8@ha
    slwi r0, r0, 2
    addi r4, r4, jumptable_803752F8@l
    lwzx r0, r4, r0
    mtctr r0
    bctr
    li r4, 0x0
    li r5, 0x36
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x42
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x45
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x46
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x47
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x48
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x49
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x4b
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x4c
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x4d
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x4e
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    li r4, 0x0
    li r5, 0x4f
    li r6, 0x0
    bl fn_801F54A4
    b @L_801F0704
    @L_801F0418:
    mr r3, r18
    bl fn_8020E204
    mr. r14, r3
    bne @L_801F0430
    li r3, 0x0
    b @L_801F0704
    @L_801F0430:
    cmplwi r16, 0x0
    beq @L_801F0458
    mr r17, r16
    mr r3, r16
    mr r4, r18
    bl fn_801F0718
    mr. r16, r3
    bne @L_801F0458
    li r3, 0x0
    b @L_801F0704
    @L_801F0458:
    mr r3, r14
    li r21, 0x0
    li r19, 0x0
    bl fn_8020E1D4
    mr r18, r3
    mr r3, r14
    bl fn_8020E1A4
    clrlwi r14, r3, 24
    clrlwi r27, r18, 24
    addi r24, r1, 0x20
    addi r25, r1, 0x10
    clrlwi r30, r15, 16
    addi r26, r1, 0x8
    li r22, 0x0
    b @L_801F06F4
    @L_801F0494:
    lwz r3, 0x40(r1)
    mr r6, r22
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    clrlslwi r28, r22, 16, 2
    cmplwi r30, 0x4
    stwx r3, r26, r28
    clrlwi r0, r22, 16
    bne @L_801F04CC
    cmplwi r0, 0x0
    bne @L_801F04CC
    lwzx r3, r26, r28
    b @L_801F0704
    @L_801F04CC:
    cmplwi r30, 0x5
    bne @L_801F04E8
    clrlwi r0, r22, 16
    cmplwi r0, 0x1
    bne @L_801F04E8
    lwzx r3, r26, r28
    b @L_801F0704
    @L_801F04E8:
    lwzx r3, r26, r28
    cmplw r3, r16
    bne @L_801F0508
    clrlwi r0, r15, 16
    li r4, 0x1
    cmplwi r0, 0x2
    bne @L_801F051C
    b @L_801F0704
    @L_801F0508:
    clrlwi r0, r15, 16
    li r4, 0x0
    cmplwi r0, 0x3
    bne @L_801F051C
    b @L_801F0704
    @L_801F051C:
    clrlwi r31, r4, 24
    li r18, 0x0
    li r23, 0x0
    b @L_801F06E4
    @L_801F052C:
    lwzx r3, r26, r28
    mr r6, r23
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    clrlslwi r29, r21, 16, 2
    cmplwi r30, 0xb
    stwx r3, r25, r29
    bne @L_801F0568
    clrlwi. r0, r22, 16
    bne @L_801F0568
    clrlwi. r0, r23, 16
    bne @L_801F0568
    lwzx r3, r25, r29
    b @L_801F0704
    @L_801F0568:
    cmplwi r31, 0x1
    bne @L_801F05C8
    clrlwi r0, r15, 16
    cmplwi r0, 0x6
    bne @L_801F058C
    clrlwi. r0, r23, 16
    bne @L_801F058C
    lwzx r3, r25, r29
    b @L_801F0704
    @L_801F058C:
    clrlwi r0, r15, 16
    cmplwi r0, 0x7
    bne @L_801F05AC
    clrlwi r0, r23, 16
    cmplwi r0, 0x1
    bne @L_801F05AC
    lwzx r3, r25, r29
    b @L_801F0704
    @L_801F05AC:
    clrlwi r0, r15, 16
    cmplwi r0, 0x8
    bne @L_801F0604
    lwzx r3, r25, r29
    cmplw r17, r3
    beq @L_801F0604
    b @L_801F0704
    @L_801F05C8:
    clrlwi r0, r15, 16
    cmplwi r0, 0x9
    bne @L_801F05E4
    clrlwi. r0, r23, 16
    bne @L_801F05E4
    lwzx r3, r25, r29
    b @L_801F0704
    @L_801F05E4:
    clrlwi r0, r15, 16
    cmplwi r0, 0xa
    bne @L_801F0604
    clrlwi r0, r23, 16
    cmplwi r0, 0x1
    bne @L_801F0604
    lwzx r3, r25, r29
    b @L_801F0704
    @L_801F0604:
    li r20, 0x0
    b @L_801F06D0
    @L_801F060C:
    lwzx r3, r25, r29
    mr r6, r20
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    clrlslwi r0, r19, 16, 2
    cmplwi r31, 0x1
    stwx r3, r24, r0
    bne @L_801F0688
    clrlwi r3, r15, 16
    cmplwi r3, 0xc
    bne @L_801F064C
    clrlwi. r3, r18, 16
    bne @L_801F064C
    lwzx r3, r24, r0
    b @L_801F0704
    @L_801F064C:
    clrlwi r3, r15, 16
    cmplwi r3, 0xd
    bne @L_801F066C
    clrlwi r3, r18, 16
    cmplwi r3, 0x1
    bne @L_801F066C
    lwzx r3, r24, r0
    b @L_801F0704
    @L_801F066C:
    clrlwi r3, r15, 16
    cmplwi r3, 0xe
    bne @L_801F06C4
    lwzx r3, r24, r0
    cmplw r17, r3
    beq @L_801F06C4
    b @L_801F0704
    @L_801F0688:
    clrlwi r3, r15, 16
    cmplwi r3, 0xf
    bne @L_801F06A4
    clrlwi. r3, r18, 16
    bne @L_801F06A4
    lwzx r3, r24, r0
    b @L_801F0704
    @L_801F06A4:
    clrlwi r3, r15, 16
    cmplwi r3, 0x10
    bne @L_801F06C4
    clrlwi r3, r18, 16
    cmplwi r3, 0x1
    bne @L_801F06C4
    lwzx r3, r24, r0
    b @L_801F0704
    @L_801F06C4:
    addi r19, r19, 0x1
    addi r18, r18, 0x1
    addi r20, r20, 0x1
    @L_801F06D0:
    clrlwi r0, r20, 16
    cmpw r0, r14
    blt @L_801F060C
    addi r21, r21, 0x1
    addi r23, r23, 0x1
    @L_801F06E4:
    clrlwi r0, r23, 16
    cmpw r0, r27
    blt @L_801F052C
    addi r22, r22, 0x1
    @L_801F06F4:
    clrlwi r0, r22, 16
    cmplwi r0, 0x2
    blt @L_801F0494
    li r3, 0x0
    @L_801F0704:
    lmw r14, 0x48(r1)
    lwz r0, 0x94(r1)
    mtlr r0
    addi r1, r1, 0x90
    blr
