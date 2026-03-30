    nofralloc
    stwu r1, -0x30(r1)
    mflr r0
    li r5, 0x0
    li r6, 0x0
    stw r0, 0x34(r1)
    stmw r22, 0x8(r1)
    mr r31, r3
    mr r22, r4
    li r3, 0x0
    li r4, 0x0
    bl fn_801F54A4
    mr r0, r3
    mr r3, r22
    mr r26, r0
    bl fn_8020E204
    mr. r27, r3
    bne @L_801F0764
    li r3, 0x0
    b @L_801F0884
    @L_801F0764:
    bl fn_8020E1D4
    mr r29, r3
    mr r3, r27
    bl fn_8020E1A4
    mr r30, r3
    mr r3, r27
    bl fn_8020E1BC
    clrlwi r28, r3, 24
    clrlwi r30, r30, 24
    clrlwi r29, r29, 24
    li r25, 0x0
    b @L_801F0874
    @L_801F0794:
    mr r3, r26
    mr r6, r25
    li r4, 0x0
    li r5, 0x35
    bl fn_801F54A4
    mr r23, r3
    cmplw r23, r31
    bne @L_801F07B8
    b @L_801F0884
    @L_801F07B8:
    li r24, 0x0
    b @L_801F0864
    @L_801F07C0:
    mr r3, r23
    mr r6, r24
    li r4, 0x0
    li r5, 0x7
    bl fn_801F76B8
    mr r22, r3
    cmplw r22, r31
    bne @L_801F07E8
    mr r3, r23
    b @L_801F0884
    @L_801F07E8:
    li r27, 0x0
    b @L_801F0818
    @L_801F07F0:
    mr r3, r22
    mr r6, r27
    li r4, 0x0
    li r5, 0x45
    bl fn_801FB1C0
    cmplw r3, r31
    bne @L_801F0814
    mr r3, r23
    b @L_801F0884
    @L_801F0814:
    addi r27, r27, 0x1
    @L_801F0818:
    clrlwi r0, r27, 16
    cmpw r0, r28
    blt @L_801F07F0
    li r27, 0x0
    b @L_801F0854
    @L_801F082C:
    mr r3, r22
    mr r6, r27
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    cmplw r3, r31
    bne @L_801F0850
    mr r3, r23
    b @L_801F0884
    @L_801F0850:
    addi r27, r27, 0x1
    @L_801F0854:
    clrlwi r0, r27, 16
    cmpw r0, r30
    blt @L_801F082C
    addi r24, r24, 0x1
    @L_801F0864:
    clrlwi r0, r24, 16
    cmpw r0, r29
    blt @L_801F07C0
    addi r25, r25, 0x1
    @L_801F0874:
    clrlwi r0, r25, 16
    cmplwi r0, 0x2
    blt @L_801F0794
    li r3, 0x0
    @L_801F0884:
    lmw r22, 0x8(r1)
    lwz r0, 0x34(r1)
    mtlr r0
    addi r1, r1, 0x30
    blr
