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
    beq @L_80135324
    bl fn_80135338
    cmplwi r27, 0x0
    mr r3, r27
    bne @L_801352F8
    li r3, 0x0
    li r4, 0x0
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_801352E0
    li r3, 0x0
    b @L_80135308
    @L_801352E0:
    li r4, 0x1
    bl fn_80129280
    cmplwi r3, 0x0
    bne @L_801352F8
    li r3, 0x0
    b @L_80135308
    @L_801352F8:
    bl fn_80135B0C
    cmplwi r3, 0x0
    bne @L_80135308
    li r3, 0x0
    @L_80135308:
    cmplwi r3, 0x0
    beq @L_80135324
    mr r4, r28
    mr r5, r29
    mr r6, r30
    mr r7, r31
    bl fn_801353C0
    @L_80135324:
    lmw r27, 0xc(r1)
    lwz r0, 0x24(r1)
    mtlr r0
    addi r1, r1, 0x20
    blr
