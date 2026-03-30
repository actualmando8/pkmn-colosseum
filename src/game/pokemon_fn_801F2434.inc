    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    cmplwi r4, 0x0
    stw r0, 0x44(r1)
    stmw r26, 0x28(r1)
    bne @L_801F2454
    li r3, -0x1
    b @L_801F2584
    @L_801F2454:
    li r7, 0x0
    addi r5, r1, 0x8
    mr r6, r7
    b @L_801F2470
    @L_801F2464:
    clrlslwi r0, r7, 16, 2
    addi r7, r7, 0x1
    stwx r6, r5, r0
    @L_801F2470:
    clrlwi r0, r7, 16
    cmplwi r0, 0x8
    blt @L_801F2464
    mr r7, r4
    addi r4, r1, 0x8
    li r5, 0x1
    li r6, 0x2
    bl fn_801F61EC
    clrlwi. r4, r3, 16
    mr r29, r3
    bne @L_801F24A4
    li r3, -0x1
    b @L_801F2584
    @L_801F24A4:
    addi r3, r1, 0x8
    li r5, 0x0
    b @L_801F2500
    @L_801F24B0:
    clrlslwi r0, r5, 16, 2
    lwzx r26, r3, r0
    cmplwi r26, 0x0
    beq @L_801F24FC
    mr r3, r26
    li r4, 0x1
    bl fn_80203ADC
    mr r28, r3
    mr r3, r26
    li r4, 0x1
    bl fn_80203B5C
    clrlwi. r0, r3, 16
    bne @L_801F24E8
    li r3, 0x1
    @L_801F24E8:
    clrlwi r4, r28, 16
    clrlwi r0, r3, 16
    mulli r3, r4, 0x64
    divw r31, r3, r0
    b @L_801F250C
    @L_801F24FC:
    addi r5, r5, 0x1
    @L_801F2500:
    clrlwi r0, r5, 16
    cmplw r0, r4
    blt @L_801F24B0
    @L_801F250C:
    addi r28, r1, 0x8
    clrlwi r30, r29, 16
    li r27, 0x0
    b @L_801F2574
    @L_801F251C:
    clrlslwi r0, r27, 16, 2
    lwzx r26, r28, r0
    cmplwi r26, 0x0
    beq @L_801F2570
    mr r3, r26
    li r4, 0x1
    bl fn_80203ADC
    mr r29, r3
    mr r3, r26
    li r4, 0x1
    bl fn_80203B5C
    clrlwi. r0, r3, 16
    bne @L_801F2554
    li r3, 0x1
    @L_801F2554:
    clrlwi r4, r29, 16
    clrlwi r0, r3, 16
    mulli r3, r4, 0x64
    divw r0, r3, r0
    cmpw r0, r31
    bge @L_801F2570
    mr r31, r0
    @L_801F2570:
    addi r27, r27, 0x1
    @L_801F2574:
    clrlwi r0, r27, 16
    cmplw r0, r30
    blt @L_801F251C
    mr r3, r31
    @L_801F2584:
    lmw r26, 0x28(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
