    nofralloc
    stwu r1, -0x20(r1)
    mflr r0
    stw r0, 0x24(r1)
    stmw r27, 0xc(r1)
    mr. r27, r3
    mr r28, r4
    mr r29, r5
    mr r30, r6
    mr r31, r7
    beq @L_8013551C
    clrlwi. r0, r28, 24
    beq @L_8013551C
    clrlwi. r0, r29, 24
    beq @L_8013551C
    clrlwi. r0, r30, 24
    beq @L_8013551C
    clrlwi. r0, r31, 24
    beq @L_8013551C
    bl fn_80135708
    cmplwi r27, 0x0
    mr r3, r27
    bne @L_8013543C
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_80135450
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_80135450
    @L_8013543C:
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_80135450
    clrlwi r4, r28, 24
    bl fn_80135A60
    @L_80135450:
    cmplwi r27, 0x0
    mr r3, r27
    bne @L_80135480
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_80135494
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_80135494
    @L_80135480:
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_80135494
    clrlwi r4, r29, 24
    bl fn_80135A50
    @L_80135494:
    cmplwi r27, 0x0
    mr r3, r27
    bne @L_801354C4
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_801354D8
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_801354D8
    @L_801354C4:
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_801354D8
    clrlwi r4, r30, 24
    bl fn_80135A40
    @L_801354D8:
    cmplwi r27, 0x0
    mr r3, r27
    bne @L_80135508
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_8013551C
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    beq @L_8013551C
    @L_80135508:
    bl fn_80135B0C
    cmplwi r3, 0x0
    beq @L_8013551C
    clrlwi r4, r31, 24
    bl fn_80135A30
    @L_8013551C:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
