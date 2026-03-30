    nofralloc
    stwu r1, -0x40(r1)
    mflr r0
    stw r0, 0x44(r1)
    stmw r23, 0x1c(r1)
    mr r24, r4
    mr r25, r5
    mr r26, r6
    mr r27, r3
    li r4, 0x0
    li r5, 0x44
    li r6, 0x0
    bl fn_801FB1C0
    cmplwi r3, 0x0
    bne @L_801F7C94
    li r3, 0x0
    b @L_801F7E4C
    @L_801F7C94:
    addi r5, r1, 0x8
    li r4, 0x2
    li r6, 0x0
    li r7, 0x0
    li r8, 0x0
    bl fn_80129BC8
    mr. r31, r3
    bne @L_801F7CBC
    li r3, 0x0
    b @L_801F7E4C
    @L_801F7CBC:
    li r5, 0x0
    addi r3, r1, 0xc
    mr r4, r5
    b @L_801F7CD8
    @L_801F7CCC:
    clrlslwi r0, r5, 16, 1
    addi r5, r5, 0x1
    sthx r4, r3, r0
    @L_801F7CD8:
    clrlwi r0, r5, 16
    cmplwi r0, 0x2
    blt @L_801F7CCC
    li r30, 0x0
    addi r28, r1, 0xc
    mr r23, r30
    b @L_801F7D38
    @L_801F7CF4:
    mr r3, r27
    mr r6, r23
    li r4, 0x0
    li r5, 0x46
    bl fn_801FB1C0
    mr r29, r3
    bl fn_802062FC
    clrlwi. r0, r3, 24
    beq @L_801F7D34
    mr r3, r29
    bl fn_80204C08
    clrlwi. r0, r3, 16
    beq @L_801F7D34
    clrlslwi r0, r30, 16, 1
    addi r30, r30, 0x1
    sthx r3, r28, r0
    @L_801F7D34:
    addi r23, r23, 0x1
    @L_801F7D38:
    clrlwi r0, r23, 16
    cmplwi r0, 0x2
    blt @L_801F7CF4
    clrlwi r0, r25, 16
    li r5, 0x0
    li r4, 0x0
    b @L_801F7D60
    @L_801F7D54:
    clrlslwi r3, r5, 16, 1
    addi r5, r5, 0x1
    sthx r4, r24, r3
    @L_801F7D60:
    clrlwi r3, r5, 16
    cmplw r3, r0
    blt @L_801F7D54
    li r28, 0x0
    li r29, 0x0
    b @L_801F7E38
    @L_801F7D78:
    clrlslwi r0, r29, 16, 2
    add r23, r31, r0
    mr r3, r23
    bl fn_801429E8
    clrlwi. r0, r3, 24
    beq @L_801F7E34
    mr r3, r23
    li r4, 0x0
    li r5, 0x1b
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r27, r3, 16
    beq @L_801F7E34
    mr r3, r23
    li r4, 0x0
    li r5, 0x1c
    li r6, 0x0
    bl fn_80142CF4
    clrlwi. r5, r3, 16
    beq @L_801F7E34
    clrlwi r0, r26, 24
    cmplwi r0, 0x1
    bne @L_801F7E18
    addi r4, r1, 0xc
    clrlwi r0, r30, 16
    li r6, 0x0
    b @L_801F7E04
    @L_801F7DE4:
    clrlslwi r3, r6, 16, 1
    lhzx r3, r4, r3
    cmplw r27, r3
    bne @L_801F7E00
    clrlwi. r3, r5, 16
    beq @L_801F7E00
    subi r5, r5, 0x1
    @L_801F7E00:
    addi r6, r6, 0x1
    @L_801F7E04:
    clrlwi r3, r6, 16
    cmplw r3, r0
    blt @L_801F7DE4
    clrlwi. r0, r5, 16
    beq @L_801F7E34
    @L_801F7E18:
    clrlwi r3, r28, 16
    clrlwi r0, r25, 16
    cmplw r3, r0
    bge @L_801F7E34
    clrlslwi r0, r28, 16, 1
    addi r28, r28, 0x1
    sthx r27, r24, r0
    @L_801F7E34:
    addi r29, r29, 0x1
    @L_801F7E38:
    lhz r0, 0x8(r1)
    clrlwi r3, r29, 16
    cmplw r3, r0
    blt @L_801F7D78
    mr r3, r28
    @L_801F7E4C:
    lmw r23, 0x1c(r1)
    lwz r0, 0x44(r1)
    mtlr r0
    addi r1, r1, 0x40
    blr
